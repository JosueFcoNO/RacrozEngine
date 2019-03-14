Texture2D AlbedoTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D MetallicTexture : register(t2);
Texture2D RoughnessTexture : register(t3);
Texture2D AOTexture : register(t4);
Texture2D EMTexture : register(t10);

sampler Sampler_ : register(s0);

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

struct VS_Input
{
	float3 pos : POSITION;
	float2 tex0 : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
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

struct PS_OUTPUT
{
    float4 Color : COLOR0;
    float4 Position : COLOR1;
    float4 Normal : COLOR2;
    float4 Emmisive : COLOR3;
};

PS_Input VS_Main(VS_Input vertex)
{
	PS_Input vsOut = (PS_Input)0;
	vsOut.tex0 = vertex.tex0;

	//Convierto mi vertice
    matrix m = mul(worldMatrix, ViewMatrix);
    m = mul(m, ProjectionMatrix);
	vsOut.pos = mul(float4(vertex.pos, 1), m);

    vsOut.normal = mul(float4(vertex.normal, 1.0f), worldMatrix);

	vsOut.TBN[0].xyz = vertex.tangent;
	vsOut.TBN[1].xyz = vertex.binormal;
	vsOut.TBN[2].xyz = vertex.normal;

	vsOut.TBN = mul(vsOut.TBN, worldMatrix);

	vsOut.wpos = mul(float4(vertex.pos,1), worldMatrix);

    vsOut.depth = vsOut.pos.z / 10.0f;

	return vsOut;
}

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

///Geometry Pass
PS_OUTPUT PS_Main(PS_Input frag) : SV_TARGET
{
    PS_OUTPUT output = (PS_OUTPUT) 0;

    float2 tex = frag.tex0*100.0f;

    ///Set the color to the first output color
	output.Color = frag.normal;//AlbedoTexture.Sample(Sampler_, tex);
    output.Color.a = AOTexture.Sample(Sampler_, tex).x;

   // output.Normal = float4(Encode(NormalFinal), MetallicTexture.Sample(Sampler_, frag.tex0).x, RoughnessTexture.Sample(Sampler_, frag.tex0).x);
    output.Normal = float4(Encode(normalize(frag.normal)), 0.0f, 0.1f);
    output.Position = float4(frag.wpos, frag.depth);

    output.Emmisive = float4(EMTexture.Sample(Sampler_, tex));

	return output;
}
