Texture3D HeightTexture : register(t0);

sampler Sampler_ : register(s0);
sampler SamplerWrap : register(s1);

cbuffer cbChangeEveryFrame : register(b2)
{
	matrix worldMatrix;
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

cbuffer cbPerlinData : register(b10)
{
	float3 GradientVector[12];
};

cbuffer cbPerlinData : register(b11)
{
	float4 PermTable[256];
};

cbuffer otra : register(b12)
{
	float4 Scale;
};

struct HS_OUTPUT
{
	float3 Position : COLOR1;
	float2 tex0 : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TEXCOORD1;
	float3 binormal : TEXCOORD2;
};

struct PS_Input
{
	float4 pos : SV_POSITION;
	float2 tex0 : TEXCOORD0;
	float3 normal : NORMAL;
	float3x3 TBN : TEXCOORD1;
	float3 wpos : POSITION1;
	float depth : TEXCOORD5;
	float height : TEXCOORD4;
};

struct ConstantOutputType
{
	float edges[3] : SV_TessFactor;
	float inside : SV_InsideTessFactor;
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
	return PermTable[PermTable[PermTable[x].x + y].x + z].x;
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

float4 Noise(float x, float y, float z)
{

	int coordx = (int)x;
	int coordy = (int)y;
	int coordz = (int)z;

	int xLeft = coordx & 255;
	int xRight = (coordx + 1) & 255;

	int yBottom = (coordy) & 255;
	int yTop = (coordy + 1) & 255;

	int zFront = coordz & 255;
	int zBack = (coordz + 1) & 255;

	float3 Coord;

	Coord.x = x - coordx;
	Coord.y = y - coordy;
	Coord.z = z - coordz;

	float u = Quintic(Coord.x);
	float v = Quintic(Coord.y);
	float w = Quintic(Coord.z);

	float du = QuinticDerivate(Coord.x);
	float dv = QuinticDerivate(Coord.y);
	float dw = QuinticDerivate(Coord.z);

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

	float a = gradientDotV(hash(xLeft, yBottom, zFront), -vFrontBottomLeft);
	float b = gradientDotV(hash(xRight, yBottom, zFront), -vFrontBottomRight);
	float c = gradientDotV(hash(xLeft, yTop, zFront), -vFrontTopLeft);
	float d = gradientDotV(hash(xRight, yTop, zFront), -vFrontTopRight);

	float e = gradientDotV(hash(xLeft, yBottom, zBack), -vBackBottomLeft);
	float f = gradientDotV(hash(xRight, yBottom, zBack), -vBackBottomRight);
	float g = gradientDotV(hash(xLeft, yTop, zBack), -vBackTopLeft);
	float h = gradientDotV(hash(xRight, yTop, zBack), -vBackTopRight);

	return lerp(
		lerp(lerp(a, b, u), lerp(c, d, u), v),
		lerp(lerp(e, f, u), lerp(g, h, u), v),
		w);

	float k0 = a;
	float k1 = (b - a);
	float k2 = (c - a);
	float k3 = (e - a);
	float k4 = (a + d - b - c);
	float k5 = (a + f - b - e);
	float k6 = (a + g - c - e);
	float k7 = (b + c + e + h - a - d - f - g);

	float3 derivs;
	derivs.x = du *(k1 + k4 * v + k5 * w + k7 * v * w);
	derivs.y = dv *(k2 + k4 * u + k6 * w + k7 * v * w);
	derivs.z = dw *(k3 + k5 * u + k6 * v + k7 * v * w);

	return float4(derivs, k0 + k1 * u + k2 * v + k3 * w + k4 * u * v + k5 * u * w + k6 * v * w + k7 * u * v * w);

};

float4 OctaveNoise(float3 pos, int octaves, float persistance)
{
	float4 total = 0;
	float frequency = 1;
	float amplitude = 1;
	float maxValue = 0;

	for (int i = 0; i < octaves; i++)
	{
		float4 current = Noise(pos.x * frequency, pos.y * frequency, pos.z * frequency);
		total.w += current.w * amplitude;
		total.xyz += current.xyz;

		maxValue += amplitude;

		amplitude *= persistance;
		frequency *= 2;
	}

	total.xyz = normalize(total.xyz);
	total.w /= maxValue;

	return total;
}

float3 Normals1(float3 grad, float3x3 TBN)
{
	return mul(normalize(float3(-grad.y, -grad.z, 1)), TBN);
}

float3 Normals2(float3 normal, float3 grad)
{
	float3 GradDerivate = grad.xyz;

	GradDerivate -= dot(GradDerivate, normal)*normal;

	return normalize(normal - GradDerivate);
};

double3 Normals3(int octaves, float amp, float extend, float height, float3 normal, float3 NoisePos, float3 tangent, float3 binormal, float Scale)
{
	double theta = 0.0001f;
	//theta = 1.0;

	double3 TangSamplePos = (NoisePos*extend + tangent*theta);
	double3 BinormalSamplePos = (NoisePos*extend + binormal*theta);

	double TangValue = (OctaveNoise(TangSamplePos, octaves, amp).w);
	double BinormalValue = (OctaveNoise(BinormalSamplePos, octaves, amp).w);
	double heightValue = height;

	//TangValue += TangValue * TangValue;
	//BinormalValue += BinormalValue * BinormalValue;
	//height += height * height;
	
	//heightValue *= heightValue;
	BinormalValue *= BinormalValue;
	TangValue *= TangValue;

	//heightValue *= Scale;
	BinormalValue *= Scale;
	TangValue *= Scale;

	double offX = (TangValue - heightValue);
	double offY = (BinormalValue - heightValue);

	double3 vecTang = normalize(offX*normal + tangent*theta);
	double3 vecBinormal = normalize(offY*normal + binormal*theta);

	return normalize(cross(vecTang, vecBinormal));
};

void CalculateTangentAndBinormal(float3 normal, out float3 tangent, out float3 binormal)
{
	if (normal.x == 1)
		tangent = normalize(cross(normal, float3(0, 1, 0)));
	else
		tangent = normalize(cross(normal, float3(1, 0, 0)));

	binormal = normalize(cross(-tangent, normal));
};

[domain("tri")]
PS_Input DS_Main(ConstantOutputType input, float3 uvwCoord : SV_DomainLocation, const OutputPatch<HS_OUTPUT, 3> patch)
{
	PS_Input output;

	// Determine the position of the new vertex.
	float3 vertexPosition = uvwCoord.x * patch[0].Position + uvwCoord.y * patch[1].Position + uvwCoord.z * patch[2].Position;
	float2 tex = uvwCoord.x * patch[0].tex0 + uvwCoord.y * patch[1].tex0 + uvwCoord.z * patch[2].tex0;
	float3 normal = vertexPosition;
	float3 tangent = uvwCoord.x * patch[0].tangent + uvwCoord.y * patch[1].tangent + uvwCoord.z * patch[2].tangent;
	float3 binormal = uvwCoord.x * patch[0].binormal + uvwCoord.y * patch[1].binormal + uvwCoord.z * patch[2].binormal;

	normal = normalize(normal);
	tangent = normalize(tangent);
	binormal = normalize(binormal);

	float scale = 1.00f;

	float3 NoisePos = (normal);
	float3 coord = (NoisePos + 1.0f) / 2.0f;

	int octaves = 5;
	float amp = 0.4f;
	float extend = 128.0f;

	//Get the height value
	float4 NoiseValue = OctaveNoise(coord*extend, octaves, amp);// *saturate(OctaveNoise(coord * 4, 2, amp)) + saturate(OctaveNoise(coord * 4, 2, amp));

	float height = (NoiseValue.w*NoiseValue.w)*scale;

	//if (height > 0.0f) height = 1.0f;

	float3 RealTangent, RealBinormal;
	CalculateTangentAndBinormal(normal, RealTangent, RealBinormal);

	output.wpos = mul(float4(vertexPosition, 1), worldMatrix);
	output.wpos.xyz += normal * height;

	matrix m;
	m = mul(ViewMatrix, ProjectionMatrix);
	output.pos = mul(float4(output.wpos, 1), m);

	float3x3 TBN;
	TBN[0] = RealTangent;
	TBN[1] = RealBinormal;
	TBN[2] = normal;

	double3 vecNorm = //float3(0, 0, 1);
		Normals3(octaves, amp, extend, height, normal,  coord, normalize(RealTangent), normalize(RealBinormal), scale);
		//Normals2(normal, NoiseValue.xyz);
		//Normals1(NoiseValue.xyz, TBN);

	//vecNorm = normalize(mul(vecNorm, TBN));

	output.TBN = mul(TBN, worldMatrix);

	output.normal = vecNorm.xyz;

	output.tex0 = tex;

	output.depth = output.pos.z / 10.0f;

	output.height = height;

	return output;
}