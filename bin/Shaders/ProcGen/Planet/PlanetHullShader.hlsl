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
	float MagToView : TEXCOORD3;
};

struct DS_INPUT
{
	float3 Position : COLOR1;
	float MagToView : COLOR2;
};

struct ConstantOutputType
{
	float edges[3] : SV_TessFactor;
	float inside : SV_InsideTessFactor;
	float factor : TEXCOORD1;
	float factor2 : TEXCOORD2;
};

float Quintic(float t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
};

////////////////////////////////////////////////////////////////////////////////
// Patch Constant Function
////////////////////////////////////////////////////////////////////////////////
ConstantOutputType ColorPatchConstantFunction(InputPatch<HS_INPUT, 3> inputPatch, uint patchId : SV_PrimitiveID)
{
	ConstantOutputType output = (ConstantOutputType)0.0f;

	float dot0 = dot(normalize(inputPatch[0].pos *200.0f - ViewPosition.xyz), ViewDirection.xyz);
	float dot1 = dot(normalize(inputPatch[1].pos *200.0f - ViewPosition.xyz), ViewDirection.xyz);
	float dot2 = dot(normalize(inputPatch[2].pos *200.0f - ViewPosition.xyz), ViewDirection.xyz);

	float avg = max(dot0, max(dot1, dot2));

	if (avg < 0.5f) return output;

	float mag1 = (inputPatch[0].MagToView + inputPatch[1].MagToView) / 2.0f;
	float mag2 = (inputPatch[1].MagToView + inputPatch[2].MagToView) / 2.0f;
	float mag3 = (inputPatch[2].MagToView + inputPatch[0].MagToView) / 2.0f;

	float cull = floor(saturate(mag2 / 400.0f));

	float factor1 = pow(1.0f - saturate(mag2 / 32.0f), 2);
	float factor2 = pow(1.0f - saturate(mag3 / 32.0f), 2);
	float factor3 = pow(1.0f - saturate(mag1 / 32.0f), 2);

	float TessReal1 = factor1 * 64.0f + 1.0f - cull;
	float TessReal2 = factor2 * 64.0f + 1.0f - cull;
	float TessReal3 = factor3 * 64.0f + 1.0f - cull;

	output.edges[0] = TessReal1;
	output.edges[1] = TessReal2;
	output.edges[2] = TessReal3;

	// Set the tessellation factor for tessallating inside the triangle.
	output.inside = (TessReal1 + TessReal2 + TessReal3) / 3.0f;

	output.factor = saturate(factor1);

	output.factor2 = max(output.inside - 32.0f, 0.0f);

	return output;
}

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ColorPatchConstantFunction")]
DS_INPUT HS_Main(InputPatch<HS_INPUT, 3> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
	DS_INPUT output;
	output.Position = patch[pointId].pos;
	output.MagToView = patch[pointId].MagToView;
	return output;
}
