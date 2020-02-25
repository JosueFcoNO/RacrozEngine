cbuffer cbChangeEveryFrame : register(b2)
{
    matrix worldMatrix;
	matrix previousWorldMatrix;
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
	float2 tex0 : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

struct PS_Input
{
	float4 position : SV_Position;
	float2 texCoord : TEXCOORD;
	float3 normal   : NORMAL;
	float3 tangent  : TANGENT;
	float3 binormal : BINORMAL;
	float3 positionWS : WSPOSITION;
};

PS_Input VS_Main(VS_Input vertex)
{
	PS_Input vsOut = (PS_Input)0;
	vsOut.tex0 = vertex.tex0;

	//Convierto mi vertice
    matrix m = mul(worldMatrix, ViewMatrix);
    m = mul(m, ProjectionMatrix);
	vsOut.pos = mul(float4(vertex.pos, 1), m);

	//Convierto mi vertice a la posición previa.
	matrix mPrev = mul(previousWorldMatrix, PreviousViewMatrix);
	mPrev = mul(mPrev, PreviousProjectionMatrix);
	vsOut.prevPos = mul(float4(vertex.pos, 1), mPrev);
	
	vsOut.newPos = vsOut.pos;

    vsOut.normal = mul(float4(vertex.normal, 1.0f), worldMatrix);

	vsOut.TBN[0].xyz = vertex.tangent;
	vsOut.TBN[1].xyz = vertex.binormal;
	vsOut.TBN[2].xyz = vertex.normal;

	vsOut.TBN = mul(vsOut.TBN, worldMatrix);

	vsOut.wpos = mul(float4(vertex.pos,1), worldMatrix);

    vsOut.depth = vsOut.pos.z / 10.0f;

	return vsOut;
}