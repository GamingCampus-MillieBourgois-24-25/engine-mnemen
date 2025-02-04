//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 22:11:15
//

struct MeshInput
{
    float4 Position : SV_POSITION;
    float2 UV : TEXCOORD;
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
};

ConstantBuffer<PushConstants> Constants : register(b0);

float4 PSMain(MeshInput input) : SV_Target
{
    Texture2D<float4> albedoTexture = ResourceDescriptorHeap[Constants.AlbedoTexture];
    SamplerState linearSampler = SamplerDescriptorHeap[Constants.LinearSampler];

    float4 textureColor = albedoTexture.Sample(linearSampler, input.UV);
    if (textureColor.a < 0.5)
        discard;
    return float4(textureColor.rgb, 1.0);
}
