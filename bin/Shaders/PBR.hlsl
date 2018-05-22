#define M_PI 3.14159265359

Texture2D AlbedoTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D PositionTexture : register(t2);
Texture2D MRBLTexture : register(t3);

TextureCube EnviromentCube : register(t4);
TextureCube IrradianceCube : register(t5);
Texture2D SSAOTex : register(t6);

sampler Sampler_ : register(s0);

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

struct PS_Input
{
    float4 pos : SV_POSITION;
    float2 Texcoord : TEXCOORD0;
};

struct PS_Output
{
    float4 PBR : SV_Target;
};

float3 Diffuse(float3 pAlbedo) /// BIEN
{
    return pAlbedo / M_PI;
}

float3 Fresnel_Schlick(float3 f0, float3 v, float3 h)  /// BIEN
{
    return f0 + (1.0f - f0) * pow((1.0f - saturate(dot(v, h))), 5);
}

///Es el efecto de fresnel
float3 Specular_F(float3 f0, float3 v, float3 h) /// BIEN
{
    return Fresnel_Schlick(f0, v, h);
}

///Vamos a limitar el reflejo al reflejo que podemos ver.
float3 Specular_F_Roughness(float3 f0, float a, float3 h, float3 v) ///BIEN
{
   ///Lo hacemos un factor entre el roughness y el specular color
    return f0 + (max(1.0f - a, f0) - f0) * pow((1.0f - saturate(dot(v, h))), 5);
}

//GGX Isotropico
float NormalDistribution_GGX(float a, float NdH) /// BIEN
{

    float a2 = a * a;
    float NdH2 = NdH * NdH;
   
   //Distribuir sobre la cosa normal y proyectarla como circulo
    float denominador = NdH2 * (a2 - 1.0f) + 1.0f;
    denominador *= denominador;
    denominador *= M_PI;
 
    return a2 / denominador;
}

///Está es la geometria distribuida.
float3 Specular_D(float a, float NdH) /// BIEN
{
    return NormalDistribution_GGX(a, NdH);
}

//
float Geometric_Smith_Schlick_GGX(float a, float NdV, float NdL) /// BIEN
{
   //GGX Smith Schlick
   ///Necesitmoas la mitad para el eje de la vista y la mitad para el eje de la luz
    float k = a * 0.5f;
    float GV = NdV / (NdV * (1 - k) + k);
    float GL = NdL / (NdL * (1 - k) + k);
   
    return GV * GL;
}

///Esto es el sombreado geometrico
float3 Specular_G(float a, float NdV, float NdL, float NdH, float VdH, float LdV) /// BIEN
{
    return Geometric_Smith_Schlick_GGX(a, NdV, NdL);
}

float3 Specular(float3 specularColor, float3 h, float3 v, float3 l, float a, float NdL, float NdV, float NdH, float VdH, float LdV) /// BIEN
{

    float3 TotalSpecular = Specular_D(a, NdH) *
                       Specular_F(specularColor, v, h) *
                       Specular_G(a, NdV, NdL, NdH, VdH, LdV);

    return TotalSpecular / (4.0f * NdV * NdL + 0.001f);
}

///es la aportación de color de tanto difuso como especular de cada luz
float3 ComputeLight(float3 albedoColor, float3 specularColor, float3 normal, float roughness, float3 lightPosition, float3 lightColor, float3 lightDir, float3 viewDir)
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


    return lightColor * NdL * (ColorDiffuse * (1.0f - ColorSpecular) + ColorSpecular);
}

///Lightning Pass
PS_Output PS_Main(PS_Input Input)
{
    PS_Output psout = (PS_Output) 0;

    float4 albedoColor = AlbedoTexture.Sample(Sampler_, Input.Texcoord);
   
    float4 normalO = NormalTexture.Sample(Sampler_, Input.Texcoord);
    float3 normal = normalO.xyz;
    float4 MRLB = MRBLTexture.Sample(Sampler_, Input.Texcoord);

    if (normalO.w == 0.0f)
    {
        psout.PBR = albedoColor;
        return psout;
    }
   
    float metallic = MRLB.x;
    float roughness = MRLB.y;
   //Lo primero es hacer una correción Gamma, a que?
   //A nuestro albedo que viene en sRGB lo convertimos a espacio lineal
    albedoColor = pow(albedoColor, 2.2f);
    
   //Iluminación
    float4 pos = PositionTexture.Sample(Sampler_, Input.Texcoord);
    float3 viewDir = normalize(ViewPosition - pos);
    float3 lightDir = float3(0, 1, 0);

   
   //Metallic
   //Que diferencia tiene el albedo con el realAlbedo, si esto es parte
   //de las propiedades, que colores tienen elos metales.
    float3 realAlbedo = albedoColor - (albedoColor * metallic);

   //Ahora sacamos el realSpecularColor
    float3 realSpecularColor = lerp(0.03f, albedoColor, metallic);
   
   ///Bidirectional Reflection Distribution Function
    float3 Light1 = float3(0, 0, 0);

    for (int i = 0; i < g_LightNumber; ++i)
    {
        float Distance = length(pos - g_Lights[i].LightPosition);
        float Attenuation = AttnConst + AttnLinear * Distance + AttnExp * Distance * Distance;
        float SpotFactor = 10.0f;
        float SpotLightDist = 1.0f;

        switch (g_Lights[0].Props.x)
        {
            case 0: //PointLight
                lightDir = normalize(g_Lights[i].LightPosition - pos);
                 break;

            case 2: //SpotLight
                lightDir = normalize(g_Lights[i].LightDirection); 

                float3 LightToPixel = normalize(pos - g_Lights[i].LightPosition);
                SpotFactor = dot(LightToPixel, g_Lights[i].LightDirection.xyz);

                SpotLightDist = (1.0 - (1.0 - SpotFactor) * 1.0 / (1.0 - g_Lights[i].Props.w));
                break;

            case 1: //Directional Light
                lightDir = normalize(-g_Lights[i].LightDirection);
                break;
        }

        if (SpotFactor > g_Lights[i].Props.w)
        {
            Light1 += (ComputeLight(realAlbedo, realSpecularColor, normal, roughness, g_Lights[i].LightPosition.xyz, g_Lights[i].LightColor.xyz, lightDir, viewDir) / Attenuation) * SpotLightDist;
        }
    }

    //Le quite el cuadrado al roughness
    float mipIndex = roughness * roughness * 8;
    float3 reflectVector = reflect(normal, -viewDir);

    float4 envVector = float4(reflectVector.xyz, mipIndex);
   
    float3 Enviroment = EnviromentCube.SampleLevel(Sampler_, envVector.xyz, mipIndex).xyz;
    Enviroment = pow(Enviroment, 2.2f);

    float3 envFresnel = Specular_F_Roughness(realSpecularColor, roughness * roughness, normal, viewDir).xyz;

    float3 Irradiance = IrradianceCube.SampleLevel(Sampler_, envVector.xyz, 0).xyz;

    float4 FinalColor = float4((Light1 * 1.0f) * 1.0f + (Enviroment * envFresnel) * (1.0f - roughness) + (realAlbedo * Irradiance) * 1.00f, 1);

    float4 SSAO = 1.0f - SSAOTex.Sample(Sampler_, Input.Texcoord);

    FinalColor *= SSAO;

    FinalColor = pow(FinalColor, 1.0f / 2.2f);
   

    //FinalColor = reflectVector.xyzx;
    psout.PBR = FinalColor;

    return psout;
}

