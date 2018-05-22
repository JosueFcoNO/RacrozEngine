Texture2D DiffuseTexture : register(t0);

SamplerState colorSampler_ : register(s0);

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
	vsOut.tex0 = vertex.tex0;

	return vsOut;
}

float4 PS_Main(PS_Input frag) : SV_TARGET
{
    float2 UV = frag.tex0;
    //UV.y *= -1;
    UV.x = 1.0f-UV.x;

    if (R + G + B == 0 && A == 1)
    {
        return DiffuseTexture.Sample(colorSampler_, UV).aaaa;
    }

    float4 color = DiffuseTexture.Sample(colorSampler_, UV);

    return float4(color.r * R, color.g * G, color.b * B, color.a);
}
