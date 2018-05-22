Texture2D NormalTexture : register(t0);
Texture2D VelocityTexture : register(t1);

sampler Sampler_ : register(s0);

struct PS_Input
{
    float4 pos : SV_POSITION;
    float2 Texcoord : TEXCOORD0;
};

struct PS_Output
{
    float4 MotionBlur : SV_Target;
};

PS_Output PS_Main(PS_Input Input)
{
    PS_Output psout = (PS_Output) 0;

    float4 FinalColor = NormalTexture.Sample(Sampler_, Input.Texcoord);

	float w, h;
	NormalTexture.GetDimensions(w, h);
	
	float2 texelSize = 1.0 / float2(w, h);

	float2 velocity = VelocityTexture.Sample(Sampler_, Input.Texcoord).rg;
	//velocity *= uVelocityScale;

	float speed = length(velocity / texelSize);
	int nSamples = clamp(int(speed), 1, 30);

	for (int i = 1; i < nSamples; ++i) 
	{
		float2 offset = velocity * (float(i) / float(nSamples - 1) - 0.5);
		FinalColor += NormalTexture.Sample(Sampler_, Input.Texcoord + offset);
	}

	FinalColor /= float(nSamples);

	psout.MotionBlur = FinalColor;
    return psout;
};

