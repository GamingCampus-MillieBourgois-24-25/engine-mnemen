//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 22:09:51
//

struct MeshOutput {
    float4 Position : SV_POSITION;
    float3 Color    : COLOR;
};

[outputtopology("triangle")]
[numthreads(1, 1, 1)]
void MSMain(out indices uint3 triangles[1], out vertices MeshOutput vertices[3]) {
    // Must be called before writing the geometry output
    SetMeshOutputCounts(3, 1); // 3 vertices, 1 primitive

    triangles[0] = uint3(2, 1, 0);

    vertices[0].Position = float4(-0.5, -0.5, 0.0, 1.0);
    vertices[0].Color = float3(1.0, 0.0, 0.0);

    vertices[1].Position = float4(0.5, -0.5, 0.0, 1.0);
    vertices[1].Color = float3(0.0, 1.0, 0.0);

    vertices[2].Position = float4(0.0, 0.5, 0.0, 1.0);
    vertices[2].Color = float3(0.0, 0.0, 1.0);
}

