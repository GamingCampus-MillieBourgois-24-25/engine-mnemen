//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-03 23:48:52
//

#pragma once

// #include <Asset/Image.hpp>

#include <RHI/RHI.hpp>
#include <RHI/CommandBuffer.hpp>
#include <RHI/Resource.hpp>
#include <RHI/Queue.hpp>
#include <RHI/Buffer.hpp>
#include <RHI/AccelerationStructure.hpp>
#include <RHI/RHI.hpp>
#include <Asset/Image.hpp>

#define KILOBYTES(s) s * 1024
#define MEGABYTES(s) KILOBYTES(s) * 1024
#define GIGABYTES(s) MEGABYTES(s) * 1024

/// @brief Class responsible for managing and batching upload requests to the GPU.
/// @details The `Uploader` class is used to efficiently handle data transfers from the CPU to the GPU. 
///          It provides mechanisms for enqueuing texture uploads, buffer uploads, and acceleration structure builds, 
///          batching requests to ensure optimal data transfer performance and usage of GPU resources.
class Uploader
{
public:
    /// @brief Initializes the uploader with the necessary resources and queues.
    /// @param rhi Pointer to the rendering hardware interface.
    /// @param device The device used for GPU operations.
    /// @param heaps The descriptor heaps used for resource management.
    /// @param queue The queue used to enqueue the upload operations.
    static void Init(RHI* rhi, Device::Ref device, DescriptorHeaps heaps, Queue::Ref queue);

    /// @brief Enqueues a texture upload request from a raw buffer.
    /// @param buffer A vector of UInt8 representing the texture data.
    /// @param texture The resource to which the texture is being uploaded.
    static void EnqueueTextureUpload(Vector<UInt8> buffer, Ref<Resource> texture);

    /// @brief Enqueues a texture upload request from an image.
    /// @param image The image containing texture data to upload.
    /// @param buffer The resource to which the texture is being uploaded.
    static void EnqueueTextureUpload(Image image, Ref<Resource> buffer);

    /// @brief Enqueues a buffer upload request.
    /// @param data Pointer to the data to be uploaded.
    /// @param size Size of the data to upload.
    /// @param buffer The buffer to which the data is being uploaded.
    static void EnqueueBufferUpload(void* data, UInt64 size, Ref<Resource> buffer);

    /// @brief Enqueues a request to build an acceleration structure.
    /// @param as Reference to the acceleration structure to be built.
    static void EnqueueAccelerationStructureBuild(Ref<AccelerationStructure> as);

    /// @brief Flushes all pending upload requests to the GPU.
    /// @details This function will process all enqueued upload requests, ensuring that they are transferred to the GPU.
    static void Flush();

    /// @brief Clears all enqueued upload requests.
    /// @details This function clears all pending upload requests, effectively resetting the state of the uploader.
    static void ClearRequests();

private:
    /// @brief The maximum upload batch size (512 MB).
    /// @details Uploads exceeding this size will be split into multiple batches for processing.
    static constexpr UInt64 MAX_UPLOAD_BATCH_SIZE = MEGABYTES(512);

    /// @brief Enum representing the type of upload request.
    enum class UploadRequestType
    {
        BufferCPUToGPU, ///< Upload request for buffer data transfer from CPU to GPU.
        TextureToGPU,   ///< Upload request for texture data transfer to GPU.
        BuildAS         ///< Upload request for building an acceleration structure.
    };

    /// @brief Structure representing a single upload request.
    struct UploadRequest
    {
        UploadRequestType Type; ///< The type of upload request.

        Ref<Resource> Resource = nullptr; ///< Resource associated with the request (e.g., texture or buffer).
        Ref<Buffer> StagingBuffer = nullptr; ///< The staging buffer used for the transfer.
        Ref<AccelerationStructure> Acceleration = nullptr; ///< Acceleration structure associated with build requests.
    };

    /// @brief Structure storing uploader's state data.
    static struct Data
    {
        RHI* Rhi = nullptr; ///< Pointer to the rendering hardware interface.
        DescriptorHeaps Heaps; ///< Descriptor heaps for resource management.
        Device::Ref Device = nullptr; ///< The device used for GPU operations.
        Queue::Ref UploadQueue = nullptr; ///< The queue used to enqueue the upload operations.
        CommandBuffer::Ref CmdBuffer = nullptr; ///< Command buffer used for batching upload operations.
        Vector<UploadRequest> Requests; ///< List of enqueued upload requests.

        int TextureRequests = 0; ///< Counter for texture upload requests.
        int BufferRequests = 0; ///< Counter for buffer upload requests.
        int ASRequests = 0; ///< Counter for acceleration structure build requests.
        int UploadBatchSize = 0; ///< Current upload batch size.
    } sData;
};

