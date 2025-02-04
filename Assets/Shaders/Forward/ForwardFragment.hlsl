//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 22:11:15
//

struct MeshInput
{
    float4 Position : SV_POSITION;
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

float4 PSMain(MeshInput input) : SV_Target
{
    uint meshletHash = hash(input.MeshletIndex);
    float3 meshletColor = float3(float(meshletHash & 255), float((meshletHash >> 8) & 255), float((meshletHash >> 16) & 255)) / 255.0;

    return float4(meshletColor, 1.0);
}
