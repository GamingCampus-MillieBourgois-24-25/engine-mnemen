//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 22:11:15
//

struct MeshInput
{
    float4 Position : SV_POSITION;
    float2 UV : TEXCOORD;
    float3 Normal : NORMAL;
    uint MeshletIndex : COLOR0;
};

uint hash(uint a)
{
    a = (a+0x7ed55d16) + (a<<12);
    a = (a^0xc761c23c) ^ (a>>19);
    a = (a+0x165667b1) + (a<<5);
    a = (a+0xd3a2646c) ^ (a<<9);
    a = (a+0xfd7046c5) + (a<<3);
    a = (a^0xb55a4f09) ^ (a>>16);
    return a;
}

struct PushConstants
{
    int Matrices;
    int VertexBuffer;
    int IndexBuffer;
    int MeshletBuffer;
    int MeshletVertices;
    int MeshletTriangleBuffer;
    int AlbedoTexture;
    int LinearSampler;
    int ShowMeshlets;
    int3 Padding;

    column_major float4x4 Transform;
};

ConstantBuffer<PushConstants> Constants : register(b0);

struct GBufferOutput
{
    float3 Normal : SV_Target0;
    float4 Albedo : SV_Target1;
};

GBufferOutput PSMain(MeshInput input)
{
    Texture2D<float4> albedoTexture = ResourceDescriptorHeap[Constants.AlbedoTexture];
    SamplerState linearSampler = SamplerDescriptorHeap[Constants.LinearSampler];

    uint meshletHash = hash(input.MeshletIndex);
    float3 meshletColor = float3(float(meshletHash & 255), float((meshletHash >> 8) & 255), float((meshletHash >> 16) & 255)) / 255.0;

    float4 textureColor = albedoTexture.Sample(linearSampler, input.UV);
    if (textureColor.a < 0.25)
        discard;
    textureColor.rgb = pow(textureColor.rgb, 2.2);

    GBufferOutput output;
    output.Albedo = Constants.ShowMeshlets ? float4(meshletColor, 1.0) : textureColor;
    output.Normal = input.Normal;
    return output;
}
