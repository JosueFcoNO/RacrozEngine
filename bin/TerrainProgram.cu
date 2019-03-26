struct GenerationData
{
	int Size;
	double distVertex;
	float xCell;
	float yCell;

	float Range;
	int Octaves;
	float Persistance;
	int CubeSide;

	float padding[3];
};

#define GRAD_X 250
#define GRAD_Y 50

struct Vector2
{
	float x;
	float y;
};

struct Vector3
{
	float x;
	float y;
	float z;
};

struct Vertex
{
	struct Vector3 VertexPosition;
	struct Vector2 TextureCoordinates;
	struct Vector3 VertexNormals;
	struct Vector3 Tangents;
	struct Vector3 BiNormals;
};

float Mag(struct Vector3 v)
{
	return sqrt((v.x*v.x) + (v.y*v.y) + (v.z*v.z));
};

void Normalize(struct Vector3* v)
{
	float mag = Mag(v[0]);
	v->x /= mag;
	v->y /= mag;
	v->z /= mag;
};

struct Vector3 Cross(struct Vector3 v1, struct Vector3 v2)
{
	struct Vector3 ret;
	ret.x = v1.y*v2.z - v1.z*v2.y;
	ret.y = v1.z*v2.x - v1.x*v2.z;
	ret.z = v1.x*v2.y - v1.y*v2.x;

	Normalize(&ret);

	return ret;
}

float Lerp(float a, float b, float t)
{
	return (a + (b - a)*t);
};

float Dot2(struct Vector2 v1, struct Vector2 v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

float Fade(float t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
};

float Noise(float x, float y, __global char* GradIndex, __global struct Vector2* GradientVector)
{
	int xi = x;
	xi %= GRAD_X;
	int yi = y;
	yi %= GRAD_Y;
	int xi2 = (xi + 1) % GRAD_X;
	int yi2 = (yi + 1) % GRAD_Y;


	struct Vector2 gTopLeft = GradientVector[GradIndex[xi*yi - 1]];
	struct Vector2 gTopRight = GradientVector[GradIndex[xi2*yi - 1]];
	struct Vector2 gBottomLeft = GradientVector[GradIndex[xi*yi2 - 1]];
	struct Vector2 gBottomRight = GradientVector[GradIndex[xi2*yi2 - 1]];

	float xf = x - floor(x);
	float yf = y - floor(y);
	float u = Fade(xf);
	float v = Fade(yf);

	xf *= 2.0f;
	yf *= 2.0f;
	xf -= 1.0f;
	yf -= 1.0f;

	struct Vector2 vTopLeft;
	vTopLeft.x = -1 - xf;
	vTopLeft.y = -1 - yf;

	struct Vector2 vTopRight;
	vTopRight.x = 1 - xf;
	vTopRight.y = -1 - yf;

	struct Vector2 vBottomLeft;
	vBottomLeft.x = -1 - xf;
	vBottomLeft.y = 1 - yf;

	struct Vector2 vBottomRight;
	vBottomRight.x = 1 - xf;
	vBottomRight.y = 1 - yf;

	return Lerp(

		Lerp(Dot2(vTopLeft, gTopLeft),
			Dot2(vTopRight, gTopRight),
			u)

		,

		Lerp(Dot2(vBottomLeft, gBottomLeft),
			Dot2(vBottomRight, gBottomRight),
			u)

		,

		v);
};

float OctaveNoise(float x, float y, int octaves, float persistance, __global char* GradIndex, __global struct Vector2* GradientVector)
{
	float total = 0;
	float frequency = 1;
	float amplitude = 1;
	float maxValue = 0;

	for (int i = 0; i < octaves; i++) 
	{
		total += Noise(x * frequency, y * frequency, GradIndex, GradientVector) * amplitude;
	
		maxValue += amplitude;
	
		amplitude *= persistance;
		frequency *= 2;
	}

	return total / maxValue;
}

void CubeToSphere(__global struct Vector3* cubepoint)
{
	float x2 = pow(cubepoint->x, 2);
	float y2 = pow(cubepoint->y, 2);
	float z2 = pow(cubepoint->z, 2);

	cubepoint->x = cubepoint->x * sqrt(1.0f - y2 / 2.0f -
		z2 / 2.0f +
		y2 * z2 / 3.0f);

	cubepoint->y = cubepoint->y * sqrt(1.0f - z2 / 2.0f -
		x2 / 2.0f +
		x2 * z2 / 3.0f);

	cubepoint->z = cubepoint->z * sqrt(1.0f - x2 / 2.0f -
		y2 / 2.0f +
		y2 * x2 / 3.0f);

}

__kernel
void GenerateMesh(__global struct GenerationData* GenData, __global char* GradIndex, __global struct Vector2* GradientVector, __global struct Vertex* OutputVertex)
{
	int i = get_global_id(0);

	int x = i / GenData[0].Size;
	int y = i % GenData[0].Size;

	float xf = x;
	float yf = y;

	xf /= GenData[0].Size;
	yf /= GenData[0].Size;

	double xxPos = (GenData[0].distVertex*x);
	double yxPos = (GenData[0].distVertex*y);

	double xPos = (xxPos / 512.0) / 50.0 + 1.0 / 50.0 * GenData[0].xCell;
	double yPos = (yxPos / 512.0) / 50.0 + 1.0 / 50.0 * GenData[0].yCell;

	OutputVertex[i].VertexPosition.x = xPos*2.0 - 1.0;
	OutputVertex[i].VertexPosition.y = yPos*2.0 - 1.0;
	OutputVertex[i].VertexPosition.z = -1.0;

	CubeToSphere(&OutputVertex[i].VertexPosition);

	OutputVertex[i].TextureCoordinates.x = xxPos/512.0f;
	OutputVertex[i].TextureCoordinates.y = yxPos/512.0f;
};

#define HEIGTHMAP_RES 1024

__kernel
void GenerateNoise(__global struct GenerationData* GenData, __global char* GradIndex, __global struct Vector2* GradientVector, __global float* OutputImage)
{
	int i = get_global_id(0);

	int y = i / HEIGTHMAP_RES;
	int x = i % HEIGTHMAP_RES;

	float xf = x + 0.5f;
	float yf = y + 0.5f;

	xf /= 1024.0f;
	yf /= 1024.0f;

	OutputImage[i] = (OctaveNoise(xf + GenData[0].xCell, yf + GenData[0].yCell, GenData[0].Octaves, GenData[0].Persistance, GradIndex, GradientVector));
};

__kernel
void GenerateNormalTexture(__global float* NoiseTexture, __global struct Vector3* OutNormalTexture)
{
	/*int i = get_global_id(0);

	int y = i / HEIGTHMAP_RES;
	int x = i % HEIGTHMAP_RES; 

	float xf = x + 0.5f;
	float yf = y + 0.5f;

	xf /= 1024.0f;
	yf /= 1024.0f;

	OutputImage[i] = (OctaveNoise(xf + GenData[0].xCell, yf + GenData[0].yCell, GenData[0].Octaves, GenData[0].Persistance, GradIndex, GradientVector));*/
};

__kernel
void GenerateNormals(__global struct GenerationData* GenData, __global unsigned int*Indices, __global struct Vertex* OutputVertex)
{
	int i = get_global_id(0); // Lo que regresa es el indice de la operación que se está ejecutando.
	i = i * 3;

	unsigned int index1 = Indices[i];
	unsigned int index2 = Indices[i + 1];
	unsigned int index3 = Indices[i + 2];

	__global struct Vertex* Vert1 = &OutputVertex[index1];
	__global struct Vertex* Vert2 = &OutputVertex[index2];
	__global struct Vertex* Vert3 = &OutputVertex[index3];

	struct Vector3 V1;
	V1.x = Vert1->VertexPosition.x - Vert2->VertexPosition.x;
	V1.y = Vert1->VertexPosition.y - Vert2->VertexPosition.y;
	V1.z = Vert1->VertexPosition.z - Vert2->VertexPosition.z;

	struct Vector3 V2;
	V2.x = Vert1->VertexPosition.x - Vert3->VertexPosition.x;
	V2.y = Vert1->VertexPosition.y - Vert3->VertexPosition.y;
	V2.z = Vert1->VertexPosition.z - Vert3->VertexPosition.z;

	Normalize(&V1);
	Normalize(&V2);

	struct Vector3 normal = Cross(V1, V2);

	Normalize(&normal);

	OutputVertex[index1].VertexNormals = OutputVertex[index2].VertexNormals = OutputVertex[index3].VertexNormals = normal;
	OutputVertex[index1].Tangents = OutputVertex[index2].Tangents = OutputVertex[index3].Tangents = V1;
	OutputVertex[index1].BiNormals = OutputVertex[index2].BiNormals = OutputVertex[index3].BiNormals = V2;
};
