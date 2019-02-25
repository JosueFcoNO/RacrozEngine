#define M_PI 3.14159265359

Texture2D BrightOriginal : register(t0);
Texture2D AddBright: register(t1);

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
    float4 BrightAdd : SV_Target;
};

///Lightning Pass
PS_Output PS_Main(PS_Input Input)
{
    PS_Output psout = (PS_Output) 0;

    float4 fBrightAdd = AddBright.SampleLevel(Sampler_, Input.Texcoord, 0);
    float4 fBrightOriginal = BrightOriginal.Sample(Sampler_, Input.Texcoord);

    psout.BrightAdd = (fBrightAdd + fBrightOriginal)*.5f;

    return psout;
};
