sampler Sampler_ : register(s0);

cbuffer TessellationBuffer : register(b0)
{
    float tessellationAmount;
    float3 padding;
};

cbuffer world : register(b1)
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

struct HS_INPUT
{
    float3 pos : POSITION;
    float2 tex0 : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TEXCOORD1;
    float3 binormal : TEXCOORD2;
};

struct HS_OUTPUT
{
	float3 Position : COLOR1;
	float2 tex0 : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TEXCOORD1;
	float3 binormal : TEXCOORD2;
};

struct ConstantOutputType
{
    float edges[3] : SV_TessFactor;
    float inside : SV_InsideTessFactor;
};

////////////////////////////////////////////////////////////////////////////////
// Patch Constant Function
////////////////////////////////////////////////////////////////////////////////
ConstantOutputType ColorPatchConstantFunction(InputPatch<HS_INPUT, 3> inputPatch, uint patchId : SV_PrimitiveID)
{
	ConstantOutputType output;

	//float3 one = (inputPatch[0].pos - inputPatch[1].pos);
	//float magone = length(one)/2.0f;
	//one = inputPatch[1].pos+normalize(one)*magone;

	//float3 two = (inputPatch[1].pos - inputPatch[2].pos);
	//float magtwo = length(two) / 2.0f;
	//two = inputPatch[2].pos+normalize(two)*magtwo;

	//float3 three=(inputPatch[2].pos - inputPatch[0].pos);
	//float magthree = length(three) / 2.0f;
	//three = inputPatch[0].pos+normalize(three)*magthree;

	//float3 pos = normalize(inputPatch[0].pos + inputPatch[1].pos + inputPatch[2].pos);
	//pos = mul(float4(pos, 1), worldMatrix).xyz - ViewPosition.xyz;

	float3 V1 = mul(float4(inputPatch[0].pos, 1), worldMatrix).xyz - ViewPosition.xyz;
	float3 V2 = mul(float4(inputPatch[1].pos, 1), worldMatrix).xyz - ViewPosition.xyz;
	float3 V3 = mul(float4(inputPatch[2].pos, 1), worldMatrix).xyz - ViewPosition.xyz;

	float mag1 = length(V1);
	float mag2 = length(V2);
	float mag3 = length(V3);

	float factor;

	if (mag1 < mag2 && mag1 < mag3)
	{
		factor = (1.0f - saturate(mag1 / 15.0f)) * 32.0f + 1;
	}
	else if (mag2 < mag1 && mag2 < mag3)
	{
		factor = (1.0f - saturate(mag2 / 15.0f)) * 32.0f + 1;
	}
	else
	{
		factor = (1.0f - saturate(mag3 / 15.0f)) * 32.0f + 1;
	}

	output.edges[0] = factor;
	output.edges[1] = factor;
	output.edges[2] = factor;

    // Set the tessellation factor for tessallating inside the triangle.
	output.inside = factor;

    return output;
}

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ColorPatchConstantFunction")]
HS_OUTPUT HS_Main(InputPatch<HS_INPUT, 3> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    HS_OUTPUT output;

    output.Position = patch[pointId].pos;

    output.tex0 = patch[pointId].tex0;

    output.tangent = patch[pointId].tangent;
    output.binormal = patch[pointId].binormal;
    output.normal = patch[pointId].normal; 

    return output;
}
