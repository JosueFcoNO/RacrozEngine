#define M_PI 3.14159265359

Texture2D NormalTexture : register(t0);
Texture2D BlurTexture : register(t1);
Texture2D DepthTexture : register(t2);

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
    float4 DOF : SV_Target;
};

PS_Output PS_Main(PS_Input Input)
{
    PS_Output psout = (PS_Output) 0;

    float Range = 0.8f;
    float focus = 0.4f;

    float4 Blur = BlurTexture.Sample(Sampler_, Input.Texcoord);
    float4 TexNormal = NormalTexture.Sample(Sampler_, Input.Texcoord);
    float depth = DepthTexture.Sample(Sampler_, Input.Texcoord).w;

    psout.DOF = lerp(TexNormal, Blur, saturate(Range * abs(focus - depth)));
    psout.DOF.w = saturate(Range * abs(focus - depth));
    return psout;
};

