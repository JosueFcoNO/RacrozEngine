Texture2D HeightTexture : register(t0);

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

    float3 V1 = mul(float4(inputPatch[0].pos, 1), worldMatrix).xyz - ViewPosition.xyz;
    float3 V2 = mul(float4(inputPatch[1].pos, 1), worldMatrix).xyz - ViewPosition.xyz;
    float3 V3 = mul(float4(inputPatch[2].pos, 1), worldMatrix).xyz - ViewPosition.xyz;

    output.edges[0] = (16.0f * (1.0f - saturate(length(V1) / 500.0f)) ) + 1;
    output.edges[1] = (16.0f * (1.0f - saturate(length(V2) / 500.0f)) ) + 1;
    output.edges[2] = (16.0f * (1.0f - saturate(length(V3) / 500.0f)) ) + 1;

    // Set the tessellation factor for tessallating inside the triangle.
    output.inside = (output.edges[0] + output.edges[1] + output.edges[2]) / 3.0f;

    return output;
}

[domain("tri")]
[partitioning("integer")]
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
