//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 11:25:41
//

struct PushConstants {
  int TextureIndex; 
  float Brightness;   
  float Exposure;
  float Pad;
};

ConstantBuffer<PushConstants> Settings : register(b0);

[numthreads(8, 8, 1)]
void CSMain(uint3 ThreadID : SV_DispatchThreadID)
{
    RWTexture2D<float4> Texture = ResourceDescriptorHeap[Settings.TextureIndex]; 
    
    float4 Color = Texture[ThreadID.xy];
    Color *= Settings.Brightness; 
    Color = pow(Color, Settings.Exposure); 
    Texture[ThreadID.xy] = Color;

}