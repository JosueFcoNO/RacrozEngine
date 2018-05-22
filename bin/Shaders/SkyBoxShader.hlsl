TextureCube EnviromentCube : register(t5);

SamplerState LinealSamplerWrap : register(s0);
SamplerState AnisotropicSamplerWrap : register(s1);

SamplerState LinealSamplerClamp : register(s2);
SamplerState AnisotropicSamplerClamp : register(s3);

cbuffer cbSkyBox : register(b0)
{
    matrix SkyboxMatrix;
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
	float3 tex0 : TEXCOORD0;
};

struct PS_Input
{
	float4 pos : SV_POSITION;
	float3 tex0 : TEXCOORD0;
};

PS_Input VS_Main(VS_Input vertex)
{
	PS_Input vsOut = (PS_Input)0;

    matrix m = SkyboxMatrix;
    m = mul(m, ProjectionMatrix);
    vsOut.pos = mul(float4(vertex.pos, 1), m);
	vsOut.tex0 = vertex.pos;

	return vsOut;
}

float4 PS_Main(PS_Input frag) : SV_TARGET
{
    float4 CubeColor = EnviromentCube.SampleLevel(AnisotropicSamplerWrap, frag.tex0, 0);
    CubeColor.a = 0.0f;
    CubeColor = pow(CubeColor, 2.2f);

    return CubeColor;
}
