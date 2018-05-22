Texture2D DiffuseTexture : register(t0);

SamplerState colorSampler_ : register(s0);

cbuffer cbUI : register(b0)
{
    float2 ScreenSize;
    float2 padding;
};

struct VS_Input
{
	float3 pos : POSITION;
	float2 tex0 : TEXCOORD0;
};

struct PS_Input
{
	float4 pos : SV_POSITION;
	float2 tex0 : TEXCOORD0;
};

PS_Input VS_Main(VS_Input vertex)
{
	PS_Input vsOut = (PS_Input)0;
    vsOut.pos = float4(vertex.pos.xyz, 1);

    vsOut.pos.y = (ScreenSize.y - vsOut.pos.y);

    vsOut.pos.xy *= 2.0f;
    vsOut.pos.xy -= ScreenSize;
    vsOut.pos.x /= ScreenSize.x;
    vsOut.pos.y /= ScreenSize.y;
    vsOut.pos.z /= 100.f;
    vsOut.pos.w = 1.0f;
	
    vsOut.tex0 = vertex.tex0;
    vsOut.tex0.y = 1.0f - vsOut.tex0.y;

	return vsOut;
}

float4 PS_Main(PS_Input frag) : SV_TARGET
{
    float2 UV = frag.tex0;

    return DiffuseTexture.Sample(colorSampler_, UV);
}
