Texture2D InTexture : register(t0);

sampler Sampler_ : register(s0);

cbuffer Blur_Settings : register(b0)
{
    int TextureSizeX;
    int TextureSizeY;
    int2 padding;
};

struct PS_INPUT
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
};

struct PS_OUTPUT
{
    float4 Color : SV_Target;
};

PS_OUTPUT PS_Main(PS_INPUT input)
{
    const float4 Gauss[] =
    {
        { -3, 0, 0, 1.0f / 64.0f },
        { -2, 0, 0, 6.0f / 64.0f },
        { -1, 0, 0, 15.0f / 64.0f },
        { 0, 0, 0, 20.0f / 64.0f },
        { 1, 0, 0, 15.0f / 64.0f },
        { 2, 0, 0, 6.0f / 64.0f },
        { 3, 0, 0, 1.0f / 64.0f }
    };

    float TextureOffset = 1.0f / TextureSizeX * 4.0f;
   
    float2 UV = input.TexCoord;

    float4 c = InTexture.SampleLevel(Sampler_, UV, 1);
    float4 Color = float4(0, 0, 0, 0);
   
    float4 m3 = InTexture.SampleLevel(Sampler_, UV + float2(-3 * TextureOffset, 0), 1);
    float4 m2 = InTexture.SampleLevel(Sampler_, UV + float2(-2 * TextureOffset, 0), 1);
    float4 m1 = InTexture.SampleLevel(Sampler_, UV + float2(-1 * TextureOffset, 0), 1);
    float4 p1 = InTexture.SampleLevel(Sampler_, UV + float2(1 * TextureOffset , 0), 1);
    float4 p2 = InTexture.SampleLevel(Sampler_, UV + float2(2 * TextureOffset , 0), 1);
    float4 p3 = InTexture.SampleLevel(Sampler_, UV + float2(3 * TextureOffset , 0), 1);
  
    Color +=
    m3 * Gauss[0].w +
    m2 * Gauss[1].w +
    m1 * Gauss[2].w +
    c  * Gauss[3].w +
    p1 * Gauss[4].w +
    p2 * Gauss[5].w +
    p3 * Gauss[6].w;
 
    PS_OUTPUT outPS = (PS_OUTPUT) 0;

    outPS.Color = Color;
   
    return outPS;
}
