Texture2D PositionTexture : register(t0);
Texture2D NormalTexture : register(t1);

sampler Sampler_ : register(s0);

cbuffer SSAO_Settings : register(b10)
{
    float Sampler_Radius;
    float Bias;
    float Intensity;
    float Scale;
};

cbuffer cbCamera : register(b5)
{
	float4 ViewPosition;
	float4 ViewDirection;
	float4 NearPlane;
	float4 FarPlane;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
	matrix PreviousViewMatrix;
	matrix PreviousProjectionMatrix;
};

///Este es el SSAO es la hemiesferica, es ambient occlusión diffuso.
float4 GetPosition(in float2 uv)
{
    return float4((PositionTexture.Sample(Sampler_, uv).xyz).xyz, 1.0f);
}

float4 GetNormal(in float2 uv)
{
    return NormalTexture.Sample(Sampler_, uv);
}

//Hay una tecnica que puede reducir está tecnica de la distribución 
//de montecarlo, usango una textura de normales con ruido. pero hizo una 
//función que calcula un random en una distribución de montecarlo.
float3 GetRandom(in float2 uv)
{
//En cada eje se le da una distribución aleatoria en una esfera. 

    float noiseX =
   (frac(sin(dot(uv,
   float2(15.8989f, 76.132f) * 1.0f)) * 46336.23745f));
   
    float noiseY =
   (frac(sin(dot(uv,
   float2(11.9899f, 62.223f) * 2.0f)) * 34748.34744f));
   
	float noiseZ =
    (frac(sin(dot(uv,
    float2(13.3238f, 63.122f) * 3.0f)) * 59998.47362f));
   
   ///regresamos un float3 porque tenemos un espacio 3D y lo aplana a 2D
   
    return (float3(noiseX, noiseY, noiseZ));
}

float DoAmbientOcclusion(float2 tcoord, float2 uv, float3 p, float3 cnorm)
{
   //Necesito una differencia. Saco distancia en espacio del mundo entre los pixeles.
    float3 diff = GetPosition(tcoord + uv).xyz - p;
   
    const float3 v = normalize(diff);
   ///g_Scale es hasta que tanto quiero revisar.
    const float d = length(diff) * Scale;
   
   ///Bias e Intensity son variables que vamos a agregar nosotros como variables de control.
    return max(0.0f, dot(cnorm, v) - Bias) * (1.0f / (1.0f + d)) * Intensity;
}

struct PS_INPUT
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
};

struct PS_OUTPUT
{
    float4 Color : SV_TARGET;
};

float2 OctWrap(float2 v)
{
	return (1.0 - abs(v.yx)) * (v.xy >= 0.0 ? 1.0 : -1.0);
}

float2 Encode(float3 n)
{
	n /= (abs(n.x) + abs(n.y) + abs(n.z));
	n.xy = n.z >= 0.0 ? n.xy : OctWrap(n.xy);
	n.xy = n.xy * 0.5 + 0.5;
	return n.xy;
}

float3 Decode(float2 encN)
{
	encN = encN * 2.0 - 1.0;

	float3 n;
	n.z = 1.0 - abs(encN.x) - abs(encN.y);
	n.xy = n.z >= 0.0 ? encN.xy : OctWrap(encN.xy);
	n = normalize(n);
	return n;
}

PS_OUTPUT PS_Main(PS_INPUT input)
{
    PS_OUTPUT Out = (PS_OUTPUT) 0;
    Out.Color = float4(1, 0, 0, 1);
   
    const float2 vec[4] =
    {
        float2(1, 0), float2(-1, 0), float2(0, 1), float2(0, -1)
    };
   
	float4 p = GetPosition(input.TexCoord);
    float3 n = (Decode(GetNormal(input.TexCoord).xy));
    float3 rand = GetRandom(input.TexCoord);
   
    float ao = 0.0f;
    float rad = Sampler_Radius / p.z;
   
    int iterations = 4;
    for (int j = 0; j < iterations; ++j)
    {
        float2 coord1 = reflect(vec[j], rand.xy) * rad;
      ///El 0.707 es especifico porque quiero moverme eso en textura, es poco más de la media del pixel.
        float2 coord2 = float2(coord1.x - coord1.y * 0.707,
                             coord1.x + coord1.y * 0.707);
                             
        ao += DoAmbientOcclusion(input.TexCoord, coord1 * 0.25, p.xyz, n);
        ao += DoAmbientOcclusion(input.TexCoord, coord2 * 0.5, p.xyz, n);
        ao += DoAmbientOcclusion(input.TexCoord, coord1 * 0.75, p.xyz, n);
        ao += DoAmbientOcclusion(input.TexCoord, coord2, p.xyz, n);
    }
   
    ao /= (iterations * 4);
	Out.Color = 1.0f.xxxx;
   
    return Out;
}