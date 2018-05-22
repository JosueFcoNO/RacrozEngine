#define M_PI 3.14159265359

Texture2D NormalTexture : register(t0);
Texture2D DepthTexture : register(t1);

sampler Sampler_ : register(s0);

cbuffer cbFog : register(b0)
{
    float fogAmount;
    float ActiveDepth;
    float2 padding;
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
    float4 LightColor;
    float4 LightPosition;
    float4 LightDirection;
    float4 Props;
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
    float4 Fog : SV_Target;
};

PS_Output PS_Main(PS_Input Input)
{
    float4 Color = float4(0.0f, 0.25f, 0.5f, 1.0f);

    PS_Output psout = (PS_Output) 0;

    float4 TexNormal = NormalTexture.Sample(Sampler_, Input.Texcoord);
    float depth = DepthTexture.Sample(Sampler_, Input.Texcoord).w;

    psout.Fog = lerp(TexNormal, Color, saturate(depth / 40.0f));
    return psout;
};

