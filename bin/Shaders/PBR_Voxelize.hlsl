/// TEXTURES

Texture2D AlbedoTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D MetallicTexture : register(t2);
Texture2D RoughnessTexture : register(t3);
Texture2D AOTexture : register(t4);
TextureCube EnviromentCube : register(t12);
Texture2D OpacityTexture : register(t6);
Texture2D BRDFLUT : register(t7);

///CONSTANT BUFFERS

cbuffer cbChangesSometimes : register(b0)
{
	matrix viewMatrix;
};

cbuffer cbChangeOnResize : register(b1)
{
	matrix projMatrix;
};

cbuffer cbChangeEveryFrame : register(b2)
{
	matrix worldMatrix;
};

struct Light
{
	float4 LightColor;
	float4 LightPosition;
	float4 LightDirection;
	float4 Props;
};

cbuffer cbLights : register(b3)
{
	int g_LightNumber;
	int3 paddding;
	Light g_Lights[8];
};

cbuffer cbMaterial : register(b4)
{
	float3 g_Albedo;
	float OverrideAlbedo;

	float3 g_Specular;
	float OverrideMetallicSpecular;

	float3 g_Emmisive;
	float OverriveEmmisive;

	float g_RoughGloss;
	float g_Metallic;
	float g_Opacity;
	float g_TesselationFactor;

	float OverrideOpacity;
	float AOStrength;
	float tileX;
	float tileY;

	float OverrideNormal;
	float OverrideRoughGloss;
	float g_SpecularTint;
	float g_SheenTint;

	float g_Anisotropic;
	float g_Sheen;
	float g_ClearcoatGloss;
	float g_Subsurface;

	float g_Clearcoat;
	float padding[3];
};

cbuffer cbCamera : register(b5)
{
	float4 ViewPosition;
	float4 ViewDirection;
	float4 NearPlane;
	float4 FarPlane;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
};

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

///SAMPLER STATES

SamplerState LinearWrapSampler : register(s0);
SamplerState AnisoWrapSampler : register(s1);
SamplerState LinearClampSampler : register(s2);
SamplerState AnisoClampSampler : register(s3);

///INPUTS

struct PS_Input
{
	float4 pos : SV_Position;
	float2 tex0 : TEXCOORD;
	float3 normal : NORMAL;
	float3x3 TBN : TEXCOORD1;
	float3 wpos : WSPOSITION;
	float depth : TEXCOORD5;
	float4 prevPos : TEXCOORD4;
	float4 newPos  : TEXCOORD6;
	VxgiVoxelizationPSInputData vxgiData;
};

static const float PI = 3.14159265;

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

	return lightColor * NdL * (ColorDiffuse);
}

//, float3 _tangent, float3 _binormal
float3 PBR_rm(float3 _position, float3 _albedoColor, float3 _normal, float _roughness, float _metallic, float3 _specular)
{
	//Calculate the Specular Color from the metallic.
	float3 specularColor = _specular;

	//Calculate the viewDir.
	double3 viewDir = normalize(double3(ViewPosition.xyz) - double3(_position.xyz));
	float3 lightDir = float3(0, 1, 0);

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
			Light1 += ComputeLight(_albedoColor.xyz, specularColor.xyz, _normal, _roughness, g_Lights[i].LightPosition.xyz, g_Lights[i].LightColor.xyz, lightDir, viewDir, _metallic) * 10.0f;
		}
#endif

	}

	float3 Irradiance = saturate(EnviromentCube.SampleLevel(LinearWrapSampler, _normal.xyz, 7.0f).xyz);
	Irradiance = pow(Irradiance, 2.2f);

	float3 FinalColor = (Light1 * (1.0f - _metallic) * g_LightIntensity) + _albedoColor.xyz * Irradiance;

	return FinalColor.xyz;
}

///Lightning Pass
void PS_Main(PS_Input Input)
{
	float2 tex = Input.tex0;

	//Sample the albedoColor from the texture
	float4 albedoColor = AlbedoTexture.Sample(LinearWrapSampler, tex) * (1.0f - OverrideAlbedo);
	float Opacity = 1.0f;

	//if (Opacity == 1.0f)
	//{
	//	Opacity = OpacityTexture.Sample(LinearWrapSampler, tex);
	//}
	//
	//if (Opacity == 1.0f)
	//{
	//	Opacity = g_Opacity;
	//}

	//Sample the original normal value from the texture
	float3 normal = NormalTexture.Sample(LinearWrapSampler, tex).xyz;
	normal = normalize(2.0f * normal - 1.0f);
	normal = normalize(mul(normal, Input.TBN));

	albedoColor = pow(albedoColor, 2.2f);

	float metallic = MetallicTexture.Sample(LinearWrapSampler, tex).x * (1.0f - OverrideMetallicSpecular);
	float roughness = RoughnessTexture.Sample(LinearWrapSampler, tex).x * (1.0f - OverrideRoughGloss);

	roughness += g_RoughGloss * OverrideRoughGloss;

#if defined(METALLIC) || defined (DISNEY_BRDF)
	metallic += g_Metallic * OverrideMetallicSpecular;
#endif

	albedoColor.xyz += g_Albedo * OverrideAlbedo;
	//normal += normalize(normal) * OverrideNormal;

#ifdef USE_GLOSSINESS
	roughness = 1.0f - roughness;
#endif

	float3 radiosity = 0;

	radiosity += PBR_rm(Input.wpos.xyz, albedoColor, normal, roughness, metallic, lerp(0.04f, albedoColor, metallic));

	radiosity += albedoColor.rgb * VxgiGetIndirectIrradiance(Input.wpos.xyz, normal) / PI;

	VxgiStoreVoxelizationData(Input.vxgiData, normal, 1, radiosity, 0);
}

