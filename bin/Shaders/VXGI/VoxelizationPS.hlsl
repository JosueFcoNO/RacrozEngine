/* 
* Copyright (c) 2012-2018, NVIDIA CORPORATION. All rights reserved. 
* 
* NVIDIA CORPORATION and its licensors retain all intellectual property 
* and proprietary rights in and to this software, related documentation 
* and any modifications thereto. Any use, reproduction, disclosure or 
* distribution of this software and related documentation without an express 
* license agreement from NVIDIA CORPORATION is strictly prohibited. 
*/ 

struct PSInput 
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD;
    float3 normal   : NORMAL;
    float3 tangent  : TANGENT;
    float3 binormal : BINORMAL;
    float3 positionWS : WSPOSITION;
    VxgiVoxelizationPSInputData vxgiData;
};

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
	float g_TesselationScale = 1.0f;
	float padd;

	float pad3 = 0.0f;
	float pad4 = 0.0f;
	float pad5 = 0.0f;
	float pad6 = 0.0f;

	float pad7 = 0.0f;
	float padding[3];
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

static const float PI = 3.14159265;

void main(PSInput IN)
{
    float3 worldPos = IN.positionWS.xyz;
    float3 normal = normalize(IN.normal.xyz);

    float3 albedo = g_DiffuseColor.rgb;

    if(g_DiffuseColor.a > 0)
        albedo = t_DiffuseColor.Sample(g_SamplerLinearWrap, IN.texCoord.xy).rgb;

    float NdotL = saturate(-dot(normal, g_LightDirection.xyz));

    float3 radiosity = 0;

    if(NdotL > 0)
    {
        float shadow = 1;

        radiosity += albedo.rgb * g_LightColor.rgb * (NdotL * shadow);
    }

    radiosity += albedo.rgb * VxgiGetIndirectIrradiance(worldPos, normal) / PI;

    VxgiStoreVoxelizationData(IN.vxgiData, normal, 1, radiosity, 0);
};