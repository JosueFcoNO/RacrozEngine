Texture2D AlbedoTexture : register(t0);
Texture2D NormalTexture : register(t1);

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

cbuffer cbMaterial : register(b4)
{
	float4 DiffuseColor : packoffset(c0);
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
	float4 pos : SV_POSITION;
	float2 tex0 : TEXCOORD0;
	float3 normal : NORMAL;
	float3x3 TBN : TEXCOORD1;
	float3 wpos : POSITION1;
};

struct PS_OUTPUT
{
    float4 Color : COLOR0;
    float4 Position : COLOR1;
    float4 Normal : COLOR2;
};

PS_Input VS_Main(VS_Input vertex)
{
	PS_Input vsOut = (PS_Input)0;
	vsOut.tex0 = vertex.tex0;

	//Convierto mi vertice
	matrix m = mul(worldMatrix, viewMatrix);
	m = mul(m, projMatrix);
	vsOut.pos = mul(float4(vertex.pos, 1), m);

	vsOut.normal = vertex.normal;

	vsOut.TBN[0].xyz = vertex.tangent;
	vsOut.TBN[1].xyz = vertex.binormal;
	vsOut.TBN[2].xyz = vertex.normal;

	vsOut.TBN = mul(vsOut.TBN, worldMatrix);

	vsOut.wpos = mul(float4(vertex.pos, 1), worldMatrix);

	return vsOut;
}

///Geometry Pass
PS_OUTPUT PS_Main(PS_Input frag) : SV_TARGET
{
    PS_OUTPUT output = (PS_OUTPUT) 0;

    ///Set the color to the first output color
    output.Color = AlbedoTexture.Sample(Sampler_, frag.tex0) * DiffuseColor;

    ///Set the normals to the second output color
	float3 NormalFinal = frag.normal;
    NormalFinal = NormalTexture.Sample(Sampler_, frag.tex0);
	NormalFinal = normalize(2.0f * NormalFinal - 1.0f);
	NormalFinal = normalize(mul(NormalFinal, frag.TBN));
    output.Normal = float4(NormalFinal,1);

    ///Set the world position to the third output color
    output.Position = float4(frag.wpos,1);

	return output;
}
