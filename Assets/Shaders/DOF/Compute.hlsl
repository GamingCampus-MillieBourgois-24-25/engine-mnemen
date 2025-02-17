struct PushConstants
{
    uint InputIndex;
    uint DepthIndex;

    float Near;
    float Far;
    float FocalDistance;
    float FocalRange;
};

ConstantBuffer<PushConstants> Settings : register(b0);

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Convert depth to NDC (-1 to 1)
    return (2.0 * Settings.Near * Settings.Far) / (Settings.Far + Settings.Near - z * (Settings.Far - Settings.Near));
}

float ComputeBlurFactor(float depth)
{
    float linearDepth = LinearizeDepth(depth);
    float blurFactor = clamp(abs(linearDepth - Settings.FocalDistance) / Settings.FocalRange, 0.0, 1.0);
    return blurFactor * blurFactor; // Increase effect for stronger blur
}

[numthreads(8, 8, 1)]
void CSMain(uint3 ThreadID : SV_DispatchThreadID)
{
    RWTexture2D<float4> Output = ResourceDescriptorHeap[Settings.InputIndex];
    Texture2D<float> Depth = ResourceDescriptorHeap[Settings.DepthIndex];

    uint Width, Height;
    Output.GetDimensions(Width, Height);

    if (ThreadID.x < Width && ThreadID.y < Height)
    {
        float depth = Depth.Load(uint3(ThreadID.xy, 0)).r;
        float blurFactor = ComputeBlurFactor(depth);

        float2 PixelSize = 1.0 / float2(Width, Height);
        float4 color = Output.Load(ThreadID.xy);

        if (blurFactor > 0.0)
        {
            float4 blurColor = float4(0.0, 0.0, 0.0, 0.0);
            float weight = 0.0;

            const float2 offsets[9] = {
                float2(-1,  1), float2(0,  1), float2(1,  1),
                float2(-1,  0), float2(0,  0), float2(1,  0),
                float2(-1, -1), float2(0, -1), float2(1, -1)
            };

            for (int i = 0; i < 9; i++)
            {
                float2 sampleTexCoord = (ThreadID.xy + offsets[i] * blurFactor * 2.0) / float2(Width, Height);
                float2 SampleCoords = sampleTexCoord * float2(Width, Height);
                blurColor += Output.Load(uint2(SampleCoords.x, SampleCoords.y));
                weight += 1.0;
            }

            blurColor /= weight;
            color = lerp(color, blurColor, blurFactor);
        }

        Output[ThreadID.xy] = color;
    }
}
