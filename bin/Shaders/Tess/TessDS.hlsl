Texture2D HeightTexture : register(t0);

sampler Sampler_ : register(s0);

cbuffer cbChangeEveryFrame : register(b2)
{
	matrix worldMatrix;
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

struct HS_OUTPUT
{
	float3 Position : COLOR1;
	float2 tex0 : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TEXCOORD1;
	float3 binormal : TEXCOORD2;
};


struct PS_Input
{
	float4 pos : SV_POSITION;
	float2 tex0 : TEXCOORD0;
	float3 normal : NORMAL;
	float3x3 TBN : TEXCOORD1;
	float3 wpos : POSITION1;
	float depth : TEXCOORD5;
	float4 prevPos : TEXCOORD4;
	float4 newPos  : TEXCOORD6;
};

struct ConstantOutputType
{
	float edges[3] : SV_TessFactor;
	float inside : SV_InsideTessFactor;
};

[domain("tri")]
PS_Input DS_Main(ConstantOutputType input, float3 uvwCoord : SV_DomainLocation, const OutputPatch<HS_OUTPUT, 3> patch)
{
	PS_Input output;

	// Determine the position of the new vertex.
	float3 vertexPosition = uvwCoord.x * patch[0].Position + uvwCoord.y * patch[1].Position + uvwCoord.z * patch[2].Position;
	float2 tex = uvwCoord.x * patch[0].tex0 + uvwCoord.y * patch[1].tex0 + uvwCoord.z * patch[2].tex0;
	float3 normal = uvwCoord.x * patch[0].normal + uvwCoord.y * patch[1].normal + uvwCoord.z * patch[2].normal;
	float3 tangent = uvwCoord.x * patch[0].tangent + uvwCoord.y * patch[1].tangent + uvwCoord.z * patch[2].tangent;
	float3 binormal = uvwCoord.x * patch[0].binormal + uvwCoord.y * patch[1].binormal + uvwCoord.z * patch[2].binormal;

	normal = normalize(normal);
	tangent = normalize(tangent);
	binormal = normalize(binormal);

	////Get the height value
	float height = HeightTexture.SampleLevel(Sampler_, tex, 0).r;

	////Offset the vertex position
	vertexPosition += normal * (height*2.0f - 1.0f) * 0.05f;

	matrix m = mul(worldMatrix, ViewMatrix);
	m = mul(m, ProjectionMatrix);
	output.pos = mul(float4(vertexPosition, 1), m);

	float3x3 TBN;
	TBN[0] = tangent;
	TBN[1] = binormal;
	TBN[2] = normal;

	output.TBN = mul(TBN, worldMatrix);
	output.normal = normal;

	output.wpos = mul(float4(vertexPosition, 1), worldMatrix);

	output.tex0 = tex;

	output.depth = output.pos.z / 10.0f;

	output.newPos = output.pos;
	output.prevPos = output.pos;

	return output;
}



