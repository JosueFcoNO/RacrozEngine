Texture2D ShadowMap : register(t5);

sampler Sampler_ : register(s0);
sampler PointSampler_ : register(s1);

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
    float3 normal : TEXCOORD0;
    float3 lightVec : TEXCOORD1;
    float3 viewVec : TEXCOORD2;
    float3 wpos : TEXCOORD3;
    float4 ShadowCoord : POSITION1;
};

PS_Input VS_Main(VS_Input vertex)
{
	PS_Input vsOut = (PS_Input)0;

	//Convierto mi vertice
    matrix m = mul(worldMatrix, ViewMatrix);
    m = mul(m, ProjectionMatrix);
    vsOut.position = mul(float4(vertex.pos, 1), m);

	vsOut.normal = vertex.normal;

    vsOut.wpos = mul(float4(vertex.pos, 1), worldMatrix);

    vsOut.lightVec = 1 * (g_LightPosition.xyz - vertex.pos);
    vsOut.viewVec = ViewPosition.xyz * vertex.pos;

    matrix m1 = mul(worldMatrix, viewMatrix);
    vsOut.ShadowCoord = mul(float4(vertex.pos, 1), m1);

    return vsOut;
}

///Check Pass
float4 PS_Main(PS_Input frag) : SV_TARGET
{
    float4 output = 0;

    float3 pos = g_LightPosition.xyz - frag.wpos.xyz;
    float CameraDepth = length(pos) / 100.0f;


    float2 uv;
    uv.x = frag.ShadowCoord.x;
    uv.y = frag.ShadowCoord.y;

    uv.x /= abs(frag.ShadowCoord.w);
    uv.y /= abs(frag.ShadowCoord.w);

    uv.x = (uv.x * 0.5f) + .5f;
    uv.y = uv.y * 0.5f + .5f;

    uv.y = 1.0f-uv.y;

    if (uv.x < 0.0f || uv.x > 1.0f || uv.y < 0.0f || uv.y > 1.0f)
    {
        return float4(1, 1, 1, 1);
    }

    float4 LightDepth = ShadowMap.Sample(Sampler_, uv);

    float bias = 0.01 * tan(acos(dot(normalize(pos), frag.normal))); // cosTheta is dot( n,l ), clamped between 0 and 1
    bias = clamp(bias, 0, 0.01);
    float visibility = 1.0;
    
    if (CameraDepth > LightDepth.x + bias)
    {
        return float4(.5f, .5f, .5f, 1);
    }

    return float4(1,1, 1, 1);
}
