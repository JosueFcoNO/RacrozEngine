TextureCube HeightTexture : register(t0);

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

struct DS_INPUT
{
	float3 Position : COLOR1;
	float MagToView : COLOR2;
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
	float factor : TEXCOORD1;
	float factor2 : TEXCOORD2;
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

	float3 vFrontTopLeft = NoisePos - float3(-1.0f, 1.0f, -1.0f);
	float3 vFrontTopRight = NoisePos - float3(1.0f, 1.0f, -1.0f);
	float3 vFrontBottomLeft = NoisePos - float3(-1.0f, -1.0f, -1.0f);
	float3 vFrontBottomRight = NoisePos - float3(1.0f, -1.0f, -1.0f);

	float3 vBackTopLeft = NoisePos - float3(-1.0f, 1.0f, 1.0f);
	float3 vBackTopRight = NoisePos - float3(1.0f, 1.0f, 1.0f);
	float3 vBackBottomLeft = NoisePos - float3(-1.0f, -1.0f, 1.0f);
	float3 vBackBottomRight = NoisePos - float3(1.0f, -1.0f, 1.0f);

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

	//return float(k0 + k1 * u + k2 * v + k3 * w + k4 * u * v + k5 * u * w + k6 * v * w + k7 * u * v * w);

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

float GetNoise(float3 pos)
{
	return pow(RiggedOctaveNoise(pos.xyz*128.0f, 6, 0.3f), 1);//
		//*OctaveNoise(pos.xyz*24.0f, 6, 0.5f) * OctaveNoise(pos.xyz, 2, 0.5f) +
		//pow(RiggedOctaveNoise(pos.xyz*12.0f, 2, 0.2f), 2) *
		//pow(OctaveNoise(pos.xyz, 2, 1), 2);
}

double3 Normals3(int octaves, float amp, float extend, float height, float3 normal, float3 NoisePos, float3 tangent, float3 binormal, float Scale)
{
	double theta = 0.0002f;
	//theta = 1.0;

	double3 NoisePosExt = NoisePos;

	double3 Tang = tangent*theta;
	double3 Bin = binormal*theta;

	double3 TangSamplePos = (NoisePosExt + Tang);
	double3 BinormalSamplePos = (NoisePosExt + Bin);

	double TangValue = pow(GetNoise(TangSamplePos),2) * Scale;
	double BinormalValue = pow(GetNoise(BinormalSamplePos),2) * Scale;
	double heightValue = height;

	double offX = (TangValue - heightValue);
	double offY = (BinormalValue - heightValue);

	double3 vecTang = (offX*normal + Tang);
	double3 vecBinormal = (offY*normal + Bin);

	//return (double3(height, TangValue, BinormalValue));
	return normalize(cross(vecTang, vecBinormal));
};

double3 Normals4(float height, float3 normal, float3 NoisePos, float3 tangent, float3 binormal, float Scale)
{
	double theta = 0.002;
	theta = 0.001;

	double3 TangSamplePos = (normal + tangent*theta);
	double3 BinormalSamplePos = (normal + binormal*theta);

	double TangValue = (HeightTexture.SampleLevel(Sampler_, normalize(TangSamplePos), 0).x);
	double BinormalValue = (HeightTexture.SampleLevel(Sampler_, normalize(BinormalSamplePos), 0).x);
	double heightValue = height;

	//TangValue += TangValue * TangValue;
	//BinormalValue += BinormalValue * BinormalValue;
	//height += height * height;

	//heightValue *= heightValue;
	//BinormalValue *= BinormalValue;
	//TangValue *= TangValue;

	//heightValue *= Scale;
	BinormalValue *= Scale;
	TangValue *= Scale;

	double offX = (TangValue - heightValue);
	double offY = (BinormalValue - heightValue);

	double3 vecTang = normalize(offX*normal + tangent*theta);
	double3 vecBinormal = normalize(offY*normal + binormal*theta);

	double3 normal2 = normalize(cross(vecTang, vecBinormal));
	normal2.y *= -1;
	//return double3(heightValue, TangValue, BinormalValue);
	return normalize(normal2);
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
PS_Input DS_Main(ConstantOutputType input, float3 uvwCoord : SV_DomainLocation, const OutputPatch<DS_INPUT, 3> patch)
{
	PS_Input output = (PS_Input)0;

	// Determine the position of the new vertex.
	float3 vertexPosition = uvwCoord.x * patch[0].Position + uvwCoord.y * patch[1].Position + uvwCoord.z * patch[2].Position;
	float magToView = uvwCoord.x * patch[0].MagToView + uvwCoord.y * patch[1].MagToView + uvwCoord.z * patch[2].MagToView;

	float3 normal = vertexPosition;
	normal = normalize(normal);
	float scale = 1;

	float3 NoisePos = (normal);
	NoisePos.xz *= -1;
	float3 coord = (NoisePos + 1.0f) * 0.5f;

	float octaveMult = pow(1.0f - saturate(magToView / 64.0f), 2);

	int octaves = 5 * octaveMult + 1.0f;
	float amp = 0.3f;
	float extend = 64.0f;

	//Get the height value
	float NoiseValue = pow(GetNoise(coord), 2);
	//(pow(RiggedOctaveNoise(coord*extend, octaves, amp), 2) + OctaveNoise(coord*24.0f, 6, amp));
	//HeightTexture.SampleLevel(Sampler_, normal.xyz, 0);
	float height = NoiseValue*scale;

	float3 RealTangent, RealBinormal;
	CalculateTangentAndBinormal(normal, RealTangent, RealBinormal);

	output.wpos = mul(float4(normal, 1), worldMatrix);
	output.wpos.xyz += normal * height;

	matrix m;
	m = mul(ViewMatrix, ProjectionMatrix);
	output.pos = mul(float4(output.wpos, 1), m);

	float3x3 TBN;
	TBN[0] = RealTangent;
	TBN[1] = RealBinormal;
	TBN[2] = normal;

	double3 vecNorm = //float3(0, 0, 1);
		//Normals4(height, normal, coord, (RealTangent), (-RealBinormal), scale);
		Normals3(octaves, amp, extend, height, normal, coord, (RealTangent), (RealBinormal), scale);
	//Normals2(normal, NoiseValue.xyz);
	//Normals1(NoiseValue.xyz, TBN);

//vecNorm = normalize(mul(vecNorm, TBN));

//output.TBN = mul(TBN, worldMatrix);

	output.TBN = TBN;
	output.normal = vecNorm.xyz;

	output.tex0 = 0.0f.xx;

	output.depth = output.pos.z / 100.0f;

	output.height = height;

	return output;
}