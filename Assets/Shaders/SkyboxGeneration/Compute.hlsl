//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-17 16:25:42
//

#include "Assets/Shaders/Common/Math.hlsl"

struct Constants
{
    uint HDRInput;
    uint CubemapOutput;
    uint CubeSampler;
    uint Pad;
};

ConstantBuffer<Constants> Settings : register(b0);

[numthreads(32, 32, 1)]
void CSMain(uint3 ThreadID : SV_DispatchThreadID)
{
    Texture2D HDR = ResourceDescriptorHeap[Settings.HDRInput];
    RWTexture2DArray<half4> Cube = ResourceDescriptorHeap[Settings.CubemapOutput];
    SamplerState Sampler = ResourceDescriptorHeap[Settings.CubeSampler];

    float outputWidth, outputHeight, outputDepth;
    Cube.GetDimensions(outputWidth, outputHeight, outputDepth);

    float2 st = ThreadID.xy / float2(outputWidth, outputHeight);
    float2 uv = 2.0 * float2(st.x, 1.0 - st.y) - float2(1.0, 1.0);

    float3 ret = float3(1.0f, 1.0f, 1.0f);
    switch (ThreadID.z) {
        case 0: ret = float3( 1.0,   uv.y, -uv.x); break;
        case 1: ret = float3(-1.0,   uv.y,  uv.x); break;
        case 2: ret = float3( uv.x,  1.0,  -uv.y); break;
        case 3: ret = float3( uv.x, -1.0,   uv.y); break;
        case 4: ret = float3( uv.x,  uv.y,  1.0); break;
        case 5: ret = float3(-uv.x,  uv.y, -1.0); break;
    }
    float3 v = normalize(ret);
    float phi   = atan2(v.z, v.x);
    float theta = acos(v.y);

    float4 color = HDR.SampleLevel(Sampler, float2(phi / TwoPI, theta / PI), 1.0);
    Cube[ThreadID] = color;
}
