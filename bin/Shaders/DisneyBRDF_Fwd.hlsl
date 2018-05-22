#define PI 3.14159265359

#ifndef COMPILEFROMFILE
#define SPECULAR 0
#define METALLIC 0
#define NDF_GGX 1
#define FRESNEL_SCHLICK 1
#define GEOMETRIC_SMITH_SCHLICK_GGX 1
#define DISNEY_BRDF 1
#endif

#define g_Roughness g_PBRValues.x
#define g_Metallic g_PBRValues.y
#define g_OverrideAlbedo g_OverrideValues.x
#define g_OverrideNormal g_OverrideValues.y
#define g_OverrideMetallic g_OverrideValues.z
#define g_OverrideRoughness g_OverrideValues.w
#define g_OverrideSpecular g_UserSpecular.w

#define g_LightIntensity g_LightningParameters.x
#define g_AmbientLightIntensity g_LightningParameters.y
#define g_ReflectionIntensity g_LightningParameters.z

#ifdef DISNEY_BRDF
#define g_Subsurface g_DisneyBRDFParam1.x
#define g_Specular g_DisneyBRDFParam1.y
#define g_SpecularTint g_DisneyBRDFParam1.z
#define g_Anisotropic g_DisneyBRDFParam1.w
#define g_Sheen g_DisneyBRDFParam2.x
#define g_SheenTint g_DisneyBRDFParam2.y
#define g_Clearcoat g_DisneyBRDFParam2.z
#define g_ClearcoatGloss g_DisneyBRDFParam2.w
#endif

Texture2D AlbedoTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D MetallicTexture : register(t2);
Texture2D RoughnessTexture : register(t3);
Texture2D AOTexture : register(t4);

TextureCube EnviromentCube : register(t5);

SamplerState LinearWrapSampler : register(s0);
SamplerState AnisoWrapSampler : register(s1);
SamplerState LinearClampSampler : register(s0);
SamplerState AnisoClampSampler : register(s1);

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
    float4 DiffuseColor : packoffset(c0);
    float4 AmbientColor : packoffset(c1);
    float3 SpecularColor : packoffset(c2);
    float SpecularStrength : packoffset(c2.w);
    float4 g_DisneyBRDFParam1 : packoffset(c3);
    float4 g_DisneyBRDFParam2 : packoffset(c4);
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

cbuffer cbUserConfig : register(b6)
{
    float4 g_PBRValues;
    float4 g_UserAlbedo;
    float4 g_OverrideValues;
    float4 g_UserSpecular;
    float4 g_LightningParameters;
    int R;
    int G;
    int B;
    int A;
};

struct PS_Input
{
    float4 pos : SV_POSITION;
    float2 tex0 : TEXCOORD0;
    float3 normal : NORMAL;
    float3x3 TBN : TEXCOORD1;
    float3 wpos : POSITION1;
    float depth : TEXCOORD5;
};


struct PS_Output
{
    float4 PBR : SV_Target;
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
    return (1 / (PI * a * a)) * pow(NdH, 2 / (a * a) - 2.0f);
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
    return (2 * NdL * NdV) / (1 + LdV);
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
    float GV = NdV / (NdV * (1 - k) + k);
    float GL = NdL / (NdL * (1 - k) + k);

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
    return (specularColor + (1.0f - specularColor) * pow((1.0f - saturate(dot(v, h))), 5));
}

float3 Fresnel_CookTorrance(float3 specularColor, float3 h, float3 v)
{
    float3 n = (1.0f + sqrt(specularColor)) / (1.0f - sqrt(specularColor));
    float c = saturate(dot(v, h));
    float3 g = sqrt(n * n + c * c - 1.0f);

    float3 part1 = (g - c) / (g + c);
    float3 part2 = ((g + c) * c - 1.0f) / ((g - c) * c + 1.0f);

    return max(0.0f.xxx, 0.5f * part1 * part1 * (1 + part2 * part2));
}

float Specular_D(float a, float NdH)
{
#ifdef NDF_BLINNPHONG
		return NormalFistribution_BlinnPhong(a,NdH);
#else
#ifdef NDF_BECKMANN
		return NormalDistribution_Beckmann(a,NdH);
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
    return (specularColor + (max(1.0f - a, specularColor) - specularColor) * pow((1 - saturate(dot(v, h))), 5));
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
		return Geometric_Implicit(a,NdV, NdL);
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
	//Compute some useful values.
    float NdL = saturate(dot(normal, lightDir));
    float NdV = saturate(dot(normal, viewDir));

    float3 h = normalize(lightDir + viewDir);
    float NdH = saturate(dot(normal, h));
    float VdH = saturate(dot(viewDir, h));
    float LdV = saturate(dot(lightDir, viewDir));
    float LdH = saturate(dot(lightDir, h));
    float a = max(0.001f, roughness * roughness);

    float luminance = 0.3f * baseColor.x + 0.6f * baseColor.y + 0.1f * baseColor.z;

    float3 tint = luminance > 0.0f ? baseColor / luminance : 1.0f.xxx; //normalize luminance to isolate hue + sat
    specularColor = lerp(g_Specular * 0.08f * lerp(1.0f.xxx, tint, g_SpecularTint), baseColor, g_Metallic) * lightColor;
    float3 CSheen = lerp(1.0f.xxx, tint, g_SheenTint); //el sheen es un entintado sobre la especularidad, por ejeplo, si pintas el metal, el metal sigue brillando. Es una ccapa de color 
	//sobre el metal

	//Diffuse fresnel - go from 1 at normal incidence to .5 at grazing
	//and mix in diffuse retro-reflection based on roughness
    float FL = Fresnel_Schlick(NdL);
    float FV = Fresnel_Schlick(NdV);
    float Fd90 = 0.5f + 2.0f * LdH * LdH * a;
    float Fd = lerp(1.0f, Fd90, FL) * lerp(1.0f, Fd90, FV);

	//Based on Hanrahan-Krueger brdf aproximation of isotropic bssrdf
	//1.25 scale is used to roughly presever albedo
	//Fss90 used to "flatten" retroreflection based on roughness
    float Fss90 = LdH * LdH * a;
    float Fss = lerp(1.0f, Fss90, FL) * lerp(1.0f, Fss90, FV);
    float ss = 1.25f * (Fss * (1.0f / (NdL + NdV + 0.0001f) - 0.5f) + 0.5f);

	//Specular
    float aspect = sqrt(1.0f - g_Anisotropic * 0.9f);
    float ax = max(0.001f, sqr(a) / aspect);
    float ay = max(0.001f, sqr(a) * aspect);
    float Ds = GTR2_aniso(NdH, dot(h, X), dot(h, Y), ax, ay);
    float FH = Fresnel_Schlick(LdH);
    float3 Fs = lerp(specularColor, 1.0f.xxx, FH);
    float roughg = sqr(a * 0.5 + 0.5f);
    float Gs = smithG_GGX(NdL, roughg) * smithG_GGX(NdV, roughg);

	//Sheen
    float3 Fsheen = FH * g_Sheen * CSheen;

	//Clearcoat (ior= 1.5 > F0 = 0.04)
    float Dr = GTR1(NdH, lerp(0.1f, 0.001f, g_ClearcoatGloss));
    float Fr = lerp(0.04, 1.0f, FH);
    float Gr = smithG_GGX(NdL, 0.25) * smithG_GGX(NdV, 0.25f);
    diffuse = ((1.0f / PI) * lerp(Fd, ss, g_Subsurface) * baseColor * lightColor + Fsheen) * (1.0f - g_Metallic);
    return (diffuse + Gs * Fs * Ds + 0.25f * g_Clearcoat * Gr * Fr * Dr) * NdL;
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

///Lightning Pass
PS_Output PS_Main(PS_Input Input)
{
    PS_Output psout = (PS_Output) 0;

    //Sample the albedoColor from the texture
    float4 albedoColor = AlbedoTexture.Sample(LinearWrapSampler, Input.tex0) * (1.0f - g_OverrideAlbedo);

    //Sample the original normal value from the texture
    float3 normal = NormalTexture.Sample(LinearWrapSampler, Input.tex0).xyz;
    normal = normalize(2.0f * normal - 1.0f);
    normal = normalize(mul(normal, Input.TBN));

    albedoColor = pow(albedoColor, 2.2f);

    float metallic = MetallicTexture.Sample(LinearWrapSampler, Input.tex0).x * (1.0f - g_OverrideMetallic);
    float roughness = RoughnessTexture.Sample(LinearWrapSampler, Input.tex0).x * (1.0f - g_OverrideRoughness);
    
    roughness += g_Roughness * g_OverrideRoughness;

#if defined(METALLIC) || defined (DISNEY_BRDF)
    metallic += g_Metallic * g_OverrideMetallic;
#endif

    albedoColor += g_UserAlbedo * g_OverrideAlbedo;
    normal += normalize(normal) * g_OverrideNormal;

#ifdef USE_GLOSSINESS
		roughness = 1.0f - roughness;
#endif

   //Iluminación
    float4 pos = float4(Input.wpos.xyz, 1);
    double3 viewDir = normalize(double3(ViewPosition.xyz) - double3(pos.xyz));
    float3 lightDir = float3(0, 1, 0);

   //Metallic
   //Que diferencia tiene el albedo con el realAlbedo, si esto es parte
   //de las propiedades, que colores tienen elos metales.
#ifdef METALLIC
    float3 realSpecularColor = lerp(0.04f, albedoColor, metallic);
    float3 realAlbedo = albedoColor.xyz - albedoColor.xyz * metallic;

#elif SPECULAR
	float3 realAlbedo = albedoColor + g_UserAlbedo * g_OverrideAlbedo;
	float3 realSpecularColor = specularColor + g_UserSpecular.xyz * g_OverrideSpecular;
#endif

    //////////////////////////////
    ///ENVIROMENT FRESNEL OR F0///
    //////////////////////////////
    float3 envFresnel = Specular_F_Roughness(realSpecularColor.xyz, roughness, normal.xyz, viewDir).xyz;
    float3 Kd = 1.0f - envFresnel;

   ///Bidirectional Reflection Distribution Function
    float3 Light1 = float3(0, 0, 0);

    for (int i = 0; i < g_LightNumber; ++i)
    {
        float Distance = length(pos - g_Lights[i].LightPosition);
        float Attenuation = 0.1f + 0.01f * Distance + 0.001f * Distance * Distance;
        float SpotFactor = 100.0f;
        float SpotLightDist = 1.0f;

        switch ((int) g_Lights[i].Props.x)
        {
            case 0: //PointLight
                lightDir = normalize(g_Lights[i].LightPosition - pos);
                break;
            case 2: //SpotLight
                lightDir = -normalize(g_Lights[i].LightDirection);

                float3 LightToPixel = normalize(pos - g_Lights[i].LightPosition);
                SpotFactor = dot(LightToPixel, g_Lights[i].LightDirection.xyz);

                SpotLightDist = (1.0 - (1.0 - SpotFactor) * 1.0 / (1.0 - g_Lights[i].Props.w));
                break;
            case 1: //Directional Light
                lightDir = normalize(-g_Lights[i].LightDirection);
                break;
        }

        float3 z = float3(0, 0, 0);
        
        float3 diffuse;

#ifndef DISNEY_BRDF
            Light1 += ComputeLight(albedoColor.xyz, realSpecularColor, normal, roughness, g_Lights[i].LightPosition.xyz, g_Lights[i].LightColor.xyz, lightDir, viewDir, metallic);
#else
        float3 spec = 0.0f.xxx;
        diffuse = 0.0f.xxx;

        if (SpotFactor > g_Lights[i].Props.w)
        {
           // Light1 += (DisneyBRDF(albedoColor.xyz, spec, normal, roughness, lightDir, viewDir, Tangents.xyz, Binormals.xyz, diffuse, g_Lights[i].LightColor.xyz));
            Light1 += ComputeLight(realAlbedo.xyz, realSpecularColor, normal, roughness, g_Lights[i].LightPosition.xyz, g_Lights[i].LightColor.xyz, lightDir, viewDir, metallic);

        }
#endif

    }

    //Le quite el cuadrado al roughness
    float mipIndex = roughness * roughness * 4.0f;
    double3 reflectVector = reflect(-viewDir, double3(normal.xyz));

    float4 envVector = float4(reflectVector.xyz, mipIndex);

    float3 Enviroment = EnviromentCube.SampleLevel(LinearWrapSampler, envVector.xyz, mipIndex + 0.0f).xyz;
    Enviroment = pow(Enviroment, 2.2f);

    float3 Irradiance = EnviromentCube.SampleLevel(LinearWrapSampler, envVector.xyz, 8.0f).xyz;

    //float2 envBRDF = BRDFLUT.Sample(LinearSampler, float2(roughness, dot(normal, viewDir))).xy;

    float4 FinalColor = float4((albedoColor.xyz * Irradiance * g_AmbientLightIntensity) +
                                ((Enviroment) * (envFresnel) * g_ReflectionIntensity) +
                                (Light1 * g_LightIntensity), 1);

    //float SSAO = 1.0f - SSAOTex.Sample(LinearSampler, Input.Texcoord).r;
    //SSAO = pow(SSAO, 2.2f);

    FinalColor *= pow(albedoColor.w, 2.2f);
    //FinalColor *= ShadowMapTex.Sample(LinearSampler, Input.Texcoord).r;


    //FinalColor = pow(FinalColor, 1.0f / 2.2f);

    psout.PBR = FinalColor;

    return psout;
}

