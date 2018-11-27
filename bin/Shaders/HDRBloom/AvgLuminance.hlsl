#define M_PI 3.14159265359

Texture2D LuminanceTex : register(t0);
SamplerState LinealSamplerWrap : register(s0);
SamplerState AnisotropicSamplerWrap : register(s1);

SamplerState LinealSamplerClamp : register(s2);
SamplerState AnisotropicSamplerClamp : register(s3);
SamplerState PointWrapSampler : register(s4);

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

	float Luminance = 0; 
	
	int o = 0;
	for (float yy = 0.0f; yy < 1.0f; yy += 1.0f/7.0f)
	for (float i = 0; i < 1.0f; i += 1.0f/15.0f)
	{
		Luminance += LuminanceTex.SampleLevel(LinealSamplerWrap, float2(i, yy), 7.0f).x;
		o++;
	}

	Luminance /= o;
	//Luminance = LuminanceTex.SampleLevel(PointWrapSampler, Input.Texcoord, 6.0f).x;
    float lum = Luminance;
    //psout.AvgLuminance = 0.5f / (exp(lum) + 0.0001f);

	psout.AvgLuminance = 0.1f / ((lum) + 0.0001f);

    psout.AvgLuminance.a = 1.0f;

    return psout;
};