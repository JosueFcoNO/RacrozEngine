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

struct VS_Input
{
	float3 pos : POSITION;
	float2 tex0 : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float2 tex0 : TEXCOORD0;
	float3 C0 : TEXCOORD3;
	float3 C1 : TEXCOORD4;
	float3 Ray : TEXCOORD5;
};

// The scale equation calculated by Vernier's Graphical Analysis
float scale(float fCos)
{
	float x = 1.0 - fCos;
	return ScaleDepth * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
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

PS_INPUT VS_Main(VS_Input vertex)
{
	float3 wPos = mul(float4(vertex.pos, 1.0f), worldMatrix).xyz;// +vertex.normal*1.5f;
	float3 ray = wPos - ViewPosition;

	float far = length(ray);
	ray /= far;

	float wposl = length(wPos);

	float3 rayStart;
	float fStartAngle;
	float fStartDepth;

	if (CamHeight.x > OuterRadius)
	{
		float near = getNearIntersection(ViewPosition, ray, CamHeight.y, OuterRadiusSquared);
	
		rayStart = ViewPosition + (ray * near);
		far -= near;
	
		fStartAngle = dot(ray, rayStart) / OuterRadius;
	
		fStartDepth = exp(-1.0 / ScaleDepth);
	}
	else
	{
		rayStart = ViewPosition;
	
		float fHeight = length(rayStart);
		fStartAngle = dot(ray, rayStart) / fHeight;
	
		fStartDepth = exp(ScaleOverScaleDepth * (InnerRadius - CamHeight.x));

	}

	float fStartOffset = fStartDepth*scale(fStartAngle);


	float sampleLength = far / fSamples;
	float scaledLength = sampleLength * Scale;
	float3 sampleRay = ray * sampleLength;
	float3 samplePoint = rayStart + (sampleRay * .5f);

	float3 frontColor = float3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < nSamples; i++)
	{
		float height = length(samplePoint);
		float depth = exp(ScaleOverScaleDepth * (InnerRadius - height));

		float fLightAngle = dot(-g_Lights[0].LightDirection.xyz, samplePoint) / height;
		float fCameraAngle = dot(ray, samplePoint) / height;

		float scatter = fStartOffset + (depth * (scale(fLightAngle) - scale(fCameraAngle)));
		float3 attenuate = exp(-scatter * ((InvWaveLength * Kr4PI) + Km4PI));

		frontColor += attenuate * (depth * scaledLength);

		samplePoint += sampleRay;
	}

	PS_INPUT vsOut;

	//Convierto mi vertice
	matrix m = mul(worldMatrix, ViewMatrix);
	m = mul(m, ProjectionMatrix);
	vsOut.pos = mul(float4(vertex.pos, 1), m);

	//vsOut.normal = mul(float4(vertex.normal, 1.0f), worldMatrix);

	vsOut.tex0 = vertex.tex0;

	vsOut.C0 = frontColor * ((InvWaveLength * KrESun));
	vsOut.C1 = frontColor * KmESun; //Fudge the attenuation
	vsOut.Ray = ViewPosition - wPos;

	return vsOut;
}

///Geometry Pass
float4 PS_Main(PS_INPUT frag) : SV_TARGET
{
	float fCos = dot(frag.Ray, -g_Lights[0].LightDirection) / length(frag.Ray);
	float fMiePhase = getMiePhase(fCos, fCos*fCos, G, GSquared);
	float4 outColor;
	outColor.xyz = min(max((frag.C0 * getRayleighPhase(fCos*fCos) + fMiePhase * frag.C1), 0.0f.xxx), 100.0f.xxx);
	outColor.a = saturate(outColor.b);

	return outColor;
}
