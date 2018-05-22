Texture2D AlbedoTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D MetallicTexture : register(t2);
Texture2D RoughnessTexture : register(t3);
Texture2D AOTexture : register(t4);
Texture2D EMTexture : register(t10);

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

cbuffer cbLights : register(b3)
{
    int g_LightNumber;
    int3 paddding;
    Light g_Lights[8];
};

cbuffer cbMaterial : register(b4)
{
	float4 DiffuseColor : packoffset(c0);
    float4 AmbientColor : packoffset(c1);
    float3 SpecularColor : packoffset(c2);
    float SpecularStrength : packoffset(c2.w);
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

struct VS_Input
{
	float3 pos : POSITION;
	float2 tex0 : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

struct HS_INPUT
{
	float3 pos : POSITION;
	float2 tex0 : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TEXCOORD1;
    float3 binormal : TEXCOORD2;
};


float3 CubeToSphere(const float3 cubepoint)
{
    float x2 = pow(cubepoint.x, 2);
    float y2 = pow(cubepoint.y, 2);
    float z2 = pow(cubepoint.z, 2);

    float3 posFinal;

    posFinal.x = cubepoint.x * sqrt(1.0f - y2 / 2.0f -
                                       z2 / 2.0f +
                                       y2 * z2 / 3.0f);

    posFinal.y = cubepoint.y * sqrt(1.0f - z2 / 2.0f -
                                       x2 / 2.0f +
                                       x2 * z2 / 3.0f);

    posFinal.z = cubepoint.z * sqrt(1.0f - x2 / 2.0f -
                                       y2 / 2.0f +
                                       y2 * x2 / 3.0f);

    return posFinal;
}

HS_INPUT VS_Main(VS_Input vertex)
{
    HS_INPUT vsOut = (HS_INPUT) 0;
	vsOut.tex0 = vertex.tex0;

    vsOut.pos = vertex.pos;

    vsOut.normal = normalize(vsOut.pos);
    vsOut.tangent = vertex.tangent;
    vsOut.binormal = vertex.binormal;

	return vsOut;
}