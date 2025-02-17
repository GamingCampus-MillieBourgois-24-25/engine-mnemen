// Input settings passed from the CPU to the shader
struct PushConstants
{
    // Index of the input resource (HDR texture)
    uint InputIndex;
    uint3 Padding;
};

ConstantBuffer<PushConstants> Settings : register(b0);

float4 GetViewFromDepth(float2 uv, float depth, column_major float4x4 inverseProj)
{
    float4 result = float4(float3(uv * 2. - 1., depth), 1.);
    result = mul(inverseProj, result);
    result.xyz /= result.w;
    result.y *= -1.0;
    return result;
}

float rand1(float n)  { return frac(sin(n) * 43758.5453123); }

[numthreads(8, 8, 1)]
void CSMain(uint3 ThreadID : SV_DispatchThreadID)
{
    RWTexture2D<float4> Output = ResourceDescriptorHeap[Settings.InputIndex]; 

    uint Width, Height;
    Output.GetDimensions(Width, Height);

    if (ThreadID.x < Width && ThreadID.y < Height) {
        Output[ThreadID.xy] = Output[ThreadID.xy];
    }
}