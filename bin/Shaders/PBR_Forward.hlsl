Texture2D AlbedoTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D MetallicTexture : register(t2);
Texture2D RoughnessTexture : register(t3);
Texture2D AOTexture : register(t4);

TextureCube EnviromentCube : register(t5);

Texture2D OpacityTexture : register(t6);

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


struct PS_Output
{
	float4 PBR : SV_Target;
};

#include "PBR_Func.hlsl"

///Lightning Pass
PS_Output PS_Main(PS_Input Input)
{
	PS_Output psout = (PS_Output)0;

	float2 tex = Input.tex0;

	//Sample the albedoColor from the texture
	float4 albedoColor = AlbedoTexture.Sample(LinearWrapSampler, tex) * (1.0f - OverrideAlbedo);
	float Opacity = albedoColor.a;

	if (Opacity == 1.0f)
	{
		Opacity = OpacityTexture.Sample(LinearWrapSampler, tex);
	}

	if (Opacity == 1.0f)
	{
		Opacity = g_Opacity;
	}

	//Sample the original normal value from the texture
	float3 normal = NormalTexture.Sample(LinearWrapSampler, tex).xyz;
	normal = normalize(2.0f * normal - 1.0f);
	normal = normalize(mul(normal, Input.TBN));

	albedoColor = pow(albedoColor, 2.2f);

	float metallic = MetallicTexture.Sample(LinearWrapSampler, tex).x * (1.0f - OverrideMetallicSpecular);
	float roughness = RoughnessTexture.Sample(LinearWrapSampler, tex).x * (1.0f - OverrideRoughGloss);

	roughness += g_RoughGloss * OverrideRoughGloss;

#if defined(METALLIC) || defined (DISNEY_BRDF)
	metallic += g_Metallic * OverrideMetallicSpecular;
#endif

	albedoColor.xyz += g_Albedo * OverrideAlbedo;
	normal += normalize(normal) * OverrideNormal;

#ifdef USE_GLOSSINESS
	roughness = 1.0f - roughness;
#endif

	float4 FinalColor;
	FinalColor.xyz = PBR_rm(Input.wpos.xyz, albedoColor, normal, roughness, metallic);

	//float SSAO = 1.0f - SSAOTex.Sample(LinearSampler, Input.Texcoord).r;
	//SSAO = pow(SSAO, 2.2f);

	FinalColor.xyz *= AOTexture.Sample(LinearWrapSampler, tex);
	FinalColor.a = Opacity;
	//FinalColor *= ShadowMapTex.Sample(LinearSampler, Input.Texcoord).r;
	psout.PBR = FinalColor;

	return psout;
}

