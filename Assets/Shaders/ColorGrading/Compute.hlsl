//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 11:25:41
//

#include "Assets/Shaders/Common/Color.hlsl"

#define ACEScc_MIDGRAY 0.4135884

struct PushConstants {
  // float4
  int TextureIndex; 
  float Brightness;   
  float Exposure;
  float Pad;

  // float4
  float Contrast;
  float Saturation;
  float2 Pad1;
};

ConstantBuffer<PushConstants> Settings : register(b0);

float3 ColorGradingContrast(float3 color)
{
    color = LinearToLogC(color);
    color = (color - ACEScc_MIDGRAY) * (Settings.Contrast * 0.1 + 1.0) + ACEScc_MIDGRAY;
    return LogCToLinear(color);
}

float3 ColorGradingSaturation(float3 color)
{
    float luminance = Luminance(color);
    return (color - luminance) * (Settings.Saturation * 0.1 + 1.0) + luminance;
}

[numthreads(8, 8, 1)]
void CSMain(uint3 ThreadID : SV_DispatchThreadID)
{
    RWTexture2D<float4> Texture = ResourceDescriptorHeap[Settings.TextureIndex]; 
    
    int width, height;
    Texture.GetDimensions(width, height);

    if (ThreadID.x < width && ThreadID.y < height) {
        
        float3 BaseColor = Texture[ThreadID.xy].rgb;
        
        BaseColor = min(BaseColor, 60.0);
        BaseColor = pow(BaseColor, 1.0 / Settings.Exposure);
        BaseColor = ColorGradingContrast(BaseColor);
        BaseColor *= Settings.Brightness; 
        BaseColor = ColorGradingSaturation(BaseColor);
        BaseColor = max(BaseColor, 0.0);

        Texture[ThreadID.xy] = float4(BaseColor, 1.0);
    }
}