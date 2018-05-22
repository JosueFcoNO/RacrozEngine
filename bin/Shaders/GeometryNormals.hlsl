struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 tex0 : TEXCOORD0;
    float3 normal : NORMAL;
    float3x3 TBN : TEXCOORD1;
    float3 wpos : POSITION1;
    float depth : TEXCOORD5;
};

[maxvertexcount(3)]
void GS_Main(triangle PS_IN input[3], inout TriangleStream<PS_IN> OutputStream)
{
    PS_IN v1 = input[2];
    PS_IN v2 = input[1];
    PS_IN v3 = input[0];

    float3 faceEdgeA = v2.wpos - v1.wpos;
    float3 faceEdgeB = v3.wpos - v1.wpos;
    float3 faceNormal = normalize(cross(faceEdgeA, faceEdgeB));

    v1.normal = (faceNormal);
    v2.normal = (faceNormal);
    v3.normal = (faceNormal);

    OutputStream.Append(v1);
    OutputStream.Append(v2);
    OutputStream.Append(v3);
}
