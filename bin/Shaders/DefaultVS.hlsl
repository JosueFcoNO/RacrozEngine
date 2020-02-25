struct VS_Input
{
	float3 pos : POSITION;
};

struct PS_Input
{
	float4 pos : SV_POSITION;
};

PS_Input VS_Main(VS_Input vertex)
{
	PS_Input vsOut = (PS_Input)0;
	return vsOut;
}

