# HLSL GUIDE - Amélie Heinrich

*Disclaimer: This guide is intended for FX programmers working on the **Mnemen engine**. Read at your own discretion >.>*

---

## Table of Contents

- [Types](#types)
- [Bindless Resources](#bindless-resources)
- [Sampling from Textures](#sampling-from-textures)
- [Writing to Textures](#writing-to-textures)
- [Additional Resources](#additional-resources)

---

## Types

In HLSL, you will often work with different types of data structures like vectors, matrices, and resources (textures, buffers). Here's a breakdown of common types and how they are used:

```cpp
// Basic vector types
float   // A single floating-point number
float2  // A vector containing 2 floats (e.g., for 2D positions or UVs)
float3  // A vector containing 3 floats (e.g., for positions, directions, or colors)
float4  // A vector containing 4 floats (e.g., for RGBA colors, or homogeneous coordinates)

// You can also use vectors with other types such as integers or unsigned integers
int2    // A 2D vector of integers
uint3   // A 3D vector of unsigned integers

// Matrices are essential for transformations (scaling, rotation, translation) in 3D space. HLSL uses 4x4 matrices frequently for transforming points or vectors in world space, camera space, or projection space.
// 4x4 matrix (used for transformations like World * View * Projection)
float4x4  // 4x4 matrix
float3x3  // 3x3 matrix (used for rotation and scaling)
// Matrix storage order
row_major float4x4  // Row-major matrix layout (default)
column_major float4x4  // Column-major matrix layout (used in some engines)

// Resources in HLSL are objects like textures or buffers that hold data. You can access and manipulate them in your shaders.
// Texture types
Texture2D<T>         // A 2D texture that contains data of type T (e.g., Texture2D<float4> for RGBA textures)
RWTexture2D<T>       // A 2D texture with read/write access (e.g., used in compute shaders)
ConstantBuffer<T>    // A buffer with read/write access from the CPU and read access from the GPU (small data)
StructuredBuffer<T>  // A buffer with read access from the GPU and read/write access from the CPU (larger data)
RWStructuredBuffer<T> // A structured buffer with both read/write access

// Sampler
SamplerState         // Used for sampling textures, defines how textures are filtered
```

## Bindless resources

Since Mnemen makes use of a bindless architecture, here’s how you can retrieve a resource using a descriptor index in HLSL:
```cpp
// The texture and sampler indices are provided by the shader (usually via push constants)
int textureIndex, samplerIndex; 

// Use these indices to retrieve the resources from the descriptor heap
Texture2D<float> myTestTexture = ResourceDescriptorHeap[textureIndex];
SamplerState myTestSampler = SamplerDescriptorHeap[samplerIndex];
```

## Sampling from textures

To get color data from textures, there are two main ways of sampling: using a **sampler** or using the **pixel position**.
```cpp
// Method 1: Sampling with a sampler
Texture2D<float4> myTexture;
float2 uvCoordinates; // UV coordinates are between [0, 1]
SamplerState mySampler;

// Get the color from the texture at the specified UV coordinate
float4 currentColor = myTexture.Sample(mySampler, uvCoordinates);

// Method 2: Sampling with the pixel position
uint2 pixelPosition; // Pixel position is in absolute pixel coordinates (not UV)
float4 currentColor = myTexture.Load(pixelPosition);
```

## Writing to textures

You can write to textures using a RWTexture2D. Here’s how you write a color value to a specific pixel:
```cpp
// RWTexture for writing data to a texture
RWTexture2D<float4> writeTexture;
uint2 pixelPosition;
float4 colorToWrite;

// Write the color to the texture at the given pixel position
writeTexture[pixelPosition] = colorToWrite;
```

## Links

- https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl-reference
