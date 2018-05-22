Texture3D AlbedoTexture : register(t0);
Texture3D NormalTexture : register(t1);
Texture3D MetallicTexture : register(t2);
Texture3D RoughnessTexture : register(t3);
Texture3D AOTexture : register(t4);
//Texture3D EMTexture : register(t10);

TextureCube EnviromentCube : register(t5);

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
	float height : TEXCOORD4;
};

struct PS_OUTPUT
{
	float4 Color : COLOR0;
	float4 Position : COLOR1;
	float4 Normal : COLOR2;
};

#include "../../PBR_Func.hlsl"

float2 PolarToPos2D(float3 polar)
{
	float u = atan2(polar.x, polar.z) / (3.1416*2)+0.5f;
	float v = asin(-polar.y) / 3.1416 + 0.5f;

	return float2(u, v);
};

///Geometry Pass
PS_OUTPUT PS_Main(PS_Input frag) : SV_TARGET
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	float dist = frag.depth / 100.0f;
	float level = clamp(frag.height*frag.height +.25f, 0.1f, 0.9f);

	float3 absNormal = abs(frag.normal);

	float3x3 tbn = frag.TBN;
	tbn[0] = normalize(frag.TBN[0]);
	tbn[1] = normalize(frag.TBN[1]);
	tbn[2] = normalize(frag.TBN[2]);

	float3 dotTri;
	dotTri.z = abs(dot(tbn[0], absNormal));
	dotTri.x = abs(dot(tbn[1], absNormal));
	dotTri.y = abs(dot(tbn[2], absNormal));
	dotTri = absNormal;

	float sum = dotTri.x + dotTri.y + dotTri.z;
	dotTri /= sum.xxx;

	float3 triPlanar1 = AlbedoTexture.SampleLevel(LinearWrapSampler,    float3(frag.wpos.yz*tileX*8, level), dist).xyz * dotTri.x;
	float3 triPlanar2 = AlbedoTexture.SampleLevel(LinearWrapSampler,    float3(frag.wpos.xz*tileX*8, level), dist).xyz * dotTri.y;
	float3 triPlanar3 = AlbedoTexture.SampleLevel(LinearWrapSampler,    float3(frag.wpos.xy*tileX*8, level), dist).xyz * dotTri.z;

	float3 result = triPlanar1 + triPlanar2 + triPlanar3;

	///Set the color to the first output color
	float3 albedo = result;// *(1.0f - OverrideAlbedo) + g_Albedo * OverrideAlbedo;

	albedo.xyz = pow(albedo.xyz, 2.2f);
	
	float height = frag.height*frag.height;
	float3 color = lerp(float3(0,0.25f,0.5f), float3(0, 1,0), height);
	
	///Set the normals to the second output color
	float3 NormalFinal;
	NormalFinal = NormalTexture.Sample(LinearWrapSampler, float3(frag.wpos.yz*tileX, level)) * (1.0f - OverrideNormal) +
	float3(0, 0, 1) * OverrideNormal;
	
	NormalFinal = normalize(2.0f * NormalFinal - 1.0f);
	NormalFinal = normalize(mul(NormalFinal, frag.TBN));
	
	float rough = RoughnessTexture.Sample(LinearWrapSampler, float3(frag.wpos.yz*tileX, level)).x * (1.0f - OverrideRoughGloss) +
	g_RoughGloss * OverrideRoughGloss;
	
	float metallic = MetallicTexture.Sample(LinearWrapSampler, float3(frag.wpos.yz*tileX, level)).x * (1.0f - OverrideMetallicSpecular) +
	g_Metallic * OverrideMetallicSpecular;
	
	output.Normal = float4(Encode((frag.normal)), 0, 0);
	
	output.Position = float4(frag.wpos, frag.depth);

	output.Color.xyz = PBR_rm(frag.wpos.xyz, albedo, normalize(frag.normal), rough, metallic);// *
		//AOTexture.Sample(LinearWrapSampler, float3(frag.wpos.yz*tileX, level)).x * AOStrength + (1.0f - AOStrength);

	//output.Color.xyz = normalize(frag.normal.xyz);
	output.Color.a = 1.0f;

	return output;
}
