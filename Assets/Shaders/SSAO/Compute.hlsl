#include "Assets/Shaders/Common/ShaderUtils.hlsl"

// Input settings
struct PushConstants
{
    uint DepthIndex;   // Index for depth texture
    uint OutputIndex;  // Index for output texture
    uint2 Padding; // Padding

    column_major float4x4 Projection;
    column_major float4x4 InverseProjection; // The inverse of the projection matrix
};

// Bind the push constants
ConstantBuffer<PushConstants> Settings : register(b0);

// SSAO Parameters
#define SAMPLE_COUNT 16
#define RADIUS 5
#define BIAS 0.00005

// Function to reconstruct view-space position from depth
float4 GetViewPosition(float2 uv, float depth, column_major float4x4 inverseProj)
{
    float4 result = float4(float3(uv * 2.0f - 1.0f, depth), 1.0f); // Convert to normalized device coordinates
    result = mul(inverseProj, result); // Inverse projection to get view-space position
    result.xyz /= result.w; // Perspective divide
    result.y *= -1.0f; // Flip y axis
    return result;
}

// Random function (hash2)
float2 hash2(inout float HASH2SEED)
{
    HASH2SEED += 0.1f;
    float2 x = frac(sin(float2(HASH2SEED, HASH2SEED + 0.1f)) * float2(43758.5453123f, 22578.1459123f));
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
    // Compute the UV (texture coordinate) in range [0, 1]
    float2 uv = (ThreadID.xy + 0.5f) / float2(Width, Height);

    // Use the function GetViewPosition to get the pixel depth in view space
    float3 viewPos = GetViewPosition(uv, depth, Settings.Projection).xyz;

    // Default size (can be changed)
    int noiseWidth = 4, noiseHeight = 4;

    float2 NoiseScale = float2(Width / noiseWidth, Height / noiseHeight);
    float2 TexCoords = TexelToUV(ThreadID.xy, 1.0f / float2(Width, Height));
    
    // Seed for random noise generation
    float Hash2Seed = (TexCoords.x * TexCoords.y) * Height;

    // Sample random noise
    float2 noise = hash2(Hash2Seed);
    
    // Generate a tangent space basis (normal, tangent, bitangent)
    float3 normal = normalize(viewPos); // Approximate normal from view position
    float3 tangent = normalize(cross(float3(0, 1, 0), normal));
    float3 bitangent = cross(normal, tangent);
    float3x3 TBN = float3x3(tangent, bitangent, normal);

    // SSAO Computation
    float occlusion = 0.0f;
    for (int i = 0; i < SAMPLE_COUNT; i++)
    {
        // Sample hemisphere (random samples)
        float3 rv = normalize(float3(hash2(Hash2Seed) * 2.0f - 1.0f, hash2(Hash2Seed).x)) * hash2(Hash2Seed).x;

        // Rotate sample using noise
        float3 sampleVec = normalize(mul(TBN, rv));

        sampleVec = sampleVec + viewPos;

        // Project back to screen space
        float4 projected = mul(Settings.InverseProjection, float4(sampleVec, 1.0f));
        projected.xy /= projected.w;  // Perspective divide
        projected.xy = projected.xy * 0.5f + 0.5f; // Transform back to [0, 1] range

        // Sample depth at new position
        uint2 samplePosition = UVToTexel(projected.xy, uint2(Width, Height));
        float sampleDepth = DepthTexture.Load(uint3(samplePosition, 0)).r;
        float3 sampleViewPos = GetViewPosition(projected.xy, sampleDepth, Settings.Projection).xyz;

        // Accumulate occlusion based on depth comparison and distance
        float rangeCheck = smoothstep(0.0f, 1.0f, RADIUS / max(abs(viewPos.z - sampleViewPos.z), 0.0001f));
        occlusion += (sampleViewPos.z >= viewPos.z + BIAS ? 1.0f : 0.0f) * rangeCheck;
    }
    
    occlusion = 1.0f - (occlusion / SAMPLE_COUNT);
    
    // Store AO value
    OutputAO[ThreadID.xy] = occlusion;
}
