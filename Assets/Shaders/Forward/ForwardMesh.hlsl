//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 22:09:51
//

struct MeshOutput {
    float4 Position : SV_POSITION;
    float3 Color    : COLOR;
};

struct PushConstants {
    column_major float4x4 View;
    column_major float4x4 Projection;
};

ConstantBuffer<PushConstants> Constants : register(b0);

static const float3 VERTICES[] = {
    float3(-0.5, -0.5, 0.0),
    float3( 0.5, -0.5, 0.0),
    float3( 0.0,  0.5, 0.0),
};

static const float3 COLORS[] = {
    float3( 1.0, 0.0, 0.0),
    float3( 0.0, 1.0, 0.0),
    float3( 0.0, 0.0, 1.0),
};

[outputtopology("triangle")]
[numthreads(1, 1, 1)]
void MSMain(out indices uint3 triangles[1], out vertices MeshOutput vertices[3]) {
    SetMeshOutputCounts(3, 1);

    triangles[0] = uint3(2, 1, 0);
    
    for (int i = 0; i < 3; i++) {
        float4 worldPos = float4(VERTICES[i], 1.0);
        
        // Transform position to clip space (NDC)
        vertices[i].Position = mul(Constants.Projection, mul(Constants.View, worldPos));
        vertices[i].Color = COLORS[i];
    }
}

