float4 GetViewFromDepth(float2 uv, float depth, column_major float4x4 inverseProj)
{
    float4 result = float4(float3(uv * 2. - 1., depth), 1.);
    result = mul(inverseProj, result);
    result.xyz /= result.w;
    result.y *= -1.0;
    return result;
}

[numthreads(8, 8, 1)]
void CSMain(uint3 threadID : SV_DispatchThreadID)
{
  
}