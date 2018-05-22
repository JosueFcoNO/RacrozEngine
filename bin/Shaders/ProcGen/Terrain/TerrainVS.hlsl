Texture2D AlbedoTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D MetallicTexture : register(t2);
Texture2D RoughnessTexture : register(t3);
Texture2D AOTexture : register(t4);
Texture2D EMTexture : register(t10);
Texture2D HeightMap : register(t11);

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

struct Light
{
    float4 LightColor;
    float4 LightPosition;
    float4 LightDirection;
    float4 Props;
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

cbuffer TerrainBuffer : register(b9)
{
    int minHeight;
    int maxHeight;
    int Size;
    int padding2;
}

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
	float4 pos : SV_POSITION;
	float2 tex0 : TEXCOORD0;
	float3 normal : NORMAL;
	float3x3 TBN : TEXCOORD1;
	float3 wpos : POSITION1;
    float depth : TEXCOORD5;
};

PS_Input VS_Main(VS_Input vertex)
{
	PS_Input vsOut = (PS_Input)0;
	vsOut.wpos = vertex.tex0/100;
    
    float3 hm = HeightMap.SampleLevel(Sampler_, vertex.tex0/Size - (1/Size), 0.0f);

    float3 newPos = vertex.pos;
    newPos.y = (maxHeight-minHeight);
    newPos.y *= hm.x;
    newPos.y += minHeight;

	//Convierto mi vertice
    matrix m = mul(worldMatrix, ViewMatrix);
    m = mul(m, ProjectionMatrix);
    vsOut.pos = mul(float4(newPos, 1), m);

    vsOut.normal = mul(vertex.normal, worldMatrix);

	vsOut.TBN[0].xyz = vertex.tangent;
	vsOut.TBN[1].xyz = vertex.binormal;
	vsOut.TBN[2].xyz = vertex.normal;

	vsOut.TBN = mul(vsOut.TBN, worldMatrix);

    vsOut.wpos = mul(float4(newPos, 1), worldMatrix);

    vsOut.depth = vsOut.pos.z / 10.0f;
	return vsOut;
}
