//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 22:09:51
//

#include "Assets/Shaders/Common/Math.hlsl"

struct Vertex
{
    float3 Position : POSITION;
    float2 TexCoords : TEXCOORD;
    float3 Normals : NORMAL;
};

struct Meshlet
{
    uint VertOffset;
    uint PrimOffset;
    uint VertCount;
    uint PrimCount;
};

struct CameraMatrices
{
    column_major float4x4 View;
    column_major float4x4 Projection;
};

struct ModelMatrices
{
    column_major float4x4 Transform;
};

struct VertexOut
{
    float4 Position : SV_POSITION;
    float2 UV : TEXCOORD;
    float3 Normals : NORMAL;
    uint MeshletIndex : COLOR0;
};


struct PushConstants
{
    int Matrices;
    int VertexBuffer;
    int IndexBuffer;
    int MeshletBuffer;
    int MeshletVertices;
    int MeshletTriangleBuffer;

    int AlbedoTexture;
    int NormalTexture;
    int LinearSampler;
    
    int ShowMeshlets;
    int2 Padding;

    column_major float4x4 Transform;
    column_major float4x4 InvTransform;
};

ConstantBuffer<PushConstants> Constants : register(b0);

VertexOut GetVertexAttributes(uint meshletIndex, uint vertexIndex)
{
    StructuredBuffer<Vertex> Vertices = ResourceDescriptorHeap[Constants.VertexBuffer];
    ConstantBuffer<CameraMatrices> Matrices = ResourceDescriptorHeap[Constants.Matrices];

    // -------- //
    Vertex v = Vertices[vertexIndex];
    float4 pos = float4(v.Position, 1.0);

    VertexOut Output = (VertexOut)0;
    Output.Position = mul(Matrices.Projection, mul(Matrices.View, mul(Constants.Transform, pos)));
    Output.UV = v.TexCoords;
    Output.Normals = normalize(float4(mul(Constants.InvTransform, float4(v.Normals, 1.0))).xyz);
    Output.MeshletIndex = meshletIndex;

    return Output;
}

[numthreads(32, 1, 1)]
[OutputTopology("triangle")]
void MSMain(
    uint GroupThreadID: SV_GroupThreadID,
    uint GroupID : SV_GroupID,
    out indices uint3 Triangles[124],
    out vertices VertexOut Verts[64]
)
{
    StructuredBuffer<uint> Indices = ResourceDescriptorHeap[Constants.IndexBuffer];
    StructuredBuffer<Meshlet> Meshlets = ResourceDescriptorHeap[Constants.MeshletBuffer];
    StructuredBuffer<uint> MeshletVertices = ResourceDescriptorHeap[Constants.MeshletVertices];
    StructuredBuffer<uint> MeshletPrimitives = ResourceDescriptorHeap[Constants.MeshletTriangleBuffer];

    // -------- //
    Meshlet m = Meshlets[GroupID];
    SetMeshOutputCounts(m.VertCount, m.PrimCount);

    for (uint i = GroupThreadID; i < m.PrimCount; i += 32) {
        uint offset = m.PrimOffset + i * 3;
        Triangles[i] = uint3(
            MeshletPrimitives[offset],
            MeshletPrimitives[offset + 1],
            MeshletPrimitives[offset + 2]
        );
    }

    for (uint i = GroupThreadID; i < m.VertCount; i += 32) {
        uint index = MeshletVertices[m.VertOffset + i];
        Verts[i] = GetVertexAttributes(GroupID, index);
    }
}
