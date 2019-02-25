#define M_PI 3.14159265359

Texture2D FinalTexture : register(t0);

sampler Sampler_ : register(s0);

cbuffer cbGamma : register(b8)
{
    int Correction;
    float Exposure;
    int2 padding;
};

struct PS_Input
{
    float4 pos : SV_POSITION;
    float2 Texcoord : TEXCOORD0;
};

struct PS_Output
{
    float4 ColorCorrection : SV_Target;
};


float4 BasicExposure(in float3 Color, in float exposure)
{
    float4 retColor = float4(Color, 1);
    retColor *= exposure;
    retColor = pow(retColor, 1.0f / 2.2f);

    return retColor;
}

float4 Reinhard(in float3 Color, in float exposure)
{
    float4 retColor = float4(Color, 1);
    retColor *= exposure;
    retColor = retColor / (1.0f + retColor);
    retColor = pow(retColor, 1.0f / 2.2f);

    return retColor;
}

float4 ReinhardRemap(in float3 Color, in float exposure)
{
    float4 retColor = float4(Color, 1);

    double L = 0.2126 * Color.r + 0.7152 * Color.g + 0.0722 * Color.b;
    double num = L * (1.0f + (L / (exposure * exposure)));
    double den = (1.0f + L);
    double nL = num / den;
    double scale = nL / L;
    retColor.r *= scale;
    retColor.g *= scale;
    retColor.b *= scale;

    //retColor *= exposure;
    //retColor = retColor / (1.0f + retColor);
    retColor = pow(retColor, 1.0f / 2.2f);

    return retColor;
}

float4 Haarm_Peter_Duiker(in float3 Color, in float exposure)
{
    float3 texColor = Color;
    texColor *= exposure;

    float3 ld = 0.0002;
    float linReference = 0.18;
    float logReference = 444;
    float logGamma = 0.45;

    float3 LogColor;
    LogColor.rgb = (log10(0.4 * texColor.rgb / linReference) / ld * logGamma + logReference) / 1023.0f;

    LogColor.rgb = saturate(LogColor.rgb);

    float FilmLutWidth = 256;
    float Padding = .5f / FilmLutWidth;

    float3 retColor;
    //retColor.r = tex2D(FilmLutSampler, float2(lerp(Padding, l - Padding, LogColor.r), 5)).r;
    //retColor.g = tex2D(FilmLutSampler, float2(lerp(Padding, l - Padding, LogColor.g), 5)).r;
    //retColor.b = tex2D(FilmLutSampler, float2(lerp(Padding, l - Padding, LogColor.b), 5)).r;

    return float4(retColor, 1);
}

float4 Burgess_Dawson(in float3 Color, in float exposure)
{
    float3 texColor = Color;
    texColor *= exposure;
    float3 x = max(0, texColor - 0.0004);
    float3 retColor = (x * (6.2 * x + .5)) / (x * (6.2 * x + 1.7) + 0.06);

    return float4(retColor, 1);
}

float3 Uncharted2Tonemap(float3 x)
{
    float A = 0.15;
    float B = 0.50;
    float C = 0.10;
    float D = 0.20;
    float E = 0.02;
    float F = 0.30;

    return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}

float4 Uncharted2(in float3 Color, in float exposure)
{
    float3 texColor = Color;
    texColor *= exposure;

    float W = 11.2;

    float ExposureBias = 2.0f;
    float3 curr = Uncharted2Tonemap(ExposureBias * texColor);

    float3 whiteScale = 1.0f / Uncharted2Tonemap(W);
    float3 color = curr * whiteScale;

    float3 retColor = pow(color, 1 / 2.2f);
    return float4(retColor, 1);
}

PS_Output PS_Main(PS_Input Input)
{
    PS_Output psout = (PS_Output) 0;

    float4 FinalColor = FinalTexture.Sample(Sampler_, Input.Texcoord);

	if (Correction == 0)
	{
		psout.ColorCorrection = Reinhard(FinalColor.xyz, Exposure);
	}
    else if (Correction == 1)
    {
		psout.ColorCorrection = ReinhardRemap(FinalColor.xyz, Exposure);
    }
    else if (Correction == 2)
    {
		psout.ColorCorrection = Burgess_Dawson(FinalColor.xyz, Exposure);
    } else
    {
		psout.ColorCorrection.xyz = Uncharted2(FinalColor.xyz, Exposure);
    }

		//psout.ColorCorrection.xyz = FinalColor.xyz;
		psout.ColorCorrection.a = 1.0f;
    return psout;
};

