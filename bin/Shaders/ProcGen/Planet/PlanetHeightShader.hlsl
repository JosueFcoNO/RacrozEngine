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
	matrix inverseWorldMatrix;
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

cbuffer Planet : register(b6)
{
	float4 PosView;
};

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
	float3 wPos : POSITION;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

cbuffer cbPerlinData : register(b11)
{
	float4 PermTable[256];
};

float Quintic(float t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
};

float QuinticDerivate(float t)
{
	return 30 * t * t * (t * (t - 2) + 1);
};

int hash(const int x, const int y, const int z)
{
	return (int)PermTable[(int)PermTable[(int)PermTable[x].x + y].x + z].x;
}

float gradientDotV(
	// a value between 0 and 255
	int perm,
	// vector from one of the corners of the cell to the point where the noise function is computed
	float3 vec)
{
	switch (perm & 11) {
	case 0: return vec.x + vec.y; // (1,1,0)
	case 1: return -vec.x + vec.y; // (-1,1,0)
	case 2: return vec.x - vec.y; // (1,-1,0)
	case 3: return -vec.x - vec.y; // (-1,-1,0)
	case 4: return vec.x + vec.z; // (1,0,1)
	case 5: return -vec.x + vec.z; // (-1,0,1)
	case 6: return vec.x - vec.z; // (1,0,-1)
	case 7: return -vec.x - vec.z; // (-1,0,-1)
	case 8: return vec.y + vec.z; // (0,1,1),
	case 9: return -vec.y + vec.z; // (0,-1,1),
	case 10: return vec.y - vec.z; // (0,1,-1),
	case 11: return -vec.y - vec.z; // (0,-1,-1)
	}
}

float Noise(float3 coord)
{

	int icoordx = (int)coord.x;
	int icoordy = (int)coord.y;
	int icoordz = (int)coord.z;

	int xLeft = icoordx & 255;
	int xRight = (icoordx + 1) & 255;

	int yBottom = (icoordy) & 255;
	int yTop = (icoordy + 1) & 255;

	int zFront = icoordz & 255;
	int zBack = (icoordz + 1) & 255;

	float3 Coord;

	Coord.x = coord.x - icoordx;
	Coord.y = coord.y - icoordy;
	Coord.z = coord.z - icoordz;

	float u = Quintic(Coord.x);
	float v = Quintic(Coord.y);
	float w = Quintic(Coord.z);

	float3 NoisePos = Coord * 2.0f - 1.0f;

	float3 vFrontTopLeft = float3(-1.0f, 1.0f, -1.0f);
	float3 vFrontTopRight = float3(1.0f, 1.0f, -1.0f);
	float3 vFrontBottomLeft = float3(-1.0f, -1.0f, -1.0f);
	float3 vFrontBottomRight = float3(1.0f, -1.0f, -1.0f);

	float3 vBackTopLeft = float3(-1.0f, 1.0f, 1.0f);
	float3 vBackTopRight = float3(1.0f, 1.0f, 1.0f);
	float3 vBackBottomLeft = float3(-1.0f, -1.0f, 1.0f);
	float3 vBackBottomRight = float3(1.0f, -1.0f, 1.0f);

	vFrontTopLeft = NoisePos - vFrontTopLeft;
	vFrontTopRight = NoisePos - vFrontTopRight;
	vFrontBottomLeft = NoisePos - vFrontBottomLeft;
	vFrontBottomRight = NoisePos - vFrontBottomRight;

	vBackTopLeft = NoisePos - vBackTopLeft;
	vBackTopRight = NoisePos - vBackTopRight;
	vBackBottomLeft = NoisePos - vBackBottomLeft;
	vBackBottomRight = NoisePos - vBackBottomRight;

	float a = gradientDotV(hash(xLeft, yBottom, zFront), vFrontBottomLeft);
	float b = gradientDotV(hash(xRight, yBottom, zFront), vFrontBottomRight);
	float c = gradientDotV(hash(xLeft, yTop, zFront), vFrontTopLeft);
	float d = gradientDotV(hash(xRight, yTop, zFront), vFrontTopRight);

	float e = gradientDotV(hash(xLeft, yBottom, zBack), vBackBottomLeft);
	float f = gradientDotV(hash(xRight, yBottom, zBack), vBackBottomRight);
	float g = gradientDotV(hash(xLeft, yTop, zBack), vBackTopLeft);
	float h = gradientDotV(hash(xRight, yTop, zBack), vBackTopRight);

	return lerp(
		lerp(lerp(a, b, u), lerp(c, d, u), v),
		lerp(lerp(e, f, u), lerp(g, h, u), v),
		w);

	//float k0 = a;
	//float k1 = (b - a);
	//float k2 = (c - a);
	//float k3 = (e - a);
	//float k4 = (a + d - b - c);
	//float k5 = (a + f - b - e);
	//float k6 = (a + g - c - e);
	//float k7 = (b + c + e + h - a - d - f - g);

	//float3 derivs;
	//derivs.x = du *(k1 + k4 * v + k5 * w + k7 * v * w);
	//derivs.y = dv *(k2 + k4 * u + k6 * w + k7 * v * w);
	//derivs.z = dw *(k3 + k5 * u + k6 * v + k7 * v * w);

	//return float4(derivs, k0 + k1 * u + k2 * v + k3 * w + k4 * u * v + k5 * u * w + k6 * v * w + k7 * u * v * w);

};

float OctaveNoise(float3 pos, int octaves, float persistance)
{
	float total = 0;
	float frequency = 1;
	float amplitude = 1;
	float maxValue = 0;

	for (int i = 0; i < octaves; i++)
	{
		float current = Noise(pos * frequency);
		total += current * amplitude;

		maxValue += amplitude;

		amplitude *= persistance;
		frequency *= 2;
	}

	total /= maxValue;

	return total;
}

float RiggedOctaveNoise(float3 pos, int octaves, float persistance)
{
	float total = 0;
	float frequency = 1;
	float amplitude = 1;
	float maxValue = 0;

	for (int i = 0; i < octaves; i++)
	{
		float current = 1.0f - abs(Noise(pos * frequency));
		total += current * amplitude;

		maxValue += amplitude;

		amplitude *= persistance;
		frequency *= 2;
	}

	total /= maxValue;

	return total;
}

PS_INPUT VS_Main(VS_Input vertex)
{
	PS_INPUT vsOut = (PS_INPUT)0;

	//Convierto mi vertice
	matrix m;
	m = mul(ViewMatrix, ProjectionMatrix);
	vsOut.pos = mul(float4(vertex.pos, 1), m);

	vsOut.wPos = (normalize(vertex.pos.xyz) + 1.0f) / 2.0f;
	vsOut.tangent = vertex.tangent;
	vsOut.binormal = vertex.binormal;

	return vsOut;
}

float GetNoise(float3 pos)
{
	return pow(RiggedOctaveNoise(pos.xyz*128.0f, 6, 0.4f), 2) * OctaveNoise(pos.xyz*24.0f, 6, 0.5f) * OctaveNoise(pos.xyz, 2, 0.5f) + 
		pow(RiggedOctaveNoise(pos.xyz*12.0f, 2, 0.2f), 2) * 
		pow(OctaveNoise(pos.xyz, 2, 1), 2);
}

void CalculateTangentAndBinormal(float3 normal, out float3 tangent, out float3 binormal)
{
	if (normal.x == 1)
		tangent = normalize(cross(normal, float3(0, 1, 0)));
	else
		tangent = normalize(cross(normal, float3(1, 0, 0)));

	binormal = normalize(cross(-tangent, normal));
};


#define PI (3.1415926535897932f)
#define PI2 2*(3.1415926535897932f)


float2 PolarToPos2D(float3 polar)
{
	float u = atan2(polar.x, polar.z) / (3.1416 * 2) + 0.5f;
	float v = asin(-polar.y) / 3.1416 + 0.5f;

	return float2(u, v);
};

float3 PosToPolar2D(float2 pos)
{
	float x = cos(PI*(0.5f - pos.y))*sin(PI2*(pos.x - 0.5f));
	float y = sin(PI*(0.5f - pos.y));
	float z = cos(PI*(0.5f - pos.y))*cos(PI2*(pos.x - 0.5f));
	return float3(x, y, z);
}

float4 PS_Main(PS_INPUT frag) : SV_TARGET
{
	float theta = 0.0001f;

	float3 normal = (frag.wPos);

	float2 pos2 = PolarToPos2D(normal);
	pos2.x += theta;

	float3 TangPos = PosToPolar2D(pos2);

	pos2.x -= theta;
	pos2.y += theta;

	float3 BinPos = PosToPolar2D(pos2);

	float noise =  GetNoise(normal);
	float noiseT = 0;
	float noiseB = 0;
	//return float4(normalize(cross(vecTang, vecBinormal)), 1.0f);
	return float4(noise, noise, noise, 1.0f);
}