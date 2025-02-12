struct ToneMapperSettings
{
    uint InputIndex;
    uint DepthIndex;
    
    float Far;
    float Golden_Angle;
    float Max_Blur_Size;
    float Rad_Scale;
};

ConstantBuffer<ToneMapperSettings> Settings : register(b0);

float GetBlurSize(float Depth, float FocusPoint, float FocusScale)
{
    float Coc = clamp((1.0 / FocusPoint - 1.0 / Depth) * FocusScale, -1.0, 1.0);
    return abs(Coc) * Settings.Max_Blur_Size;
}

float3 DepthOfField(uint2 threadID, float FocusPoint, float FocusScale, uint Width, uint Height)
{   
    RWTexture2D<float4> Output = ResourceDescriptorHeap[Settings.InputIndex];
    Texture2D<float> Depth = ResourceDescriptorHeap[Settings.DepthIndex];

    float CenterDepth = Depth.Load(uint3(threadID, 0)).r;
    float CenterSize = GetBlurSize(CenterDepth, FocusPoint, FocusScale);
    float2 PixelSize = 1.0 / float2(Width, Height);

    float3 Color = Output.Load(threadID).rgb;
    float Tot = 1.0;
    float Radius = Settings.Rad_Scale;

    for (float Ang = 0.0; Radius < Settings.Max_Blur_Size; Ang += Settings.Golden_Angle) 
    {
        float2 TexCoord = threadID / float2(Width, Height);
        float2 Tc = TexCoord + float2(cos(Ang), sin(Ang)) * PixelSize * Radius;
        float2 SampleCoords = Tc * float2(Width, Height);
        float4 SampleColorFull = Output[uint2(SampleCoords.x, SampleCoords.y)];
        float3 SampleColor = SampleColorFull.rgb;
        float SampleDepth = Depth.Load(uint3(threadID, 0)).r * Settings.Far;
        float SampleSize = GetBlurSize(SampleDepth, FocusPoint, FocusScale);

        if (SampleDepth > CenterDepth) 
        {
            SampleSize = clamp(SampleSize, 0.0, CenterSize * 2.0);
        }
        float M = smoothstep(Radius - 0.5, Radius + 0.5, SampleSize);
        Color += lerp(Color / Tot, SampleColor, M);
        Tot += 1.0;     
        Radius += Settings.Rad_Scale / Radius;
    }
    return Color /= Tot;
}

[numthreads(8, 8, 1)]
void CSMain(uint3 ThreadID : SV_DispatchThreadID)
{
    RWTexture2D<float4> Output = ResourceDescriptorHeap[Settings.InputIndex];

    uint Width, Height;
    Output.GetDimensions(Width, Height); 

    if (ThreadID.x < Width && ThreadID.y < Height)
    {
        float FocusPoint = 10.0; 
        float FocusScale = 1.0;

        float3 FinalColor = DepthOfField(ThreadID.xy, FocusPoint, FocusScale, Width, Height);
        Output[ThreadID.xy] = float4(FinalColor, 1.0);
    }
}