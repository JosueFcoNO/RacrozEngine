Texture2D LightDepth : register(t0);
Texture2D PositionDepth : register(t1);


SamplerState LinearWrapSampler : register(s0);
SamplerState AnisoWrapSampler : register(s1);
SamplerState LinearClampSampler : register(s2);
SamplerState AnisoClampSampler : register(s3);

cbuffer LightViewBuffer : register(b0)
{
    matrix lightView;
	matrix lightProj;
	float3 lightPosition;
	float padding;
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

cbuffer cbCamera : register(b5)
{
    float4 ViewPosition;
    float4 ViewDirection;
    float4 NearPlane;
    float4 FarPlane;
    matrix ViewMatrix;
    matrix ProjectionMatrix;
};

struct PS_Input
{
    float4 pos : SV_POSITION;
    float2 Texcoord : TEXCOORD0;
};

struct VS_Input
{
	float3 pos : POSITION;
	float2 tex0 : TEXCOORD0;
};

struct PS_Output
{
    float4 Shadow : SV_Target;
};

PS_Output PS_Main(PS_Input Input)
{
    PS_Output psout = (PS_Output)0;

    //Sample the albedoColor from the texture
    float4 positionDepth = PositionDepth.Sample(LinearWrapSampler, Input.Texcoord);


	matrix m = mul(lightView, lightProj);
	float4 ShadowCoord = mul(float4(positionDepth.xyz, 1), m);

	float3 uv;
	uv.xyz = ShadowCoord.xyz;

	uv.xy /= abs(ShadowCoord.w);

	uv.xy *= 0.5f;
	uv.xy += 0.5f;

	uv.y = 1.0f - uv.y;

	float depth = uv.z;

	if (uv.x < 0.0f || uv.x > 1.0f || uv.y < 0.0f || uv.y > 1.0f)
	{
		psout.Shadow = float4(0, 0, 0, 1);
		return psout;
	}

	float4 lightDepthValue = LightDepth.Sample(LinearWrapSampler, uv);
	//psout.Shadow = float4(lightDepthValue.xyz, 1);
	//psout.Shadow = float4(depth.x, 0, lightDepthValue.x, 1);
	//return psout;

	float bias = 0.005;// *tan(acos(dot(normalize(pos), frag.normal))); // cosTheta is dot( n,l ), clamped between 0 and 1

	if (depth - bias > lightDepthValue.x)
	{
		psout.Shadow = float4(0, 0, 0, 1);
		return psout;
	}



	psout.Shadow = float4(1,1,1,1);
	return psout;
}

