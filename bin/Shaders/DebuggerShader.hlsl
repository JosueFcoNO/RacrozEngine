
cbuffer cbColor : register(b0)
{
	float4 Color;
};

cbuffer cbCamera : register(b5)
{
    float4 ViewPosition;
    float4 ViewDirection;
    float4 NearPlane;
    float4 FarPlane;
    matrix ViewMatrix;
    matrix ProjectionMatrix;
	matrix PreviousViewMatrix;
	matrix PreviousProjectionMatrix;
};

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

	//Convierto mi vertice
    matrix m = mul(ViewMatrix, ProjectionMatrix);
	vsOut.pos = mul(float4(vertex.pos, 1), m);

	return vsOut;
}

float4 PS_Main(PS_Input frag) : SV_TARGET
{
	return float4(Color.xyz, 0.5f);
}