//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-03 11:52:45
//

#pragma once

#include <RHI/Queue.hpp>
#include <RHI/View.hpp>
#include <RHI/GraphicsPipeline.hpp>
#include <RHI/MeshPipeline.hpp>
#include <RHI/ComputePipeline.hpp>
#include <RHI/Resource.hpp>
#include <RHI/Buffer.hpp>
#include <RHI/AccelerationStructure.hpp>
#include <RHI/TLAS.hpp>

/// @brief Enum for different primitive topologies.
enum class Topology
{
    /// @brief List of lines.
    LineList = D3D_PRIMITIVE_TOPOLOGY_LINELIST,
    /// @brief Strip of connected lines.
    LineStrip = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP,
    /// @brief List of points.
    PointList = D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
    /// @brief List of triangles.
    TriangleList = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
    /// @brief Strip of connected triangles.
    TriangleStrip = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
};

/// @brief Handles GPU command recording and execution.
///
/// CommandBuffer is responsible for recording and executing GPU commands 
/// such as rendering, resource transitions, and compute dispatches.
/// It supports graphics, compute, and mesh pipelines while allowing 
/// for efficient resource management through barriers and buffer copies.
class CommandBuffer
{
public:
    /// @brief Type alias for referencing
    using Ref = Ref<CommandBuffer>;

    /// @brief Creates a new command buffer.
    /// @param device The device used to allocate the command buffer.
    /// @param queue The queue where the command buffer will be submitted.
    /// @param heaps The descriptor heaps used for resource binding.
    /// @param singleTime Whether this command buffer is for single-time use.
    ///
    /// A command buffer is used to record commands before execution.
    /// If singleTime is true, it is expected to be reset after execution.
    CommandBuffer(Device::Ref device, Queue::Ref queue, DescriptorHeaps heaps, bool singleTime = false);

    /// @brief Destroys the command buffer and releases resources.
    ~CommandBuffer();

    /// @brief Begins command recording.
    ///
    /// This must be called before issuing any draw, compute, or copy commands.
    void Begin();

    /// @brief Ends command recording.
    ///
    /// This must be called before submitting the command buffer for execution.
    void End();

    /// @brief Marks the beginning of a debug section.
    /// @param name The name of the debug marker.
    ///
    /// This helps with GPU debugging and profiling by adding a named section.
    void BeginMarker(const String& name);

    /// @brief Marks the end of a debug section.
    ///
    /// Must be called after BeginMarker to properly close the section.
    void EndMarker();

    /// @brief Inserts a UAV (Unordered Access View) barrier.
    /// @param resource The resource requiring synchronization.
    ///
    /// UAV barriers ensure that previous unordered accesses to a resource
    /// are completed before new ones start.
    void UAVBarrier(::Ref<Resource> resource);

    /// @brief Inserts a resource barrier to transition resource states.
    /// @param resource The resource to transition.
    /// @param layout The new layout to transition to.
    /// @param mip The mip level to transition, defaults to all.
    ///
    /// This ensures that GPU operations respect proper resource states.
    void Barrier(::Ref<Resource> resource, ResourceLayout layout, UInt32 mip = VIEW_ALL_MIPS);
    
    /// @brief Sets the viewport for rendering.
    /// @param x The X position of the viewport.
    /// @param y The Y position of the viewport.
    /// @param width The width of the viewport.
    /// @param height The height of the viewport.
    ///
    /// Defines the region of the screen where rendering will occur.
    void SetViewport(float x, float y, float width, float height);

    /// @brief Sets the primitive topology for drawing.
    /// @param topology The topology type (triangles, lines, points, etc.).
    ///
    /// Determines how vertex data is interpreted when rendering.
    void SetTopology(Topology topology);

    /// @brief Binds a graphics pipeline for rendering.
    /// @param pipeline The graphics pipeline to bind.
    ///
    /// This sets up shaders, rasterization, and blending states.
    void SetGraphicsPipeline(GraphicsPipeline::Ref pipeline);

    /// @brief Binds a mesh shading pipeline.
    /// @param pipeline The mesh pipeline to bind.
    ///
    /// Used when rendering with mesh shaders instead of traditional vertex shaders.
    void SetMeshPipeline(MeshPipeline::Ref pipeline);

    /// @brief Binds a compute pipeline.
    /// @param pipeline The compute pipeline to bind.
    ///
    /// Sets up a compute pipeline for dispatching compute workloads.
    void SetComputePipeline(ComputePipeline::Ref pipeline);

    /// @brief Sets the render targets and depth buffer.
    /// @param targets The color render targets.
    /// @param depth The depth buffer target.
    ///
    /// These define the surfaces where rendered images are stored.
    void SetRenderTargets(const Vector<View::Ref> targets, View::Ref depth);

    /// @brief Binds a vertex buffer.
    /// @param buffer The vertex buffer to bind.
    ///
    /// Defines the vertex data to be used in rendering.
    void SetVertexBuffer(Buffer::Ref buffer);

    /// @brief Binds an index buffer.
    /// @param buffer The index buffer to bind.
    ///
    /// Used for indexed rendering to optimize memory usage.
    void SetIndexBuffer(Buffer::Ref buffer);

    /// @brief Pushes constants to the graphics pipeline.
    /// @param data Pointer to the data.
    /// @param size The size of the data.
    /// @param index The constant buffer index.
    ///
    /// This is used for small, frequently updated data without creating buffers.
    void GraphicsPushConstants(const void *data, UInt32 size, int index);

    /// @brief Pushes constants to the compute pipeline.
    /// @param data Pointer to the data.
    /// @param size The size of the data.
    /// @param index The constant buffer index.
    ///
    /// Similar to GraphicsPushConstants but used in compute shaders.
    void ComputePushConstants(const void *data, UInt32 size, int index);

    /// @brief Clears the depth buffer.
    /// @param view The depth buffer view.
    ///
    /// Resets depth values to ensure correct depth testing.
    void ClearDepth(View::Ref view);

    /// @brief Clears a render target with a specific color.
    /// @param view The render target view.
    /// @param r The red component.
    /// @param g The green component.
    /// @param b The blue component.
    ///
    /// This fills the render target with a uniform color.
    void ClearRenderTarget(View::Ref view, float r, float g, float b);

    /// @brief Draws non-indexed geometry.
    /// @param vertexCount Number of vertices to draw.
    ///
    /// Uses the currently bound vertex buffer.
    void Draw(int vertexCount);

    /// @brief Draws indexed geometry.
    /// @param indexCount Number of indices to draw.
    ///
    /// Uses the currently bound index and vertex buffers.
    void DrawIndexed(int indexCount);

    /// @brief Dispatches a mesh shader workload.
    /// @param meshletCount Number of meshlets to dispatch.
    /// @param triangleCount For debug only, used to increment the number of triangles in profiler.
    ///
    /// Used in mesh shading for modern GPU rendering techniques.
    void DispatchMesh(int meshletCount, int triangleCount = 0);

    /// @brief Dispatches a compute shader workload.
    /// @param x Number of thread groups in the X direction.
    /// @param y Number of thread groups in the Y direction.
    /// @param z Number of thread groups in the Z direction.
    ///
    /// Executes a compute shader with the given workgroup size.
    void Dispatch(int x, int y, int z);

    /// @brief Copies a texture to another texture.
    /// @param dst The destination texture.
    /// @param src The source texture.
    void CopyTextureToTexture(::Ref<Resource> dst, ::Ref<Resource> src) {
        CopyBufferToBuffer(dst, src);
    }

    /// @brief Copies a buffer to another buffer.
    /// @param dst The destination buffer.
    /// @param src The source buffer.
    void CopyBufferToBuffer(::Ref<Resource> dst, ::Ref<Resource> src);

    /// @brief Copies a buffer to a texture.
    /// @param dst The destination texture.
    /// @param src The source buffer.
    void CopyBufferToTexture(::Ref<Resource> dst, ::Ref<Resource> src);

    /// @brief Updates the TLAS (Top Level Acceleration Structure).
    /// @param tlas The TLAS to update.
    /// @param instanceBuffer The buffer containing instance data.
    /// @param numInstances The number of instances.
    void UpdateTLAS(TLAS::Ref tlas, Buffer::Ref instanceBuffer, int numInstances);

    /// @brief Builds a ray tracing acceleration structure.
    /// @param as The acceleration structure to build.
    void BuildAccelerationStructure(::Ref<AccelerationStructure> as);

    /// @brief Begins GUI rendering.
    /// @param width The GUI rendering width.
    /// @param height The GUI rendering height.
    ///
    /// Prepares the command buffer for UI rendering.
    void BeginGUI(int width, int height);

    /// @brief Ends GUI rendering.
    void EndGUI();

    /// @brief Gets the internal Direct3D command list.
    ID3D12GraphicsCommandList10* GetList() { return mList; }

    /// @brief Converts the command buffer to a raw Direct3D command list.
    operator ID3D12CommandList*() { return mList; }

private:
    bool mSingleTime; ///< Indicates whether the command buffer is a single-use buffer.
    Device::Ref mDevice; ///< Reference to the device that owns this command buffer.
    Queue::Ref mParentQueue; ///< Reference to the parent queue this command buffer is associated with.
    DescriptorHeaps mHeaps; ///< Descriptor heap manager for resource binding.
    ID3D12CommandAllocator* mAllocator; ///< D3D12 command allocator used for memory management of command lists.
    ID3D12GraphicsCommandList10* mList; ///< D3D12 graphics command list used to record commands.
};

