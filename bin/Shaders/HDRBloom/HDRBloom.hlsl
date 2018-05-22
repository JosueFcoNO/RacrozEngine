Texture2D PBRTexture : register(t0);
Texture2D BloomTexture : register(t1);
Texture2D AvgLuminanceTex : register(t2);

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

cbuffer cbLights : register(b3)
{
    float4 LightColor;
    float4 LightPosition;
    float4 LightDirection;
    float4 Props;
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

struct PS_Input
{
    float4 pos : SV_POSITION;
    float2 Texcoord : TEXCOORD0;
};

struct PS_Output
{
    float4 FinalColor : SV_Target;
};

///Lightning Pass
PS_Output PS_Main(PS_Input Input)
{
    PS_Output psout = (PS_Output) 0;

    psout.FinalColor = PBRTexture.Sample(Sampler_, Input.Texcoord) + BloomTexture.Sample(Sampler_, Input.Texcoord);

    float3 Lw = PBRTexture.Load(int3(Input.pos.xy, 0), 0).xyz;

    float3 bloom = float3(0,0,0);
    bloom += 0.5f * BloomTexture.SampleLevel(Sampler_, Input.Texcoord, 0).xyz;

	float scale = 1.0f;// saturate(AvgLuminanceTex.SampleLevel(Sampler_, Input.Texcoord, 0.0f).r);
    float3 L = scale * Lw + bloom;

    psout.FinalColor = float4(L.xyz, 1.0f);
    return psout;
};

