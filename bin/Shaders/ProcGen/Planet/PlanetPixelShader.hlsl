Texture3D AlbedoTexture : register(t0);
Texture3D NormalTexture : register(t1);
Texture3D MetallicTexture : register(t2);
Texture3D RoughnessTexture : register(t3);
Texture3D AOTexture : register(t4);
//Texture3D EMTexture : register(t10);

TextureCube EnviromentCube : register(t5);
TextureCube HeightCube : register(t6);

struct VS_Input
{
	float3 pos : POSITION;
	float2 tex0 : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float Displacement : TEXCOORD1;
};

struct PS_Input
{
	float4 pos : SV_POSITION;
	float2 tex0 : TEXCOORD0;
	float3 normal : NORMAL;
	float3x3 TBN : TEXCOORD1;
	float3 wpos : POSITION1;
	float depth : TEXCOORD5;
	float Displacement : TEXCOORD6;
};

struct PS_OUTPUT
{
	float4 Color : COLOR0;
	float4 Position : COLOR1;
	float4 Normal : COLOR2;
};

#include "Buffers.hlsl"
#include "../../PBR_Func.hlsl"
#include "GroundFromSpaceAtmos.hlsl"

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

///Geometry Pass
PS_OUTPUT PS_Main(PS_Input frag) : SV_TARGET
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	float height = frag.Displacement;

	float dist = frag.depth * 3;
	float level = clamp(height + .15f, 0.1f, 0.9f);

	float2 tex0 = PolarToPos2D(normalize(frag.wpos.xyz)) * 2048.0f;

	float3 samplePos = float3(tex0, level);

	///Set the color to the first output color
	float3 albedo = AlbedoTexture.SampleLevel(LinearWrapSampler, samplePos, dist);

	albedo.xyz = pow(albedo.xyz, 2.2f);

	//Set the normals to the second output color
	float3 NormalFinal;
	NormalFinal = NormalTexture.SampleLevel(LinearWrapSampler, samplePos, dist);
	
	NormalFinal = normalize(2.0f * NormalFinal - 1.0f);
	NormalFinal = normalize(mul(NormalFinal, frag.TBN));

	float rough = RoughnessTexture.SampleLevel(LinearWrapSampler, samplePos, dist).x;
	float metallic = MetallicTexture.SampleLevel(LinearWrapSampler, samplePos, dist).x;

	output.Normal = float4(Encode((NormalFinal)), 0, 0);

	output.Position = float4(frag.wpos, frag.depth);
	
	float3 atmos = groundAtmos(frag.wpos.xyz);

	output.Color.xyz = lerp(PBR_rm(frag.wpos.xyz, albedo, NormalFinal, rough, metallic, lerp(0.04f, albedo, metallic)), pow(atmos, 2.2f), 0.5f);

	//output.Color.xyz = frag.normal.xyz;

    //output.Color.xyz = atmos;
	output.Color.a = 1.0f;

	return output;
}
