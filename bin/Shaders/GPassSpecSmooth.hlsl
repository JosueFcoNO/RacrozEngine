Texture2D AlbedoTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D SpecularTexture : register(t2);
Texture2D SmoothnessTexture : register(t3);
Texture2D AOTexture : register(t4);
Texture2D EMTexture : register(t10);

sampler Sampler_ : register(s0);

cbuffer cbChangeEveryFrame : register(b2)
{
	matrix worldMatrix;
	matrix previousWorldMatrix;
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
	float3 g_Albedo;
	float OverrideAlbedo;

	float3 g_Specular;
	float OverrideMetallicSpecular;

	float3 g_Emmisive;
	float OverriveEmmisive;

	float g_RoughGloss;
	float g_Metallic;
	float g_Opacity;
	float g_TesselationFactor;

	float OverrideOpacity;
	float AOStrength;
	float tileX;
	float tileY;

	float OverrideNormal;
	float OverrideRoughGloss;
	float paddings[2];
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
	float4 prevPos : TEXCOORD4;
	float4 newPos  : TEXCOORD6;

};

struct PS_OUTPUT
{
	float4 Color : COLOR0;
	float4 Position : COLOR1;
	float4 Normal : COLOR2;
	float4 Emmisive : COLOR3;
	float4 Velocity : COLOR4;
	float4 Specular : COLOR5;

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

    float2 tex = frag.tex0;
	tex.x *= tileX;
	tex.y *= tileY;

    ///Set the color to the first output color
	float3 albedo = AlbedoTexture.Sample(Sampler_, tex)  * (1.0f - OverrideAlbedo) +
		g_Albedo * OverrideAlbedo;

    output.Specular = SpecularTexture.Sample(Sampler_, tex);
    output.Color.xyz = albedo.xyz;
    output.Color.a = AOTexture.Sample(Sampler_, tex).x;

    ///Set the normals to the second output color
    float3 NormalFinal;
    NormalFinal = NormalTexture.Sample(Sampler_, tex);
    NormalFinal = normalize(2.0f * NormalFinal - 1.0f);
    NormalFinal = normalize(mul(NormalFinal, frag.TBN));

	float smooth = SmoothnessTexture.Sample(Sampler_, tex).x * (1.0f - OverrideRoughGloss) +
		g_RoughGloss * OverrideRoughGloss;

    output.Normal = float4(Encode(NormalFinal), 0.0f, 1.0f - smooth);

    output.Position = float4(frag.wpos, frag.depth);

	output.Emmisive.xyz = EMTexture.Sample(Sampler_, tex).xyz * (1.0f - OverriveEmmisive) +
		g_Emmisive * OverriveEmmisive;

	float2 a = (frag.newPos.xy / frag.newPos.w) * 0.5f + 0.5f;
	float2 b = (frag.prevPos.xy / frag.prevPos.w) * 0.5f + 0.5f;
	output.Velocity.xy = (a - b);
	output.Velocity.zw = float2(0.0f, 1.0f);

	return output;
}
