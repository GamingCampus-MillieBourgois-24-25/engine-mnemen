//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-03 06:22:48
//

#pragma once

#include <Core/Window.hpp>

#include <RHI/Device.hpp>
#include <RHI/DescriptorHeap.hpp>
#include <RHI/Queue.hpp>
#include <RHI/Fence.hpp>
#include <RHI/Surface.hpp>
#include <RHI/CommandBuffer.hpp>
#include <RHI/GraphicsPipeline.hpp>
#include <RHI/ComputePipeline.hpp>
#include <RHI/MeshPipeline.hpp>
#include <RHI/Buffer.hpp>
#include <RHI/Texture.hpp>
#include <RHI/Sampler.hpp>
#include <RHI/BLAS.hpp>
#include <RHI/TLAS.hpp>

/// @brief A structure representing the state of a single frame in the rendering pipeline.
/// @details Contains all necessary data for rendering a frame, including command buffers and backbuffer information.
struct Frame
{
    CommandBuffer::Ref CommandBuffer; ///< Command buffer associated with the current frame.
    Texture::Ref Backbuffer; ///< The backbuffer texture used for rendering.
    View::Ref BackbufferView; ///< The view to the backbuffer texture, defining how the backbuffer is accessed.
    UInt32 FrameIndex; ///< The index of the frame in the frame buffer queue (for synchronization).

    Int32 Width; ///< The width of the frame buffer (screen or backbuffer).
    Int32 Height; ///< The height of the frame buffer (screen or backbuffer).
};


/// @brief A class representing the Rendering Hardware Interface (RHI).
/// @details This class provides methods to interact with the GPU and execute rendering commands. It handles creating resources like buffers, pipelines, textures, and views, and submitting commands for execution.
class RHI
{
public:
    using Ref = Ref<RHI>; ///< Smart pointer type for the RHI class.

    /// @brief Constructor that initializes the RHI instance.
    /// @param window A reference to the window associated with the graphics context.
    RHI(::Ref<Window> window);

    /// @brief Destructor that cleans up the RHI resources.
    ~RHI();

    /// @brief Waits for all queued commands to finish before proceeding.
    void Wait();

    /// @brief Submits a vector of command buffers to the GPU for execution.
    /// @param buffers The command buffers to be submitted for execution.
    void Submit(const Vector<CommandBuffer::Ref> buffers);

    /// @brief Begins a new frame by preparing the necessary resources (command buffers, backbuffers).
    /// @return A `Frame` structure representing the state of the new frame.
    Frame Begin();

    /// @brief Ends the current frame and prepares for presentation or submission.
    void End();

    /// @brief Presents the current frame on screen.
    /// @param vsync If true, waits for vertical synchronization before presenting.
    void Present(bool vsync);

    /// @brief Creates a new root signature, which is used to define the layout of pipeline resources.
    /// @return A reference to the created `RootSignature` object.
    RootSignature::Ref CreateRootSignature();

    /// @brief Creates a new root signature with custom entries.
    /// @param entries A vector of root entries (e.g., constant buffers, textures) to be used in the root signature.
    /// @param pushConstantSize The size of the push constant, if used.
    /// @return A reference to the created `RootSignature` object.
    RootSignature::Ref CreateRootSignature(const Vector<RootType>& entries, UInt64 pushConstantSize = 0);

    /// @brief Creates a new graphics pipeline for rendering.
    /// @param specs The specifications for the graphics pipeline, including shaders, render targets, etc.
    /// @return A reference to the created `GraphicsPipeline` object.
    GraphicsPipeline::Ref CreateGraphicsPipeline(GraphicsPipelineSpecs& specs);

    /// @brief Creates a new mesh pipeline for handling mesh shaders.
    /// @param specs The specifications for the mesh pipeline.
    /// @return A reference to the created `MeshPipeline` object.
    MeshPipeline::Ref CreateMeshPipeline(GraphicsPipelineSpecs& specs);

    /// @brief Creates a new compute pipeline for GPU-based compute operations.
    /// @param shader The compute shader used in the pipeline.
    /// @param signature The root signature used in the compute pipeline.
    /// @return A reference to the created `ComputePipeline` object.
    ComputePipeline::Ref CreateComputePipeline(Shader shader, RootSignature::Ref signature);

    /// @brief Creates a new buffer resource.
    /// @param size The size of the buffer in bytes.
    /// @param stride The stride (size of one element) in the buffer.
    /// @param type The type of buffer (e.g., vertex buffer, index buffer).
    /// @param name The name to be assigned to the buffer.
    /// @return A reference to the created `Buffer` object.
    Buffer::Ref CreateBuffer(UInt64 size, UInt64 stride, BufferType type, const String& name = "Buffer");

    /// @brief Creates a new texture resource.
    /// @param desc The texture description (width, height, format, usage, etc.).
    /// @return A reference to the created `Texture` object.
    Texture::Ref CreateTexture(TextureDesc desc);

    /// @brief Creates a new view for a resource.
    /// @param resource The resource (e.g., buffer, texture) to create a view for.
    /// @param type The type of view (render target, shader resource, etc.).
    /// @param dimension The dimension of the resource (buffer, texture, etc.).
    /// @param format The format of the resource (e.g., RGBA8, depth format).
    /// @param mip The mip level to use for the view.
    /// @param depthSlice The depth slice to use (for 3D textures or slices).
    /// @return A reference to the created `View` object.
    View::Ref CreateView(::Ref<Resource> resource, ViewType type, ViewDimension dimension = ViewDimension::Texture, TextureFormat format = TextureFormat::Unknown, UInt64 mip = VIEW_ALL_MIPS, UInt64 depthSlice = 0);

    /// @brief Creates a new sampler for textures.
    /// @param address The texture addressing mode (e.g., wrap, clamp, mirror).
    /// @param filter The texture filter type (e.g., linear, anisotropic).
    /// @param mips If true, enables mipmaps in the sampler.
    /// @param anisotropyLevel The level of anisotropic filtering to apply.
    /// @param comparison If true, enables comparison for the sampler.
    /// @return A reference to the created `Sampler` object.
    Sampler::Ref CreateSampler(SamplerAddress address, SamplerFilter filter, bool mips = false, int anisotropyLevel = 4, bool comparison = false);

    /// @brief Creates a new Bottom-Level Acceleration Structure (BLAS).
    /// @param vertex The buffer containing vertex data.
    /// @param index The buffer containing index data.
    /// @param vtxCount The number of vertices in the BLAS.
    /// @param idxCount The number of indices in the BLAS.
    /// @param name The name assigned to the BLAS.
    /// @return A reference to the created `BLAS` object.
    BLAS::Ref CreateBLAS(Buffer::Ref vertex, Buffer::Ref index, UInt32 vtxCount, UInt32 idxCount, const String& name = "BLAS");

    /// @brief Creates a new Top-Level Acceleration Structure (TLAS).
    /// @param instanceBuffer The buffer containing the instances.
    /// @param numInstance The number of instances in the TLAS.
    /// @param name The name assigned to the TLAS.
    /// @return A reference to the created `TLAS` object.
    TLAS::Ref CreateTLAS(Buffer::Ref instanceBuffer, UInt32 numInstance, const String& name = "TLAS");

private:
    ::Ref<Window> mWindow = nullptr; ///< The window associated with the RHI instance.
    Device::Ref mDevice = nullptr; ///< The device object representing the GPU.
    Queue::Ref mGraphicsQueue = nullptr; ///< The command queue for submitting graphics commands.
    DescriptorHeaps mDescriptorHeaps; ///< The descriptor heaps for managing resource descriptors.
    Surface::Ref mSurface = nullptr; ///< The surface associated with the window for presenting frames.

    Fence::Ref mFrameFence = nullptr; ///< Fence for synchronizing frame submissions.
    Array<UInt64, FRAMES_IN_FLIGHT> mFrameValues; ///< Array of frame fence values for synchronization.
    Array<CommandBuffer::Ref, FRAMES_IN_FLIGHT> mCommandBuffers; ///< Command buffers for each frame.
    UInt32 mFrameIndex = 0; ///< The current frame index.

    DescriptorHeap::Descriptor mFontDescriptor; ///< Descriptor for the font texture or resource.
};

