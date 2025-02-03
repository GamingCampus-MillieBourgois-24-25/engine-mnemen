//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 22:11:15
//

struct MeshInput {
    float4 Position : SV_POSITION;
    float3 Color    : COLOR;
};

float4 PSMain(MeshInput input) : SV_Target
{
    return float4(input.Color, 1.0);
}
