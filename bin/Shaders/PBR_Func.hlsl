#define PI 3.14159265359

#ifndef COMPILEFROMFILE
#define SPECULAR 0
#define METALLIC 0
#define NDF_GGX 1
#define FRESNEL_SCHLICK 1
#define GEOMETRIC_SMITH_SCHLICK_GGX 1
#define DISNEY_BRDF 1
#endif

#define PBR_ENV_MIP_MAPS 7.0f

#define g_LightIntensity LightIntensity
#define g_AmbientLightIntensity AmbientLightIntensity
#define g_ReflectionIntensity ReflectionIntensity

Texture2D BRDFLUT : register(t7);

SamplerState LinearWrapSampler : register(s0);
SamplerState AnisoWrapSampler : register(s1);
SamplerState LinearClampSampler : register(s2);
SamplerState AnisoClampSampler : register(s3);

cbuffer cbRendererConfig : register(b6)
{
	float LightIntensity;
	float AmbientLightIntensity;
	float ReflectionIntensity;
	float pad0;

	int pad;

	int Rc;
	int Gc;
	int Bc;
};

//-----------------Disney BRDFHelpers----------------------------

float3 Diffuse(float3 pAlbedo)
{
	///QUITE PI
	return pAlbedo / PI;
};

float sqr(float x)
{
	return x * x;
}

float GTR2_aniso(float NdH, float HdX, float HdY, float ax, float ay)
{
	return 1.0f / (PI * ax * ax * sqr(sqr(HdX / ax) + sqr(HdY / ay) + NdH * NdH));
}

float smithG_GGX(float NdV, float alphaG)
{
	float a = alphaG * alphaG;
	float b = NdV * NdV;
	return 1.0f / (NdV + sqrt(a + b - a * b));
}

float GTR1(float NdH, float a)
{
	if (a >= 1.0f)
	{
		return 1.0f / PI;
	}

	float a2 = a * a;
	float t = 1.0f + (a2 - 1.0f) * NdH * NdH;
	return (a2 - 1.0f) / (PI * log(a2) * t);
}

//-----------------NormalDistribution Function---------------------------

float NormalDistribution_GGX(float a, float NdH)
{
	//Isotropic ggx
	float a2 = a * a;
	float NdH2 = NdH * NdH;

	float denominator = NdH2 * (a2 - 1.0f) + 1.0f;
	denominator *= denominator;
	denominator *= PI;
	return a2 / denominator;
}

float NormalDistribution_BlinnPhon(float a, float NdH)
{
	return (1.0f / (PI * a * a)) * pow(NdH, 2.0f / (a * a) - 2.0f);
}

float NormalDistribution_Beckmann(float a, float NdH)
{
	float a2 = a * a;
	float NdH2 = NdH * NdH;

	return (1.0f / (PI * a2 * NdH2 * NdH2 + 0.0001)) * exp((NdH2 - 1.0f) / (a2 * NdH2));
}

//-------------------Geometric Shadowing---------------------------------

float Geometric_Implicit(float a, float NdV)
{
	return NdV * NdV;
}

float Geometric_Neumann(float a, float NdV, float NdL)
{
	return (NdL * NdV) / max(NdL, NdV);
}

float Geometric_CookTorrance(float a, float NdV, float NdL, float NdH, float VdH)
{
	return min(1.0f, min((2.0f * NdH * NdV) / VdH, (2.0f * NdH * NdL) / VdH));
}

float Geometric_Kelemen(float a, float NdV, float NdL, float LdV)
{
	return (2.0f * NdL * NdV) / (1.0f + LdV);
}

float Geometric_Beckman(float a, float dotValue)
{
	float c = dotValue / (a * sqrt(1.0f - dotValue * dotValue));

	if (c >= 1.6f)
	{
		return 1.0f;
	}
	else
	{
		float c2 = c * c;
		return (3.535f * c + 2.181f * c2) / (1 + 2.276f * c + 2.577f * c2);
	}
}

float Geometric_Smith_Beckmann(float a, float NdV, float NdL)
{
	return Geometric_Beckman(a, NdV) * Geometric_Beckman(a, NdL);
}

float Geometric_GGX(float a, float dotValue)
{
	float a2 = a * a;
	return (2.0f * dotValue) / (dotValue + sqrt(a2 + ((1.0f - a2) * (dotValue * dotValue))));
}

float Geometric_Smith_GGX(float a, float NdV, float NdL)
{
	return Geometric_GGX(a, NdV) * Geometric_GGX(a, NdL);
}

float Geometric_Smith_Schlick_GGX(float a, float NdV, float NdL)
{
	//smith schlick-GGX
	float k = a * 0.5f;
	float GV = NdV / (NdV * (1.0f - k) + k);
	float GL = NdL / (NdL * (1.0f - k) + k);

	return GV * GL;
}

//-------------------------Fresnel------------------------------

float3 Fresnel_None(float3 specularColor)
{
	return specularColor;
}

float Fresnel_Schlick(float u)
{
	float m = saturate(1.0f - u);
	float m2 = m * m;
	return m2 * m2 * m;
}

float3 Fresnel_Schlick(float3 specularColor, float3 h, float3 v)
{
	return (specularColor + (1.0f - specularColor) * pow((1.0f - saturate(dot(v, h))), 5.0f));
}

float3 Fresnel_CookTorrance(float3 specularColor, float3 h, float3 v)
{
	float3 n = (1.0f + sqrt(specularColor)) / (1.0f - sqrt(specularColor));
	float c = saturate(dot(v, h));
	float3 g = sqrt(n * n + c * c - 1.0f);

	float3 part1 = (g - c) / (g + c);
	float3 part2 = ((g + c) * c - 1.0f) / ((g - c) * c + 1.0f);

	return max(0.0f.xxx, 0.5f * part1 * part1 * (1.0f + part2 * part2));
}

float Specular_D(float a, float NdH)
{
#ifdef NDF_BLINNPHONG
	return NormalFistribution_BlinnPhong(a, NdH);
#else
#ifdef NDF_BECKMANN
	return NormalDistribution_Beckmann(a, NdH);
#else
#ifdef NDF_GGX
	return NormalDistribution_GGX(a, NdH);
#endif
#endif
#endif
}

float3 Specular_F(float3 specularColor, float3 h, float3 v)
{
#ifdef FRESNEL_NONE
	return Fresnel_None(specularColor);
#else
#ifdef FRESNEL_SCHLICK
	return Fresnel_Schlick(specularColor, h, v);
#else
#ifdef FRESNEL_COOKTORRANCE
	return Fresnel_CookTorrance(specularColor, h, v);
#endif
#endif
#endif
}

float3 Specular_F_Roughness(float3 specularColor, float a, float3 h, float3 v)
{
#ifdef FRESNEL_SCHLICK
	//Schlick using roughness to attenuate fresnel.
	return specularColor + (max(1.0f - a, specularColor) - specularColor) * pow((1.0f - saturate(dot(v, h))), 5.0f);
#else
#ifdef FRESNEL_NONE
	return Fresnel_None(specularColor);
#else
#ifdef FRESNEL_COOKTORRANCE
	return Fresnel_CookTorrance(specularColor, h, v);
#endif
#endif
#endif
}

float Specular_G(float a, float NdV, float NdL, float NdH, float VdH, float LdV)
{
#ifdef GEOMETRIC_IMPLICIT
	return Geometric_Implicit(a, NdV, NdL);
#else

#ifdef GEOMETRIC_NEUMANN
	return Geometric_Neumann(a, NdV, NdL);
#else

#ifdef GEOMETRIC_COOKTORRANCE
	return Geometric_CookTorrance(a, NdV, NdL, NdH, VdH);
#else

#ifdef GEOMETRIC_KELEMEN
	return Geometric_Kelemen(a, NdV, NdL, LdV);
#else

#ifdef GEOMETRIC_SMITH_BECKMANN
	return Geometric_Smith_Beckmann(a, NdV, NdL);
#else

#ifdef GEOMETRIC_SMITH_GGX
	return Geometric_Smith_GGX(a, NdV, NdL);
#else

#ifdef GEOMETRIC_SMITH_SCHLICK_GGX
	return Geometric_Smith_Schlick_GGX(a, NdV, NdL);
#endif
#endif
#endif
#endif
#endif
#endif
#endif
}

float3 Specular(float3 specularColor, float3 h, float3 v, float3 l, float a, float NdL, float NdV, float NdH, float VdH, float LdV) /// BIEN
{

	float3 TotalSpecular = Specular_D(a, NdH) *
		Specular_F(specularColor, v, h) *
		Specular_G(a, NdV, NdL, NdH, VdH, LdV);

	return TotalSpecular / (4.0f * NdV * NdL + 0.001f);
}

///es la aportación de color de tanto difuso como especular de cada luz
float3 ComputeLight(float3 albedoColor, float3 specularColor, float3 normal, float roughness, float3 lightPosition, float3 lightColor, float3 lightDir, float3 viewDir, float met)
{
	float NdL = saturate(dot(normal, lightDir));
	float NdV = saturate(dot(normal, viewDir));
	float3 h = normalize(lightDir + viewDir);
	float NdH = saturate(dot(normal, h));
	float VdH = saturate(dot(viewDir, h));
	float LdV = saturate(dot(lightDir, viewDir));
	float a = max(0.001f, roughness * roughness);

	float3 ColorDiffuse = Diffuse(albedoColor);
	float3 ColorSpecular = Specular(specularColor, h, viewDir, lightDir, a, NdL, NdV, NdH, VdH, LdV);

	float3 F = Fresnel_Schlick(specularColor, h, viewDir);
	float3 kS = F;
	float3 kD = 1.0f.xxx - kS;

	kD *= 1.0 - met;

	return lightColor * NdL * (kD * ColorDiffuse + ColorSpecular);
}

#ifdef DISNEY_BRDF

float3 DisneyBRDF(float3 baseColor, out float3 specularColor, float3 normal, float roughness, float3 lightDir, float3 viewDir, float3 X, float3 Y, out float3 diffuse, float3 lightColor)
{
	////Compute some useful values.
	//float NdL = saturate(dot(normal, lightDir));
	//float NdV = saturate(dot(normal, viewDir));
	//
	//float3 h = normalize(lightDir + viewDir);
	//float NdH = saturate(dot(normal, h));
	//float VdH = saturate(dot(viewDir, h));
	//float LdV = saturate(dot(lightDir, viewDir));
	//float LdH = saturate(dot(lightDir, h));
	//float a = max(0.001f, roughness * roughness);
	//
	//float luminance = 0.3f * baseColor.x + 0.6f * baseColor.y + 0.1f * baseColor.z;
	//
	//float3 tint = luminance > 0.0f ? baseColor / luminance : 1.0f.xxx; //normalize luminance to isolate hue + sat
	//specularColor = lerp(g_Specular * 0.08f * lerp(1.0f.xxx, tint, g_SpecularTint), baseColor, g_Metallic) * lightColor;
	//float3 CSheen = lerp(1.0f.xxx, tint, g_SheenTint); //el sheen es un entintado sobre la especularidad, por ejeplo, si pintas el metal, el metal sigue brillando. Es una ccapa de color 
	////sobre el metal
	//
	////Diffuse fresnel - go from 1 at normal incidence to .5 at grazing
	////and mix in diffuse retro-reflection based on roughness
	//float FL = Fresnel_Schlick(NdL);
	//float FV = Fresnel_Schlick(NdV);
	//float Fd90 = 0.5f + 2.0f * LdH * LdH * a;
	//float Fd = lerp(1.0f, Fd90, FL) * lerp(1.0f, Fd90, FV);
	//
	////Based on Hanrahan-Krueger brdf aproximation of isotropic bssrdf
	////1.25 scale is used to roughly presever albedo
	////Fss90 used to "flatten" retroreflection based on roughness
	//float Fss90 = LdH * LdH * a;
	//float Fss = lerp(1.0f, Fss90, FL) * lerp(1.0f, Fss90, FV);
	//float ss = 1.25f * (Fss * (1.0f / (NdL + NdV + 0.0001f) - 0.5f) + 0.5f);
	//
	////Specular
	//float aspect = sqrt(1.0f - g_Anisotropic * 0.9f);
	//float ax = max(0.001f, sqr(a) / aspect);
	//float ay = max(0.001f, sqr(a) * aspect);
	//float Ds = GTR2_aniso(NdH, dot(h, X), dot(h, Y), ax, ay);
	//float FH = Fresnel_Schlick(LdH);
	//float3 Fs = lerp(specularColor, 1.0f.xxx, FH);
	//float roughg = sqr(a * 0.5 + 0.5f);
	//float Gs = smithG_GGX(NdL, roughg) * smithG_GGX(NdV, roughg);
	//
	////Sheen
	//float3 Fsheen = FH * g_Sheen * CSheen;
	//
	////Clearcoat (ior= 1.5 > F0 = 0.04)
	//float Dr = GTR1(NdH, lerp(0.1f, 0.001f, g_ClearcoatGloss));
	//float Fr = lerp(0.04, 1.0f, FH);
	//float Gr = smithG_GGX(NdL, 0.25) * smithG_GGX(NdV, 0.25f);
	//diffuse = ((1.0f / PI) * lerp(Fd, ss, g_Subsurface) * baseColor * lightColor + Fsheen) * (1.0f - g_Metallic);
	//return (diffuse + Gs * Fs * Ds + 0.25f * g_Clearcoat * Gr * Fr * Dr) * NdL;
}
#endif

float2 OctWrap(float2 v)
{
	return (1.0 - abs(v.yx)) * (v.xy >= 0.0 ? 1.0 : -1.0);
}

float2 Encode(float3 n)
{
	n /= (abs(n.x) + abs(n.y) + abs(n.z));
	n.xy = n.z >= 0.0 ? n.xy : OctWrap(n.xy);
	n.xy = n.xy * 0.5 + 0.5;
	return n.xy;
}

float3 Decode(float2 encN)
{
	encN = encN * 2.0 - 1.0;

	float3 n;
	n.z = 1.0 - abs(encN.x) - abs(encN.y);
	n.xy = n.z >= 0.0 ? encN.xy : OctWrap(encN.xy);
	n = normalize(n);
	return n;
}

//, float3 _tangent, float3 _binormal
float3 PBR_rm(float3 _position, float3 _albedoColor, float3 _normal, float _roughness, float _metallic, float3 _specular)
{
	//Calculate the Specular Color from the metallic.
	float3 specularColor = _specular;

	//Calculate the viewDir.
	double3 viewDir = normalize(double3(ViewPosition.xyz) - double3(_position.xyz));
	float3 lightDir = float3(0, 1, 0);

	//////////////////////////////
	///ENVIROMENT FRESNEL OR F0///
	//////////////////////////////
	float3 envFresnel = Specular_F_Roughness(specularColor.xyz, _roughness * _roughness, _normal, viewDir).xyz;
	float3 Kd = 1.0f - envFresnel;
	Kd *= 1.0f - _metallic;

	///Bidirectional Reflection Distribution Function
	float3 Light1 = float3(0, 0, 0);

	for (int i = 0; i < g_LightNumber; ++i)
	{
		float Distance = length(_position - g_Lights[i].LightPosition);
		float Attenuation = 0.1f + 0.01f * Distance + 0.001f * Distance * Distance;
		float SpotFactor = 100.0f;
		float SpotLightDist = 1.0f;

		switch ((int)g_Lights[i].Props.x)
		{
		case 0: //PointLight
			lightDir = normalize(g_Lights[i].LightPosition - _position);
			break;
		case 2: //SpotLight
			lightDir = -normalize(g_Lights[i].LightDirection);

			float3 LightToPixel = normalize(_position - g_Lights[i].LightPosition);
			SpotFactor = dot(LightToPixel, g_Lights[i].LightDirection.xyz);

			SpotLightDist = (1.0 - (1.0 - SpotFactor) * 1.0 / (1.0 - g_Lights[i].Props.w));
			break;
		case 1: //Directional Light
			lightDir = normalize(-g_Lights[i].LightDirection);
			break;
		}

#ifndef DISNEY_BRDF
		//Light1 += DisneyBRDF(_albedoColor.xyz, specularColor.xyz, _normal, _roughness, lightDir, viewDir, _tangent, _binormal, diffuse, g_Lights[i].LightColor.xyz);
#else
		float3 spec = 0.0f.xxx;
		float3 diffuse = 0.0f.xxx;

		if (SpotFactor > g_Lights[i].Props.w && (Distance < g_Lights[i].Props.z || g_Lights[i].Props.x != 0))
		{
			Light1 += ComputeLight(_albedoColor.xyz, specularColor.xyz, _normal, _roughness, g_Lights[i].LightPosition.xyz, g_Lights[i].LightColor.xyz, lightDir, viewDir, _metallic);
		}
#endif

	}

	float mipIndex = _roughness * PBR_ENV_MIP_MAPS;
	double3 reflectVector = reflect(-viewDir, double3(_normal.xyz));

	float4 envVector = (float4(reflectVector.xyz, mipIndex));

	float3 Enviroment = EnviromentCube.SampleLevel(LinearWrapSampler, envVector.xyz, mipIndex).xyz;
	Enviroment = pow(Enviroment, 2.2f);

	float3 Irradiance = saturate(EnviromentCube.SampleLevel(LinearWrapSampler, _normal.xyz, 7.0f).xyz);
	Irradiance = pow(Irradiance, 2.2f);

	float2 envBRDF = BRDFLUT.SampleLevel(LinearClampSampler, float2(saturate(dot(_normal, viewDir)), clamp(_roughness, 0.05f, 0.95f)), 0.0f).xy;

	float3 FinalColor =
		(_albedoColor.xyz * Irradiance * Kd * g_AmbientLightIntensity) +
		(Enviroment * (envFresnel * envBRDF.xxx + envBRDF.yyy) * g_ReflectionIntensity) +
		(Light1 * (1.0f - _metallic) * g_LightIntensity);

	return FinalColor.xyz;
}


//, float3 _tangent, float3 _binormal
float3 PBR_rm_VXGI(float3 _position, float3 _albedoColor, float3 _normal, float _roughness, float _metallic, float3 _specular, float3 diffuseGI, float3 specularGI, float AOGI)
{
	//Calculate the Specular Color from the metallic.
	float3 specularColor = _specular;

	//Calculate the viewDir.
	double3 viewDir = normalize(double3(ViewPosition.xyz) - double3(_position.xyz));
	float3 lightDir = float3(0, 1, 0);

	//////////////////////////////
	///ENVIROMENT FRESNEL OR F0///
	//////////////////////////////
	float3 envFresnel = Specular_F_Roughness(specularColor.xyz, _roughness * _roughness, _normal, viewDir).xyz;
	float3 Kd = 1.0f - envFresnel;
	Kd *= 1.0f - _metallic;

	///Bidirectional Reflection Distribution Function
	float3 Light1 = float3(0, 0, 0);

	for (int i = 0; i < g_LightNumber; ++i)
	{
		float Distance = length(_position - g_Lights[i].LightPosition);
		float Attenuation = 0.1f + 0.01f * Distance + 0.001f * Distance * Distance;
		float SpotFactor = 100.0f;
		float SpotLightDist = 1.0f;

		switch ((int)g_Lights[i].Props.x)
		{
		case 0: //PointLight
			lightDir = normalize(g_Lights[i].LightPosition - _position);
			break;
		case 2: //SpotLight
			lightDir = -normalize(g_Lights[i].LightDirection);

			float3 LightToPixel = normalize(_position - g_Lights[i].LightPosition);
			SpotFactor = dot(LightToPixel, g_Lights[i].LightDirection.xyz);

			SpotLightDist = (1.0 - (1.0 - SpotFactor) * 1.0 / (1.0 - g_Lights[i].Props.w));
			break;
		case 1: //Directional Light
			lightDir = normalize(-g_Lights[i].LightDirection);
			break;
		}

#ifndef DISNEY_BRDF
		//Light1 += DisneyBRDF(_albedoColor.xyz, specularColor.xyz, _normal, _roughness, lightDir, viewDir, _tangent, _binormal, diffuse, g_Lights[i].LightColor.xyz);
#else
		float3 spec = 0.0f.xxx;
		float3 diffuse = 0.0f.xxx;

		if (SpotFactor > g_Lights[i].Props.w)
		{
			Light1 += ComputeLight(_albedoColor.xyz, specularColor.xyz, _normal, _roughness, g_Lights[i].LightPosition.xyz, g_Lights[i].LightColor.xyz, lightDir, viewDir, _metallic);
		}
#endif

	}

	float mipIndex = _roughness * PBR_ENV_MIP_MAPS;
	double3 reflectVector = reflect(-viewDir, double3(_normal.xyz));

	float4 envVector = (float4(reflectVector.xyz, mipIndex));

	float3 Enviroment = EnviromentCube.SampleLevel(LinearWrapSampler, envVector.xyz, mipIndex).xyz;
	Enviroment = pow(Enviroment, 2.2f);

	float3 Irradiance = saturate(EnviromentCube.SampleLevel(LinearWrapSampler, _normal.xyz, 7.0f).xyz);
	Irradiance = pow(Irradiance, 2.2f);

	float2 envBRDF = BRDFLUT.SampleLevel(LinearClampSampler, float2(saturate(dot(_normal, viewDir)), clamp(_roughness, 0.05f, 0.95f)), 0.0f).xy;

	float3 specResult = pow(specularGI, 1);
	float3 diffResult = pow(diffuseGI, 1);

	float3 FinalColor =
		(_albedoColor.xyz *  diffResult * Kd) +
		(specResult * (envFresnel * envBRDF.xxx + envBRDF.yyy)) +
		(Light1 * (1.0f - _metallic));
		
	return FinalColor.xyz;
}