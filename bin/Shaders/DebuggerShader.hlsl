
cbuffer cbColor : register(b0)
{
	float4 Color;
};

struct PS_Input
{
	float4 pos : SV_POSITION;
	float2 tex0 : TEXCOORD0;
	float3 normal : NORMAL;
};

float4 PS_Main(PS_Input frag) : SV_TARGET
{
	return Color;
}