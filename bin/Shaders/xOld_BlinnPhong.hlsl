Texture2D AlbedoTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D PositionTexture : register(t2);

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

cbuffer cbLights : register(b3)
{
    float4 LightDiffuseColor;
    float4 LightAmbientColor;
    float4 LightPosition;
    float4 LightDirection;
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
	float2 UV : TEXCOORD0;
};

float4 CalculateLight(float4 albedoColor, float3 wpos, float3 normal, int lightType, float4 lightColor, float4 lightAmbientColor, float4 lightPos, float4 lightDir)
{
    float3 RealLightDir = 0;

    switch (lightType)
    {
    default:
    case 0: //POINT LIGHT
            RealLightDir = normalize(wpos - lightPos.xyz);
            break;
    case 1: //DIR LIGHT
            RealLightDir = normalize(lightDir.xyz);
            break;
    }

    float3 RealViewDir = normalize(wpos - ViewPosition.xyz);

    //Diffuse
    float Diffuse = saturate(dot(normal, -RealLightDir.xyz));
    float4 DiffuseColor = Diffuse * albedoColor * lightColor;

    //Specular
    float3 Half = normalize(RealLightDir.xyz + RealViewDir.xyz);
    float Specular = saturate(dot(-normal, Half));
    Specular = pow(Specular, 32);
    float4 SpecularColor = Specular * lightColor;
    
    return DiffuseColor + SpecularColor + lightAmbientColor*0.03f;

}

///Lightning Pass
float4 PS_Main(PS_Input vertex) : SV_TARGET
{
    float4 ColorFinal = float4(0,0,0,0);

    float2 UV = vertex.UV;
    UV.y *= -1;

    float4 NormalFinal = NormalTexture.Sample(Sampler_,UV);
    float4 PosFinal =    PositionTexture.Sample(Sampler_, UV);
    float4 Albedo =      AlbedoTexture.Sample(Sampler_,     UV);

    float4 Light = 0;

    if (Albedo.w != 0.0f)
    {
        Light += CalculateLight(Albedo, PosFinal.xyz, NormalFinal.xyz, 0, LightDiffuseColor, LightAmbientColor, LightPosition, LightDirection);
    }

    ColorFinal = Light;

    return ColorFinal;
}
