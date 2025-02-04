# WRITING A POSTFX PASS USING GPU COMPUTE WITH THE RENDER PASS API - Amélie Heinrich

*Disclaimer: This article is intended for FX programmers working on the **Mnemen engine**. Proceed at your own discretion >.>*

---

## Introduction

Post-processing refers to the stage of rendering where visual effects and filters are applied to the image after the geometry has been rendered to the screen. These effects can range from subtle adjustments like **color grading** to more complex and dramatic effects such as **Screen Space Ambient Occlusion (SSAO)** or **Kuwahara filters**. 

In this tutorial, we’ll focus on writing a simple **box blur** filter using the **RenderPass API** of Mnemen's engine. This will help you understand the core concepts behind post-processing effects and how to implement them in the engine's framework.

---

## What is a Box Blur?

A **box blur** is a simple, yet effective blur technique. It works by averaging the color values of a pixel’s surrounding neighbors (North, East, South, West, Northwest, Northeast, Southwest, Southeast) and assigning the resulting average value back to the central pixel. 

This effect is commonly used in combination with other post-processing techniques, but for the purpose of this tutorial, we will focus solely on the box blur itself. This will allow us to cover the basic steps involved in creating a post-processing pass using the RenderPass API, while keeping things simple.

## Compute Shader

This simple compute shader utilizes **bindless architecture** to efficiently read and write to shader resources, allowing for more flexible and high-performance resource management.

```cpp
// Input settings passed from the CPU to the shader
struct TonemapperSettings
{
    // Index of the input resource (HDR texture)
    uint InputIndex;
    // Padding to ensure the struct is 16-byte aligned
    uint3 Pad;
};

// Push constant buffer for settings
ConstantBuffer<TonemapperSettings> Settings : register(b0);

// Define the number of threads per group (8x8 threads per group)
[numthreads(8, 8, 1)]
void CSMain(uint3 ThreadID : SV_DispatchThreadID)
{
    // Retrieve the HDR texture output resource
    RWTexture2D<float4> Output = ResourceDescriptorHeap[Settings.InputIndex];

    // Get the dimensions of the image to avoid out-of-bounds access
    uint Width, Height;
    Output.GetDimensions(Width, Height);

    // Prevent out-of-bounds access to the image data
    if (ThreadID.x < Width && ThreadID.y < Height)
    {
        // Define the size of the blur kernel (3x3 box blur)
        int blurRadius = 1;

        // Initialize a color accumulator to sum the neighboring pixel values
        float4 colorAccumulator = float4(0.0f, 0.0f, 0.0f, 0.0f);

        // Loop through the neighboring pixels within the blur kernel
        for (int dx = -blurRadius; dx <= blurRadius; ++dx)
        {
            for (int dy = -blurRadius; dy <= blurRadius; ++dy)
            {
                // Calculate the coordinates of the neighboring pixel
                int2 neighborCoord = int2(ThreadID.x + dx, ThreadID.y + dy);

                // Check if the neighbor is within valid bounds
                if (neighborCoord.x >= 0 && neighborCoord.x < Width && neighborCoord.y >= 0 && neighborCoord.y < Height)
                {
                    // Sample the color from the neighboring pixel
                    float4 neighborColor = Output[neighborCoord];

                    // Add the color to the accumulator
                    colorAccumulator += neighborColor;
                }
            }
        }

        // Compute the average color of the neighboring pixels
        float numSamples = (blurRadius * 2 + 1) * (blurRadius * 2 + 1);
        colorAccumulator /= numSamples;

        // Write the computed average color back to the output texture
        Output[ThreadID.xy] = colorAccumulator;
    }
}
```

## Class Definition

To begin, you need to declare the class that will manage your rendering code. This class will inherit from the `RenderPass` class, allowing you to override key functions to define the specific behavior of the pass. For this example, we’ll create a simple class for a **box blur** effect using a compute pipeline.

Here's a basic implementation of the `BoxBlur` class:

```cpp
class BoxBlur : public RenderPass
{
public:
    // Constructor: Takes a reference to the Render Hardware Interface (RHI)
    BoxBlur(RHI::Ref rhi);

    // Destructor: Default implementation
    ~BoxBlur() = default;

    // Bake function: Used to prepare resources before rendering (empty for this example)
    void Bake(Scene& scene) {}

    // Render function: The main rendering logic goes here
    void Render(const Frame& frame, Scene& scene) override;

    // UI function: Used to create ImGui settings in the Renderer window
    void UI(const Frame& frame) override;
};
```

### Explanation:

- Class Inheritance: The BoxBlur class inherits from the RenderPass base class. By overriding the Render and UI functions, you define how this render pass behaves in the pipeline.

- Constructor and Destructor:
    - The constructor takes a reference to the Render Hardware Interface (RHI::Ref rhi), which is typically used to interface with the GPU and manage render resources.
    - The destructor is defaulted, meaning no special cleanup is necessary beyond the default behavior.

- Render Function: The Render function contains the actual rendering logic. This is where you would call the compute shader to perform the box blur effect, set up resource bindings, and manage the execution of the rendering pass.

- UI Function: The UI function is used to create a settings interface in the ImGui renderer window. You can use this function to expose controls (such as blur strength, kernel size, or other options) that allow users to modify the effect during runtime.

## Class implementation

## Class Implementation

In this section, we will implement the **ColorGrading** class, which will manage the rendering pass for color grading (in this case, we use a simple compute shader). This implementation involves setting up the compute pipeline, managing resources, and dispatching the shader for execution.

```cpp
ColorGrading::ColorGrading(RHI::Ref rhi)
    : RenderPass(rhi)
{
    // Retrieve the compute shader from the asset manager
    Asset::Handle computeShader = AssetManager::Get("BoxBlur.hlsl", AssetType::Shader);
 
    // Create a root signature for the shader (using push constants for small data)
    auto signature = mRHI->CreateRootSignature({ RootType::PushConstant }, sizeof(int) * 4);

    // Create the compute pipeline with the shader and root signature
    mPipeline = mRHI->CreateComputePipeline(computeShader->Shader, signature);
}

void ColorGrading::Render(const Frame& frame, Scene& scene)
{
    // Retrieve the HDR color texture that we will write to
    auto color = RendererTools::Get("HDRColorBuffer");

    // Create the push constants structure for settings passed to the shader
    struct {
        int Output;           // The output texture we will write to (storage view)
        glm::vec3 Pad;       // Padding for alignment purposes (16-byte boundary)
    } PushConstants = {
        // Descriptor of the HDR texture to write to (storage view type)
        color->Descriptor(ViewType::Storage),
        glm::vec3(0)         // Padding vector
    };
    
    // Begin a command marker for easier GPU debugging
    frame.CommandBuffer->BeginMarker("Color Grading");

    // Transition the texture layout to notify the GPU that we are going to write to it
    frame.CommandBuffer->Barrier(color->Texture, ResourceLayout::Storage);

    // Set the compute pipeline for this rendering pass
    frame.CommandBuffer->SetComputePipeline(mPipeline);

    // Push the constants (settings) to the GPU
    frame.CommandBuffer->ComputePushConstants(&PushConstants, sizeof(PushConstants), 0);

    // Dispatch the compute shader (divide the image width and height by 8 since we're using 8x8 thread groups)
    frame.CommandBuffer->Dispatch(frame.Width / 8, frame.Height / 8, 1);

    // After the dispatch, change the texture layout back to 'Common' to indicate we're done writing
    frame.CommandBuffer->Barrier(color->Texture, ResourceLayout::Common);

    // End the command marker
    frame.CommandBuffer->EndMarker();
}

void ColorGrading::UI(const Frame& frame)
{
    // Add ImGui code to display the settings and allow modifications (if necessary)
    /// ...
}
```

### Explanation:

1. **Constructor (`ColorGrading::ColorGrading`)**:
   - **Asset Management**: The compute shader is retrieved from the asset manager (`AssetManager::Get`), using the `BoxBlur.hlsl` shader file. 
   - **Root Signature**: The root signature is created, which defines how the shader will interact with resources. In this case, we are using push constants to pass small amounts of data (e.g., the output texture).
   - **Pipeline Creation**: The compute pipeline is created using the shader and root signature. This pipeline will be used to execute the compute shader on the GPU.

2. **Render Function (`ColorGrading::Render`)**:
   - **Resource Access**: The HDR color texture is retrieved, and we create a structure (`PushConstants`) that will be sent to the shader as push constants. This structure contains the `Output` texture descriptor (the resource we’ll write to) and padding for proper alignment.
   - **Command Buffer**:
     - **Markers**: Markers are used for easier GPU debugging by labeling sections of GPU work.
     - **Barriers**: Barriers are issued to manage the transition of resource layouts (i.e., from a general-use layout to a storage layout to indicate that we intend to write to the texture).
   - **Compute Dispatch**: The compute shader is dispatched by specifying the number of thread groups. Since we use 8x8 thread groups, the image width and height are divided by 8 to determine the number of work groups needed.
   - **Final Barrier**: Another barrier is issued to indicate that we are done with the texture and it can now be accessed by other stages or passes in the pipeline.
   
3. **UI Function (`ColorGrading::UI`)**:
   - This function is a placeholder for adding UI elements using **ImGui** to adjust settings dynamically at runtime (e.g., adjusting the blur strength or color grading parameters).
   - The UI code would allow the user to modify parameters such as the texture being processed, blur radius, etc.

---

### Key Points:

- **Root Signature and Push Constants**: The shader is designed to receive small amounts of data (such as texture descriptors) using **push constants**, making it efficient for passing necessary settings to the compute shader.
- **Compute Pipeline**: The **compute pipeline** is set up and used to perform the image manipulation (color grading or blurring) on the HDR texture.
- **Resource Layouts**: Using **barriers** ensures proper synchronization of resource states. We specify a `Storage` layout when writing to textures and revert it to `Common` when done.
- **GPU Debugging**: Command markers and barriers help with debugging GPU workloads, making it easier to track the progress of each render pass.
