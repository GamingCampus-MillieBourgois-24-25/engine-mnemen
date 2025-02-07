struct ToneMapperSettings
{
    uint InputIndex;
    uint DepthIndex;
    float Far;

    float Pad;
};

ConstantBuffer<ToneMapperSettings> Settings : register(b0);

[numthreads(8, 8, 1)]
void CSMain(uint3 ThreadID : SV_DispatchThreadID)
{
    RWTexture2D<float4> Output = ResourceDescriptorHeap[Settings.InputIndex];
    Texture2D<float> Depth = ResourceDescriptorHeap[Settings.DepthIndex];

    uint Width, Height;
    Output.GetDimensions(Width, Height); 

    if (ThreadID.x < Width && ThreadID.y < Height)
    {
        float centerDepth = Depth.Load(int3(ThreadID.xy, 0)).r * Settings.Far;
    }
}