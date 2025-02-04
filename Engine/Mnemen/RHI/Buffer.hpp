//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-03 23:10:09
//

#pragma once

#include <RHI/Device.hpp>
#include <RHI/DescriptorHeap.hpp>
#include <RHI/Resource.hpp>

/// @brief Enum for different GPU buffer types.
enum class BufferType
{
    /// @brief Buffer for vertex data.
    Vertex,
    /// @brief Buffer for index data.
    Index,
    /// @brief Buffer for constant data.
    Constant,
    /// @brief General-purpose buffer for compute shaders.
    Storage,
    /// @brief Buffer for copying data between resources.
    Copy,
    /// @brief Buffer for reading data back to the CPU.
    Readback,
    /// @brief Buffer for ray tracing acceleration structures.
    AccelerationStructure
};

/// @brief A class representing a GPU buffer used for various types of data storage and resource access.
///
/// A `Buffer` class abstracts GPU buffers used in Direct3D 12 for various purposes like storing vertex data, 
/// index data, constants, and more. It supports building views for constant buffer (CBV), unordered access 
/// (UAV), and shader resource (SRV) types. Buffers are essential components in rendering pipelines, compute 
/// shaders, and ray tracing applications.
class Buffer : public Resource
{
public:
    /// @brief Type alias for buffer pointers
    using Ref = Ref<Buffer>;

    /// @brief Constructs a Buffer with the specified device, heaps, size, stride, buffer type, and an optional name.
    /// 
    /// The constructor initializes a GPU buffer of the given size and type. It also sets up descriptor heaps
    /// and prepares the buffer for use in Direct3D 12 operations such as vertex processing, constant buffer access,
    /// and unordered access.
    /// 
    /// @param device The device reference used for resource creation.
    /// @param heaps The descriptor heaps used to manage the buffer's views (CBV, UAV, SRV).
    /// @param size The size of the buffer in bytes.
    /// @param stride The stride (or size) of an individual element in the buffer (e.g., for vertex data).
    /// @param type The type of buffer being created (e.g., vertex, index, constant, storage, etc.).
    /// @param name The optional name for the buffer (default is "Buffer").
    Buffer(Device::Ref device, DescriptorHeaps heaps, UInt64 size, UInt64 stride, BufferType type, const String& name = "Buffer");

    /// @brief Destructor for the Buffer class.
    ///
    /// The destructor cleans up any resources associated with the buffer, including descriptor heap entries 
    /// and Direct3D 12-specific memory resources.
    ~Buffer();

    /// @brief Builds a Constant Buffer View (CBV) for this buffer.
    /// 
    /// This method prepares the buffer to be accessed as a constant buffer, allowing the GPU to read its contents.
    void BuildCBV();

    /// @brief Builds an Unordered Access View (UAV) for this buffer.
    /// 
    /// This method prepares the buffer to be accessed as an unordered access resource, which can be read from and written to.
    void BuildUAV();

    /// @brief Builds a Shader Resource View (SRV) for this buffer.
    /// 
    /// This method prepares the buffer to be accessed as a shader resource, allowing shaders to read its contents.
    void BuildSRV();

    /// @brief Maps a portion of the buffer to a memory space for direct access.
    /// 
    /// This method maps a region of the buffer to CPU accessible memory, allowing read/write operations to take place
    /// between the CPU and the GPU.
    /// 
    /// @param start The starting byte offset of the region to map.
    /// @param end The ending byte offset of the region to map.
    /// @param data A pointer that will hold the mapped memory data.
    void Map(int start, int end, void **data);

    /// @brief Unmaps a previously mapped buffer region.
    /// 
    /// This method unmaps the buffer after it has been mapped for CPU access. Unmapping ensures that the GPU
    /// can safely access the buffer again.
    /// 
    /// @param start The starting byte offset of the region to unmap.
    /// @param end The ending byte offset of the region to unmap.
    void Unmap(int start, int end);

    /// @brief Copies data into a mapped buffer region.
    /// 
    /// This method allows copying data into a mapped portion of the buffer.
    /// 
    /// @param data A pointer to the data to copy into the buffer.
    /// @param size The size of the data being copied.
    void CopyMapped(void *data, UInt64 size);

    /// @brief Returns the index of the Constant Buffer View (CBV) descriptor for this buffer.
    /// 
    /// @return The index of the Constant Buffer View (CBV).
    Int32 CBV() { return mCBV.Index; }

    /// @brief Returns the index of the Unordered Access View (UAV) descriptor for this buffer.
    /// 
    /// @return The index of the Unordered Access View (UAV).
    Int32 UAV() { return mUAV.Index; }

    /// @brief Returns the index of the Shader Resource View (SRV) descriptor for this buffer.
    /// 
    /// @return The index of the Shader Resource View (SRV).
    Int32 SRV() { return mSRV.Index; }

private:
    friend class CommandBuffer;

    DescriptorHeaps mHeaps; ///< Descriptor heaps used for managing views like CBV, UAV, and SRV.
    BufferType mType; ///< The type of buffer (e.g., vertex, index, constant, etc.).

    DescriptorHeap::Descriptor mCBV = {}; ///< Constant Buffer View descriptor.
    DescriptorHeap::Descriptor mSRV = {}; ///< Shader Resource View descriptor.
    DescriptorHeap::Descriptor mUAV = {}; ///< Unordered Access View descriptor.

    D3D12_VERTEX_BUFFER_VIEW mVBV; ///< Vertex Buffer View, used for vertex buffers.
    D3D12_INDEX_BUFFER_VIEW mIBV; ///< Index Buffer View, used for index buffers.
};

