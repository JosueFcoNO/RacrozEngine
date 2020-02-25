#define PI 3.14159265359

#ifndef COMPILEFROMFILE
#define SPECULAR 0
#define METALLIC 0
#define NDF_GGX 1
#define FRESNEL_SCHLICK 1
#define GEOMETRIC_SMITH_SCHLICK_GGX 1
#define DISNEY_BRDF 1
#endif

Texture2D AlbedoTexture : register(t0);
Texture2D NormalMRTexture : register(t1);
Texture2D PositionTexture : register(t2);
Texture2D SpecularTexture : register(t3);
Texture2D SSAOTex : register(t5);
//Texture2D ShadowMapTex : register(t6);
TextureCube EnviromentCube : register(t12);

Texture2D DiffuseResult : register(t8);
Texture2D SpecularResult : register(t9);

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
    float2 Texcoord : TEXCOORD0;
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

    //Sample the albedoColor from the texture
    float4 albedoColor = AlbedoTexture.Sample(LinearWrapSampler, Input.Texcoord);

	//If the AO value is 0, then discard the pixel.
    //if (albedoColor.w == 0.0f)
    //{
	//	psout.PBR.xyz = albedoColor.xyz;
	//	return psout;
    //}

	float4 specular = SpecularTexture.Sample(LinearWrapSampler, Input.Texcoord);

	//Sample the original normal value from the texture
	float4 NMR = NormalMRTexture.SampleLevel(LinearWrapSampler, Input.Texcoord, 0);
	
	//Get the normal value decoded
	//float3 normal = Decode(NMR.xy);
	float3 normal = (NMR.xyz);

	//Convert the Albedo to linear
    albedoColor.xyz = pow(albedoColor.xyz, 2.2f);

	//Get the Metallic and Roughness from the texture.
	float metallic = specular.a;
    float roughness = NMR.w;

	//Read the position from the depth buffer.
	float3 pos = PositionTexture.SampleLevel(LinearWrapSampler, Input.Texcoord, 0.0f).xyz;

	float4 diffuseResultColor = DiffuseResult.Sample(LinearWrapSampler, Input.Texcoord);
	float3 specularResultColor = SpecularResult.Sample(LinearWrapSampler, Input.Texcoord).xyz;

	//Calculate the PBR result
	psout.PBR.xyz = PBR_rm_VXGI(pos, albedoColor.xyz, normal, roughness, metallic, specular.xyz, diffuseResultColor, specularResultColor, diffuseResultColor.a);

    float SSAO = SSAOTex.Sample(LinearWrapSampler, Input.Texcoord).r;
    SSAO = pow(SSAO, 4.0f);

	psout.PBR.xyz *= SSAO;
    //FinalColor *= ShadowMapTex.Sample(LinearSampler, Input.Texcoord).r;
    return psout;
}

