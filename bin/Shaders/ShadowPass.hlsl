Texture2D ColorTex : register(t0);

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

cbuffer cbLights : register(b3)
{
    float4 g_LightColor;
    float4 g_LightPosition;
    float4 g_LightDirection;
    float4 g_Props;
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

struct PS_Input
{
    float4 position : SV_POSITION;
    float2 tex0 : TEXCOORD4;
    float3 normal : TEXCOORD0;
    float3 lightVec : TEXCOORD1;
    float3 viewVec : TEXCOORD2;
    float3 wpos : TEXCOORD3;
    float depth : TEXCOORD5;

};


PS_Input VS_Main(VS_Input vertex)
{
	PS_Input vsOut = (PS_Input)0;

	//Convierto mi vertice
    matrix m = mul(worldMatrix, viewMatrix);
    vsOut.position = mul(float4(vertex.pos, 1), m);

	vsOut.normal = vertex.normal;

    vsOut.wpos = mul(float4(vertex.pos, 1), worldMatrix);

    vsOut.lightVec = 1 * (g_LightPosition.xyz - vertex.pos);
    vsOut.viewVec = ViewPosition.xyz * vertex.pos;

    vsOut.depth = vsOut.wpos.z / 100.0f;

    float3 lightPos = g_LightPosition.xyz;

    return vsOut;
}

///Geometry Pass
float4 PS_Main(PS_Input frag) : SV_TARGET
{
    float4 output = 0;

    float3 pos = g_LightPosition.xyz - frag.wpos.xyz;
    float pos2 = length(pos) / 100.0f;
    output.x = pos2;
    output.y = pos2;
    output.z = pos2;

    output.w = pos2;

    return output;
}
