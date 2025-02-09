//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-03 10:38:04
//

#pragma once

#include <RHI/Device.hpp>
#include <Utility/UUID.hpp>

/// @brief Represents the possible resource layouts/states in Direct3D 12.
enum class ResourceLayout
{
    /// @brief Common state for a resource. It can be used by any operation.
    Common = D3D12_RESOURCE_STATE_COMMON,
    /// @brief Shader resource state for resources that can be used in shaders.
    Shader = D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE,
    /// @brief Storage state for resources that can be accessed as unordered access.
    Storage = D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
    /// @brief Depth write state for resources used as a depth target.
    DepthWrite = D3D12_RESOURCE_STATE_DEPTH_WRITE,
    /// @brief Depth read state for resources used as a depth source.
    DepthRead = D3D12_RESOURCE_STATE_DEPTH_READ,
    /// @brief Render target state for resources that are used as render targets.
    ColorWrite = D3D12_RESOURCE_STATE_RENDER_TARGET,
    /// @brief Copy source state for resources used as a source in copy operations.
    CopySource = D3D12_RESOURCE_STATE_COPY_SOURCE,
    /// @brief Copy destination state for resources used as a destination in copy operations.
    CopyDest = D3D12_RESOURCE_STATE_COPY_DEST,
    /// @brief Present state for resources that are presented to the screen.
    Present = D3D12_RESOURCE_STATE_PRESENT,
    /// @brief Generic read state for resources that can be read by the GPU.
    GenericRead = D3D12_RESOURCE_STATE_GENERIC_READ,
    /// @brief Vertex and constant buffer state for resources used as vertex buffers or constant buffers.
    Vertex = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
    /// @brief Ray tracing acceleration structure state for resources used in ray tracing operations.
    AccelerationStructure = D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE,
    /// @brief Non-pixel shader resource state for resources used in non-pixel shaders.
    NonPixelShader = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE
};

/// @brief Tags for categorizing resources.
enum class ResourceTag
{
    /// @brief Tag for model geometry resources.
    ModelGeometry,
    /// @brief Tag for model texture resources.
    ModelTexture,
    /// @brief Tag for shader pass input/output resources.
    ShaderPassIO,
    /// @brief Tag for shader pass-specific resources.
    ShaderPassResource,
    /// @brief Tag for GPU readback resources.
    GPUReadback
};

/// @brief Base class representing a resource in Direct3D 12.
class Resource
{
public:
    /// @brief Constructs a new resource.
    /// @param device The device that will manage the resource.
    Resource(Device::Ref device);
    /// @brief Destructor for cleaning up the resource.
    ~Resource();

    /// @brief Sets the name of the resource.
    /// @param string The name to assign to the resource.
    void SetName(const String& string);

    /// @brief Assigns a tag to the resource.
    /// @param tag The tag to assign.
    void Tag(ResourceTag tag);

    /// @brief Returns the size of the resource in bytes.
    UInt64 GetSize() const { return mSize; }

    /// @brief Returns the stride of the resource in bytes (e.g., for buffers).
    UInt64 GetStride() const { return mStride; }

    /// @brief Gets the underlying D3D12 resource object.
    /// @return The D3D12 resource pointer.
    ID3D12Resource* GetResource() const { return mResource; }

    /// @brief Gets the GPU virtual address of the resource.
    /// @return The GPU virtual address.
    UInt64 GetAddress() { return mResource->GetGPUVirtualAddress(); }

    /// @brief Gets the name of the resource.
    /// @return The name of the resource.
    String GetName() { return mName; }

    /// @brief Gets the current layout of the resource.
    /// @return The resource's layout.
    ResourceLayout GetLayout() { return mLayout; }

    /// @brief Sets the layout of the resource.
    /// @param layout The new layout to set.
    void SetLayout(ResourceLayout layout) { mLayout = layout; }

protected:
    Device::Ref mParentDevice; ///< Device that created and owns the resource.
    ID3D12Resource* mResource = nullptr; ///< The Direct3D 12 resource object.

    bool mShouldFree = false; ///< Flag indicating whether the resource should be freed.
    UInt64 mSize; ///< Size of the resource in bytes.
    UInt64 mStride; ///< Stride of the resource, if applicable (e.g., for buffers).
    ResourceLayout mLayout; ///< Current layout of the resource.
    String mName; ///< Name of the resource.

    Vector<ResourceTag> mTags; ///< Tags associated with the resource.

    /// @brief Creates a new resource.
    /// @param heapProps Properties for the heap where the resource will reside.
    /// @param resourceDesc Description of the resource.
    /// @param state Initial resource state.
    void CreateResource(D3D12_HEAP_PROPERTIES* heapProps, D3D12_RESOURCE_DESC* resourceDesc, D3D12_RESOURCE_STATES state);

private:
    UInt64 mAllocSize; ///< The total allocated size of the resource (may include padding).
    Util::UUID mUUID;
};
