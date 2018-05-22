#define M_PI 3.14159265359

Texture2D LuminanceTex : register(t0);

sampler Sampler_ : register(s0);

cbuffer cbMiddleGrey : register(b0)
{
    float middleGrey;
    float3 padding;
};

struct PS_Input
{
    float4 pos : SV_POSITION;
    float2 Texcoord : TEXCOORD0;
};

struct PS_Output
{
    float4 AvgLuminance : SV_Target;
};

///Lightning Pass
PS_Output PS_Main(PS_Input Input)
{
    PS_Output psout = (PS_Output) 0;

    float4 Luminance = LuminanceTex.SampleLevel(Sampler_, Input.Texcoord, 12.0f);

    float lum = Luminance.x;
    psout.AvgLuminance = 0.5f / (exp(lum) - 0.0001f);
    psout.AvgLuminance.a = 1.0f;

    return psout;
};