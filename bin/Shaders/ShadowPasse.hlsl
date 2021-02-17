Texture2D LightDepth : register(t0);
Texture2D PositionDepth : register(t1);
Texture2D Normals : register(t2);


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
	float bias = 0.0005f;// max(0.05 * (1.0 - dot(normalize(normals), normalize(-g_Lights[0].LightDirection.xyz))), 0.005);


	float shadow = 0.0;
	float texelSize = 1.0 / 3096.0;
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			float pcfDepth = LightDepth.Sample(LinearWrapSampler, uv + float2(x,y) * texelSize.xx);
			shadow += depth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0;

	psout.Shadow = float4((1.0f - shadow).xxx, 1);
	return psout;
}

