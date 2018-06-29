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

	float height = frag.height;
	//HeightCube.Sample(LinearWrapSampler, normalize(frag.normal.xyz));
	float dist = frag.depth;
	float level = clamp(height + .25f, 0.1f, 0.9f);

	float2 tex0 = PolarToPos2D(normalize(frag.wpos.xyz)) * 1800.0f;

	///Set the color to the first output color
	float3 albedo = AlbedoTexture.SampleLevel(LinearWrapSampler, float3(tex0, level), dist);

	albedo.xyz = pow(albedo.xyz, 2.2f);
	//float3 color = lerp(float3(0,0.25f,0.5f), float3(0, 1,0), height);

	///Set the normals to the second output color
	//float3 NormalFinal;
	//NormalFinal = NormalTexture.Sample(LinearWrapSampler, float3(tex0, level));
	//
	//NormalFinal = normalize(2.0f * NormalFinal - 1.0f);
	//NormalFinal = normalize(mul(NormalFinal, frag.TBN));

	float rough = RoughnessTexture.Sample(LinearWrapSampler, float3(tex0, level)).x;
	float metallic = MetallicTexture.Sample(LinearWrapSampler, float3(tex0, level)).x;

	output.Normal = float4(Encode((frag.normal)), 0, 0);

	output.Position = float4(frag.wpos, frag.depth);
	
	output.Color.xyz = PBR_rm(frag.wpos.xyz, albedo, normalize(frag.normal.xyz).xyz, rough, metallic);// *
		//AOTexture.Sample(LinearWrapSampler, float3(frag.wpos.yz*tileX, level)).x * AOStrength + (1.0f - AOStrength);

	//output.Color.xyz = frag.normal.xyz;

	//output.Color.xyz = frag.normal.xyz;
	output.Color.a = 1.0f;

	return output;
}
