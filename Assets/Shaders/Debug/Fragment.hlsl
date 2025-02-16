//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-11 11:30:22
//

struct FragmentIn
{
    float4 Position : SV_Position;
    float3 Color : COLOR;
};

float4 PSMain(FragmentIn Input) : SV_Target
{
    return float4(Input.Color, 1.0);
}
