//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-03 09:32:16
//

#pragma once

#include <RHI/Device.hpp>

/// @brief Defines descriptor heap types in Direct3D 12.
enum class DescriptorHeapType
{
    ShaderResource = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, ///< Heap for constant buffers, shader resources, and unordered access views.
    Sampler = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, ///< Heap for samplers.
    RenderTarget = D3D12_DESCRIPTOR_HEAP_TYPE_RTV, ///< Heap for render target views.
    DepthTarget = D3D12_DESCRIPTOR_HEAP_TYPE_DSV ///< Heap for depth stencil views.
};

/// @brief Manages a descriptor heap for GPU resources.
class DescriptorHeap
{
public:
    /// @brief Type alias for referencing
    using Ref = Ref<DescriptorHeap>;

    /// @brief Represents a single descriptor within a descriptor heap.
    struct Descriptor
    {
        bool Valid = false; ///< Whether the descriptor is valid.
        int Index = -1; ///< The index of the descriptor in the heap.
        DescriptorHeap* Parent = nullptr; ///< The parent descriptor heap.

        D3D12_CPU_DESCRIPTOR_HANDLE CPU; ///< CPU handle to the descriptor.
        D3D12_GPU_DESCRIPTOR_HANDLE GPU; ///< GPU handle to the descriptor (only if shader visible).

        /// @brief Default constructor.
        Descriptor() = default;

        /// @brief Creates a descriptor from a heap.
        /// @param heap The parent descriptor heap.
        /// @param index The index in the heap.
        Descriptor(DescriptorHeap* heap, int index)
            : Parent(heap), Index(index), Valid(true)
        {
            CPU = Parent->mHeap->GetCPUDescriptorHandleForHeapStart();
            CPU.ptr += index * Parent->mIncrementSize;

            if (Parent->mShaderVisible) {
                GPU = Parent->mHeap->GetGPUDescriptorHandleForHeapStart();
                GPU.ptr += index * Parent->mIncrementSize;
            }
        }
    };

    /// @brief Creates a descriptor heap.
    /// @param device The GPU device.
    /// @param type The type of descriptor heap.
    /// @param size The number of descriptors in the heap.
    DescriptorHeap(Device::Ref device, DescriptorHeapType type, UInt32 size);

    /// @brief Destroys the descriptor heap.
    ~DescriptorHeap();

    /// @brief Allocates a descriptor from the heap.
    /// @return The allocated descriptor.
    Descriptor Allocate();

    /// @brief Frees a descriptor, making it available for reuse.
    /// @param descriptor The descriptor to free.
    void Free(Descriptor& descriptor);

    /// @brief Retrieves the raw D3D12 descriptor heap.
    /// @return Pointer to the descriptor heap.
    ID3D12DescriptorHeap* GetHeap() { return mHeap; }

private:
    DescriptorHeapType mType; ///< The type of descriptor heap.
    ID3D12DescriptorHeap* mHeap = nullptr; ///< The D3D12 descriptor heap.

    int mIncrementSize = 0; ///< Descriptor size increment.
    UInt32 mHeapSize = 0; ///< Total number of descriptors in the heap.
    bool mShaderVisible = false; ///< Whether the heap is shader visible.
    Vector<bool> mLookupTable; ///< Tracks allocated/free descriptors.
};

/// @brief Collection of descriptor heaps mapped by type.
using DescriptorHeaps = UnorderedMap<DescriptorHeapType, DescriptorHeap::Ref>;

