Texture2D NormalTexture : register(t0);
Texture2D VelocityTexture : register(t1);

SamplerState LinearWrapSampler : register(s0);
SamplerState AnisoWrapSampler : register(s1);
SamplerState LinearClampSampler : register(s2);
SamplerState AnisoClampSampler : register(s3);
SamplerState PointWrapSampler : register(s4);

struct PS_Input
{
    float4 pos : SV_POSITION;
    float2 Texcoord : TEXCOORD0;
};

struct PS_Output
{
    float4 MotionBlur : SV_Target;
};

cbuffer FPS : register(b7)
{
	float4 FPS;
}

PS_Output PS_Main(PS_Input Input)
{
    PS_Output psout = (PS_Output) 0;


	float w, h;
	NormalTexture.GetDimensions(w, h);
	float2 texelSize = 1.0f.xx / float2(w, h);

	float4 FinalColor = NormalTexture.Sample(PointWrapSampler, Input.Texcoord);
	float2 velocity = VelocityTexture.Sample(PointWrapSampler, Input.Texcoord).rg;

	float uVelocityScale = FPS.x / 110.0f;
	velocity *= uVelocityScale * 1.0f;

	float speed = (length(velocity / texelSize));
	int nSamples = clamp(int(speed), 24, 34);

	for (int i = 1; i < nSamples; ++i) 
	{
		float2 offset = velocity * (float(i) / (float(nSamples - 1) - 0.5f));
		FinalColor += NormalTexture.Sample(PointWrapSampler, Input.Texcoord + offset);
	}

	FinalColor /= float(nSamples);

	FinalColor.a = 1.0f;
	psout.MotionBlur = FinalColor;
    return psout;
};

