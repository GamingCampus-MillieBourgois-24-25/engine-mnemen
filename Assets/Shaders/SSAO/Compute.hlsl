#include "Assets/Shaders/Common/ShaderUtils.hlsl"

// Input settings
struct PushConstants
{
    uint DepthIndex;   // Index for depth texture
    uint OutputIndex;  // Index for output texture
    uint2 Padding; // Padding

    column_major float4x4 InverseProjection; // The inverse of the projection matrix
};

// Bind the push constants
ConstantBuffer<PushConstants> Settings : register(b0);

// SSAO Parameters
#define SAMPLE_COUNT 16
#define RADIUS 0.5
#define BIAS 0.025

// Sample kernel (define 16 precomputed offsets in a hemisphere)
static const float3 KernelSamples[SAMPLE_COUNT] = {
    float3( 0.5381,  0.1856, -0.4319),
    float3( 0.1379,  0.2486,  0.4430),
    float3( 0.3371, -0.7696,  0.3490),
    float3( 0.6475, -0.1764, -0.7776),
    float3( 0.2594,  0.9035,  0.3427),
    float3( 0.9442, -0.3420,  0.0200),
    float3(-0.1854,  0.5159, -0.8327),
    float3(-0.8743, -0.3690, -0.3122),
    float3( 0.7633, -0.7202,  0.0020),
    float3( 0.6442,  0.7243, -0.2482),
    float3( 0.1234, -0.4257,  0.8264),
    float3(-0.6774,  0.1133,  0.7244),
    float3(-0.3489, -0.5014, -0.7911),
    float3(-0.7163,  0.2271,  0.6590),
    float3(-0.6663, -0.5206,  0.5356),
    float3( 0.5697, -0.2816, -0.7717)
};

// Function to reconstruct view-space position from depth
float4 GetViewPosition(float2 uv, float depth, column_major float4x4 inverseProj)
{
    float4 result = float4(float3(uv * 2. - 1., depth), 1.);
    result = mul(inverseProj, result);
    result.xyz /= result.w;
    result.y *= -1.0;
    return result;
}

// Random function
float2 hash2(inout float HASH2SEED) 
{
    HASH2SEED += 0.1f;
    float2 x= frac(sin(float2(HASH2SEED, HASH2SEED + 0.1f)) * float2(43758.5453123, 22578.1459123));
    HASH2SEED += 0.2f;
	return x;
}

// SSAO Shader
[numthreads(8, 8, 1)]
void CSMain(uint3 ThreadID : SV_DispatchThreadID)
{
    // Get the depth texture from the index passed from the CPU
    Texture2D<float> DepthTexture = ResourceDescriptorHeap[Settings.DepthIndex];
    RWTexture2D<float> OutputAO = ResourceDescriptorHeap[Settings.OutputIndex];

    // Get the width and height of output texture
    uint Width, Height;
    OutputAO.GetDimensions(Width, Height);
    
    // If the pixel position is out of bounds, just return and don't do anything
    if (ThreadID.x >= Width || ThreadID.y >= Height) return;

    // Get the current pixel depth
    float depth = DepthTexture.Load(uint3(ThreadID.xy, 0));
    // Compute the UV (texture coordinate) in range [-1, 1]
    // Meanwhile, ThreadID.xy (pixel position) is in range [0, Width] and [0, Height]
    float2 uv = (ThreadID.xy + 0.5) / float2(Width, Height);

    // Use the function GetViewPosition to get the pixel depth in view space
    float3 viewPos = GetViewPosition(uv, depth, Settings.InverseProjection).xyz;

    // Default size (can be changed)
    int noiseWidth = 4, noiseHeight = 4;

    float2 NoiseScale = float2(Width / noiseWidth, Height / noiseHeight);
    float2 TexCoords = TexelToUV(ThreadID.xy, 1.0 / float2(Width, Height));
    
	float Hash2Seed;
	Hash2Seed = (TexCoords.x * TexCoords.y) * Height;

    // Sample random noise
    float2 noise = float2(hash2(Hash2Seed).x, hash2(Hash2Seed).y);

    // Generate a tangent space basis
    float3 tangent = normalize(cross(float3(0, 1, 0), viewPos));
    float3 bitangent = cross(viewPos, tangent);
    float3x3 TBN = float3x3(tangent, bitangent, viewPos);

    // SSAO Computation
    float occlusion = 0.0;
    for (int i = 0; i < SAMPLE_COUNT; i++)
    {
        // Rotate sample using noise
        float3 sampleVec = normalize(mul(TBN, KernelSamples[i]));
        sampleVec = viewPos + sampleVec * RADIUS;

        // Project back to screen space
        float4 projected = mul(Settings.InverseProjection, float4(sampleVec, 1.0));
        projected.xy /= projected.w;
        projected.xy = projected.xy * 0.5 + 0.5;

        // Sample depth at new position
        uint2 samplePosition = UVToTexel(projected.xy, uint2(Width, Height));
        float sampleDepth = DepthTexture.Load(uint3(samplePosition, 0)).r;
        float3 sampleViewPos = GetViewPosition(projected.xy, sampleDepth, Settings.InverseProjection).xyz;

        // Accumulate occlusion
        float rangeCheck = smoothstep(0.0, 1.0, RADIUS / abs(viewPos.z - sampleViewPos.z));
        occlusion += (sampleViewPos.z >= viewPos.z + BIAS ? 1.0 : 0.0) * rangeCheck;
    }
    occlusion = 1.0 - (occlusion / SAMPLE_COUNT);
    
    // Store AO value
    OutputAO[ThreadID.xy] = occlusion;
}
