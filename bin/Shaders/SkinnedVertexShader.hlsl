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

cbuffer cbFinalMatrix : register(b6)
{
    matrix FinalMatrix[256];
};

struct VS_Input
{
    float3 pos : POSITION;
    float2 tex0 : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    int boneindices[4] : BLENDINDICES;
    float boneweight[4] : BLENDWEIGHT;
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

PS_Input VS_Main(VS_Input vertex)
{
    PS_Input vsOut = (PS_Input) 0;
    vsOut.tex0 = vertex.tex0;

    float4 bone = { 0, 0, 0, 1 };
    float4 boneN = { 0, 0, 0, 0 };
    float4 boneT = { 0, 0, 0, 0 };
    float4 boneB = { 0, 0, 0, 0 };

    bool Weights = false;

    for (int i = 0; i < 4; ++i)
    {
        if (vertex.boneindices[i] >= 0)
        {
            bone += vertex.boneweight[i] * mul(float4(vertex.pos, 1), FinalMatrix[vertex.boneindices[i]]);
            boneN += vertex.boneweight[i] * mul(float4(vertex.normal, 0), FinalMatrix[vertex.boneindices[i]]);
            boneT += vertex.boneweight[i] * mul(float4(vertex.tangent, 0), FinalMatrix[vertex.boneindices[i]]);
            boneB += vertex.boneweight[i] * mul(float4(vertex.binormal, 0), FinalMatrix[vertex.boneindices[i]]);

            Weights = true;
        }
    }
    bone.w = 1.0f;

	//Convierto mi vertice float4(vertex.pos,1)
    matrix WVP = mul(worldMatrix, ViewMatrix);
    WVP = mul(WVP, ProjectionMatrix);

	matrix prevWVP = mul(previousWorldMatrix, PreviousViewMatrix);
	prevWVP = mul(prevWVP, PreviousProjectionMatrix);

    if (Weights)
    {
        vsOut.pos = mul(bone, WVP);
        vsOut.wpos = mul(bone, worldMatrix);
        vsOut.normal = normalize(boneN);

		vsOut.prevPos = mul(bone, prevWVP);
        
        vsOut.TBN[0].xyz = boneT.xyz;
        vsOut.TBN[1].xyz = boneB.xyz;
        vsOut.TBN[2].xyz = boneN.xyz;

    }
    else
    {
        vsOut.pos = mul(float4(vertex.pos, 1), WVP);
        vsOut.wpos = mul(vertex.pos, worldMatrix);
        vsOut.normal = normalize(mul(vertex.normal, worldMatrix));

		vsOut.prevPos = mul(float4(vertex.pos, 1), prevWVP);

        vsOut.TBN[0].xyz = vertex.tangent;
        vsOut.TBN[1].xyz = vertex.binormal;
        vsOut.TBN[2].xyz = vertex.normal;

    }

	vsOut.newPos = vsOut.pos;

    vsOut.TBN = mul(vsOut.TBN, worldMatrix);

    vsOut.depth = vsOut.pos.z / 10.0f;

	

    return vsOut;
}


