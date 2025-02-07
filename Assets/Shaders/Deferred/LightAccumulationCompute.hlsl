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
    
}
