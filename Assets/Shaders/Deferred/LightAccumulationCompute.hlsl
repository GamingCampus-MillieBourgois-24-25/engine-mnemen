//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-07 14:42:28
//

struct PushConstants
{
    uint DepthTexture;
    uint AlbedoTexture;
    uint NormalTexture;
    uint OutputTexture;
};

ConstantBuffer<PushConstants> Settings : register(b0);

[numthreads(8, 8, 1)]
void CSMain(uint3 ThreadID : SV_DispatchThreadID)
{
    RWTexture2D<float4> output = ResourceDescriptorHeap[Settings.OutputTexture];

    int width, height;
    output.GetDimensions(width, height);

    if (ThreadID.x < width || ThreadID.y < height) {
        Texture2D<float4> albedo = ResourceDescriptorHeap[Settings.AlbedoTexture];
        output[ThreadID.xy] = albedo.Load(ThreadID);
    }
}
