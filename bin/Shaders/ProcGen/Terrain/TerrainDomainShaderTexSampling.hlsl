Texture3D HeightTexture : register(t0);
Texture2D NoiseMap : register(t1);

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

cbuffer cbPerlinData
{

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
};

struct ConstantOutputType
{
    float edges[3] : SV_TessFactor;
    float inside : SV_InsideTessFactor;
};

#define GRAD_X 250.0f;
#define GRAD_Y 50.0f;

float Fade(float t)
{
    return t * t * t * (t * (t * 6 - 15) + 10);
};

float Noise(float x, float y)
{
    int xi = x;
    xi %= GRAD_X;
    int yi = y;
    yi %= GRAD_Y;
    int xi2 = (xi + 1) % GRAD_X;
    int yi2 = (yi + 1) % GRAD_Y;


    float2 gTopLeft = 0; //GradientVector[GradIndex[xi * yi - 1]];
    float2 gTopRight = 0; //GradientVector[GradIndex[xi2 * yi - 1]];
    float2 gBottomLeft = 0; // GradientVector[GradIndex[xi * yi2 - 1]];
    float2 gBottomRight = 0; //GradientVector[GradIndex[xi2 * yi2 - 1]];

    float xf = x - floor(x);
    float yf = y - floor(y);
    float u = Fade(xf);
    float v = Fade(yf);

    xf *= 2.0f;
    yf *= 2.0f;
    xf -= 1.0f;
    yf -= 1.0f;

    float2 vTopLeft;
    vTopLeft.x = -1 - xf;
    vTopLeft.y = -1 - yf;

    float2 vTopRight;
    vTopRight.x = 1 - xf;
    vTopRight.y = -1 - yf;

    float2 vBottomLeft;
    vBottomLeft.x = -1 - xf;
    vBottomLeft.y = 1 - yf;

    float2 vBottomRight;
    vBottomRight.x = 1 - xf;
    vBottomRight.y = 1 - yf;

    return lerp(

		lerp(dot(vTopLeft, gTopLeft),
			dot(vTopRight, gTopRight),
			u)

		,

		lerp(dot(vBottomLeft, gBottomLeft),
			dot(vBottomRight, gBottomRight),
			u)

		,

		v);
};

float OctaveNoise(float x, float y, int octaves, float persistance)
{
    float total = 0;
    float frequency = 1;
    float amplitude = 1;
    float maxValue = 0;

    for (int i = 0; i < octaves; i++)
    {
        total += Noise(x * frequency, y * frequency) * amplitude;
	
        maxValue += amplitude;
	
        amplitude *= persistance;
        frequency *= 2;
    }

    return total / maxValue;
}

[domain("tri")]
PS_Input DS_Main(ConstantOutputType input, float3 uvwCoord : SV_DomainLocation, const OutputPatch<HS_OUTPUT, 3> patch)
{
    PS_Input output;

    // Determine the position of the new vertex.
    float3 vertexPosition = uvwCoord.x * patch[0].Position + uvwCoord.y * patch[1].Position + uvwCoord.z * patch[2].Position;
    float2 tex = uvwCoord.x * patch[0].tex0 + uvwCoord.y * patch[1].tex0 + uvwCoord.z * patch[2].tex0;
    float3 normal = uvwCoord.x * patch[0].normal + uvwCoord.y * patch[1].normal + uvwCoord.z * patch[2].normal;
    float3 tangent = uvwCoord.x * patch[0].tangent + uvwCoord.y * patch[1].tangent + uvwCoord.z * patch[2].tangent;
    float3 binormal = uvwCoord.x * patch[0].binormal + uvwCoord.y * patch[1].binormal + uvwCoord.z * patch[2].binormal;

    normal = normalize(normal);
    tangent = normalize(tangent);
    binormal = normalize(binormal);

   //float dist = depth / 10.0f;
   //float level = clamp((frag.wpos.y / 20.0f), 0.1f, 0.9f);
    float3 SamplePos = float3(tex * 20.0f, 0);

    float HeightMapScale = 0.00f;
    float MapScale = 3.0f;

    ////Get the height value
    float height = NoiseMap.SampleLevel(Sampler_, tex, 0).x * MapScale;

    ////Offset the vertex position
    const float2 size = float2(2.0f, 0.0);
    const float3 off = float3(-1.0f / 1024.0f, 0, 1.0f / 1024.0f);

    float s01 = (NoiseMap.SampleLevel(Sampler_, float2(tex + off.xy), 0).x) * MapScale;
    float s21 = (NoiseMap.SampleLevel(Sampler_, float2(tex + off.zy), 0).x) * MapScale;
    float s10 = (NoiseMap.SampleLevel(Sampler_, float2(tex + off.yx), 0).x) * MapScale;
    float s12 = (NoiseMap.SampleLevel(Sampler_, float2(tex + off.yz), 0).x) * MapScale;

    float3 va = normalize(float3(size.x, s01 - s21, size.y));
    float3 vb = normalize(float3(size.y, s12 - s10, size.x));

    float3 normal2 = normalize(float3(cross(vb, va)));
    normal2.y *= 1;
    normal2 = normalize(normal2.zyx);

    //vertexPosition = CubeToSphere(output.wpos.xyz);
    vertexPosition.y += (height);

    output.wpos = mul(float4(vertexPosition, 1), worldMatrix);

    matrix m;
    m = mul(ViewMatrix, ProjectionMatrix);
    output.pos = mul(float4(output.wpos, 1), m);

    float3x3 TBN;
    TBN[0] = binormal;
    TBN[1] = normal;
    TBN[2] = tangent;

    output.TBN = mul(TBN, worldMatrix);

    output.normal = normal2;

    output.tex0 = tex;

    output.depth = output.pos.z / 10.0f;

    return output;
}



