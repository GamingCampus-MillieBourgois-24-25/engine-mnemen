struct ToneMapperSettings
{
    uint InputIndex;

    uint3 Pad;
};

ConstantBuffer<ToneMapperSettings> Settings : register(b0);

[numthreads(8, 8, 1)]
void CSMain(uint3 ThreadID : SV_DispatchThreadID)
{
    RWTexture2D<float4> Output = ResourceDescriptorHeap[Settings.InputIndex];

    uint Width, Height;
    Output.GetDimensions(Width, Height); 

    if (ThreadID.x < Width && ThreadID.y < Height)
    {
        int blurRadius = 3;

        float4 colorAccumulator = float4(0, 0, 0, 0);

        for (int dx = -blurRadius; dx <= blurRadius; dx++)
        {
            for (int dy = -blurRadius; dy <= blurRadius; dy++)
            {

                int2 neighborCoord = int2(ThreadID.x + dx, ThreadID.y + dy);

                if (neighborCoord.x >= 0 && neighborCoord.x < Width && neighborCoord.y >= 0 && neighborCoord.y < Height)
                {
                    float4 neighborColor = Output[neighborCoord];

                    colorAccumulator += neighborColor;
                }
            }
        }

        float numSamples = (blurRadius * 2 + 1) * (blurRadius * 2+1);
        colorAccumulator /= numSamples;

        Output[ThreadID.xy] = colorAccumulator;
    }
}