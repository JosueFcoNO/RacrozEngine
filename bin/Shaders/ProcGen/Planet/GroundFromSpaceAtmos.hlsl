struct VS_InputAtmos
{
	float3 pos : POSITION;
	float2 tex0 : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

struct PS_INPUTAtmos
{
	float4 pos : SV_POSITION;
	float3 wpos : POSITION;
	float2 tex0 : TEXCOORD0;
	float3 C0 : TEXCOORD3;
	float3 C1 : TEXCOORD4;
};

//Atmospheric scattering variables from O'Neil's implementation
cbuffer ScatteringParameters : register(b10)
{
	int nSamples : packoffset(c0.x);
	float fSamples : packoffset(c0.y);
	float SkyScale : packoffset(c0.z);
	float OuterRadius : packoffset(c0.w);

	float3 InvWaveLength : packoffset(c1);
	float OuterRadiusSquared : packoffset(c1.w);

	float InnerRadius : packoffset(c2.x);
	float InnerRadiusSquared : packoffset(c2.y);

	float KrESun : packoffset(c2.z);
	float KmESun : packoffset(c2.w);

	float Kr4PI : packoffset(c3.x);
	float Km4PI : packoffset(c3.y);
	float Scale : packoffset(c3.z);
	float ScaleDepth : packoffset(c3.w);

	float InvScaleDepth : packoffset(c4.x);
	float ScaleOverScaleDepth : packoffset(c4.y);
	float G : packoffset(c4.z);
	float GSquared : packoffset(c4.w);
};

cbuffer CamHeight : register(b11)
{
	float4 CamHeight;
}

// The scale equation calculated by Vernier's Graphical Analysis
float scale(float fCos, float scaleDepth)
{
	float x = 1.0 - fCos;
	return scaleDepth * exp(-0.00287 + x * (0.459 + x * (3.83 + x * (-6.80 + x * 5.25))));
}

// Calculates the Mie phase function
float getMiePhase(float fCos, float fCos2, float g, float g2)
{
	return 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fCos2) / pow(1.0 + g2 - 2.0*g*fCos, 1.5);
}

// Calculates the Rayleigh phase function
float getRayleighPhase(float fCos2)
{
	//return 1.0f;
	return 0.75 + 0.75*fCos2;
}

// Returns the near intersection point of a line and a sphere
float getNearIntersection(float3 v3Pos, float3 v3Ray, float fDistance2, float fRadius2)
{
	float B = 2.0 * dot(v3Pos, v3Ray);
	float C = fDistance2 - fRadius2;
	float fDet = max(0.0, B*B - 4.0 * C);
	return 0.5 * (-B - sqrt(fDet));
}

// Returns the far intersection point of a line and a sphere
float getFarIntersection(float3 v3Pos, float3 v3Ray, float fDistance2, float fRadius2)
{
	float B = 2.0 * dot(v3Pos, v3Ray);
	float C = fDistance2 - fRadius2;
	float fDet = max(0.0, B*B - 4.0 * C);
	return 0.5 * (-B + sqrt(fDet));
}

float3 groundAtmos(float3 pos)
{
	float3 ray = pos - ViewPosition;

	float far = length(ray);
	ray /= far;

	float wposl = length(pos);
	float wposlSquared = pow(wposl, 2);

	float depth;
	float3 rayStart;

	float scaleDepth = 0.25f;

	float Scale2 = 1.0f / (OuterRadius - InnerRadius);
	float ScaleOverScaleDepth2 = Scale2 / scaleDepth;

	if (CamHeight.x > OuterRadius)
	{
		float near = getNearIntersection(ViewPosition, ray, CamHeight.y, OuterRadiusSquared);

		rayStart = ViewPosition + (ray * near);
		far -= near;

		depth = exp(max(InnerRadius - OuterRadius, 0.0f) / scaleDepth);
	}
	else
	{
		rayStart = ViewPosition;
		depth = exp(max(InnerRadius - CamHeight.x, 0.0f) / scaleDepth);
	}

	float camAngle = max(dot(-ray, pos), 20.0f) / wposl;
	float camScale = scale(camAngle, scaleDepth);

	float lightAngle = dot(-g_Lights[0].LightDirection.xyz, pos) / wposl;
	float lightScale = scale(lightAngle, scaleDepth);

	float camOffset = depth * camScale;
	float temp = (lightScale + camScale);

	float sampleLength = far / fSamples;
	float scaledLength = sampleLength * Scale;
	float3 sampleRay = ray * sampleLength;
	float3 samplePoint = rayStart + (sampleRay * 0.5f);

	float3 frontColor = float3(0.0f, 0.0f, 0.0f);
	float3 attenuate;

	for (int i = 0; i < nSamples; i++)
	{
		float height = length(samplePoint);
		float depth2 = exp(ScaleOverScaleDepth2 * (InnerRadius - height));
		float scatter = (depth2 * temp) - camOffset;
		attenuate = exp(-scatter * ((InvWaveLength * Kr4PI) + Km4PI));
		frontColor += attenuate * (depth2 * scaledLength);
		samplePoint += sampleRay;
	}

	float3 C0 = frontColor * ((InvWaveLength * KrESun) + KmESun);
	float3 C1 = attenuate.xyz; //Fudge the attenuation

	return min(max(C0 + 0.25f*C1, 0.0f.xxx), 100.0f.xxx);
}
