Texture3D AlbedoTexture : register(t0);
Texture3D NormalTexture : register(t1);
Texture3D MetallicTexture : register(t2);
Texture3D RoughnessTexture : register(t3);
Texture3D AOTexture : register(t4);
Texture2D HeightMap : register(t10);

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

    float dist = frag.depth / 100.0f;
    float level = clamp(HeightMap.Sample(Sampler_, frag.tex0, 0), 0.1f, 0.9f);
    float3 SamplePos = float3(frag.tex0*20.0f, level);

    output.Color = AlbedoTexture.Sample(Sampler_, SamplePos);
    output.Color.a = AOTexture.Sample(Sampler_, SamplePos).x;

    ///Set the normals to the second output color
    float3 NormalFinal; 
    NormalFinal = NormalTexture.Sample(Sampler_, SamplePos).xyz;

    float3x3 TBNReal = frag.TBN;
    TBNReal[2] = frag.TBN[1];
    TBNReal[1] = frag.TBN[2];

    NormalFinal = normalize(2.0f * NormalFinal - 1.0f);
    NormalFinal = normalize(mul(NormalFinal, TBNReal));

    output.Normal = float4(Encode(frag.normal + NormalFinal * 0.25f), MetallicTexture.Sample(Sampler_, SamplePos).x, RoughnessTexture.Sample(Sampler_, SamplePos).x);
    output.Position = float4(frag.wpos, frag.depth);

    output.Emmisive = float4(0, 0, 0, 0);

    return output;
}
