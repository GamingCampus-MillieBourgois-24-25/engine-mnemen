//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 11:25:41
//

float3 ACESFilm(float3 X)
{
    float A = 2.51f;
    float B = 0.03f;
    float C = 2.43f;
    float D = 0.59f;
    float E = 0.14f;
    return saturate((X * (A * X + B)) / (X * (C * X + D) + E));
}

struct TonemapperSettings
{
    uint InputIndex;
    uint OutputIndex;
    float Gamma;
    uint Pad;
};

ConstantBuffer<TonemapperSettings> Settings : register(b0);

[numthreads(8, 8, 1)]
void CSMain(uint3 ThreadID : SV_DispatchThreadID)
{
    Texture2D HDRTexture = ResourceDescriptorHeap[Settings.InputIndex];
    RWTexture2D<float4> LDRTexture = ResourceDescriptorHeap[Settings.OutputIndex];

    uint Width, Height;
    HDRTexture.GetDimensions(Width, Height);

    if (ThreadID.x <= Width && ThreadID.y <= Height)
    {
        float3 MappedColor = ACESFilm(HDRTexture[ThreadID.xy].xyz);
        LDRTexture[ThreadID.xy] = float4(pow(MappedColor.rgb, 1.0 / Settings.Gamma), 1.0);
    }
}