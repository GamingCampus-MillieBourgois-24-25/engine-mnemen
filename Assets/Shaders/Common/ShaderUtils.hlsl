//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-07 14:49:37
//

float2 TexelToUV(uint2 texel, float2 texelSize)
{
    return ((float2)texel + 0.5f) * texelSize;
}

uint2 UVToTexel(float2 uv, uint2 size)
{
    return uv * size;
}
