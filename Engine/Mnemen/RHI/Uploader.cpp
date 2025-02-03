//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-03 23:55:27
//

#include <RHI/Uploader.hpp>
#include <Core/Logger.hpp>
#include <Core/Timer.hpp>

Uploader::Data Uploader::sData;

void Uploader::Init(RHI* rhi, Device::Ref device, DescriptorHeaps heaps, Queue::Ref queue)
{
    sData.Rhi = rhi;
    sData.Device = device;
    sData.Heaps = heaps;
    sData.UploadQueue = queue;
    sData.CmdBuffer = nullptr;
    sData.BufferRequests = 0;
    sData.TextureRequests = 0;
    sData.UploadBatchSize = 0;
}

void Uploader::EnqueueTextureUpload(Vector<UInt8> buffer, Ref<Resource> texture)
{
    sData.TextureRequests++;

    UploadRequest request = {};
    request.Type = UploadRequestType::TextureToGPU;
    request.Resource = texture;
    
    D3D12_RESOURCE_DESC desc = texture->GetResource()->GetDesc();
    std::vector<D3D12_PLACED_SUBRESOURCE_FOOTPRINT> footprints(desc.MipLevels);
    std::vector<UInt32> numRows(desc.MipLevels);
    std::vector<UInt64> rowSizes(desc.MipLevels);
    uint64_t totalSize = 0;

    sData.Device->GetDevice()->GetCopyableFootprints(&desc, 0, desc.MipLevels, 0, footprints.data(), numRows.data(), rowSizes.data(), &totalSize);
    request.StagingBuffer = MakeRef<Buffer>(sData.Device, sData.Heaps, totalSize, 0, BufferType::Copy, "Staging Buffer " + texture->GetName());

    UInt8 *pixels = reinterpret_cast<UInt8*>(buffer.data());    
    UInt8* mapped;
    request.StagingBuffer->Map(0, 0, (void**)&mapped);
    for (int i = 0; i < desc.MipLevels; i++) {
        for (int j = 0; j < numRows[i]; j++) {
            memcpy(mapped, pixels, rowSizes[i]);
            mapped += footprints[i].Footprint.RowPitch;
            pixels += rowSizes[i];
        }
    }
    request.StagingBuffer->Unmap(0, 0);

    sData.Requests.push_back(request);

    sData.UploadBatchSize += totalSize;
    if (sData.UploadBatchSize >= MAX_UPLOAD_BATCH_SIZE)
        Flush();
}

void Uploader::EnqueueTextureUpload(Image image, Ref<Resource> buffer)
{
    sData.TextureRequests++;

    UploadRequest request = {};
    request.Type = UploadRequestType::TextureToGPU;
    request.Resource = buffer;
    
    D3D12_RESOURCE_DESC desc = buffer->GetResource()->GetDesc();
    std::vector<D3D12_PLACED_SUBRESOURCE_FOOTPRINT> footprints(desc.MipLevels);
    std::vector<UInt32> numRows(desc.MipLevels);
    std::vector<UInt64> rowSizes(desc.MipLevels);
    uint64_t totalSize = 0;

    sData.Device->GetDevice()->GetCopyableFootprints(&desc, 0, desc.MipLevels, 0, footprints.data(), numRows.data(), rowSizes.data(), &totalSize);
    request.StagingBuffer = MakeRef<Buffer>(sData.Device, sData.Heaps, totalSize, 0, BufferType::Copy, "Staging Buffer " + buffer->GetName());

    UInt8 *pixels = reinterpret_cast<UInt8*>(image.Pixels.data());    
    UInt8* mapped;
    request.StagingBuffer->Map(0, 0, (void**)&mapped);
    for (int i = 0; i < desc.MipLevels; i++) {
        for (int j = 0; j < numRows[i]; j++) {
            memcpy(mapped, pixels, rowSizes[i]);
            mapped += footprints[i].Footprint.RowPitch;
            pixels += rowSizes[i];
        }
    }
    request.StagingBuffer->Unmap(0, 0);

    sData.Requests.push_back(request);

    sData.UploadBatchSize += totalSize;
    if (sData.UploadBatchSize >= MAX_UPLOAD_BATCH_SIZE)
        Flush();
}

void Uploader::EnqueueBufferUpload(void* data, UInt64 size, Ref<Resource> buffer)
{
    sData.BufferRequests++;

    UploadRequest request = {};
    request.Type = UploadRequestType::BufferCPUToGPU;
    request.Resource = buffer;
    request.StagingBuffer = MakeRef<Buffer>(sData.Device, sData.Heaps, size, 0, BufferType::Copy, "Staging Buffer " + buffer->GetName());

    void* mapped;
    request.StagingBuffer->Map(0, 0, &mapped);
    memcpy(mapped, data, size);
    request.StagingBuffer->Unmap(0, 0);

    sData.Requests.push_back(request);

    sData.UploadBatchSize += size;
    if (sData.UploadBatchSize >= MAX_UPLOAD_BATCH_SIZE)
        Flush();
}

void Uploader::EnqueueAccelerationStructureBuild(Ref<AccelerationStructure> as)
{
    sData.ASRequests++;

    UploadRequest request = {};
    request.Type = UploadRequestType::BuildAS;
    request.Acceleration = as;

    sData.Requests.push_back(request);
}

void Uploader::Flush()
{
    if (sData.Requests.empty())
        return;

    sData.CmdBuffer = MakeRef<CommandBuffer>(sData.Device, sData.UploadQueue, sData.Heaps, true);
    sData.CmdBuffer->Begin();

    LOG_INFO("Flushing {0} upload requests ({1} buffer uploads, {2} texture uploads, {3} acceleration structure builds)", sData.Requests.size(), sData.BufferRequests, sData.TextureRequests, sData.ASRequests);
    for (auto request : sData.Requests) {        
        switch (request.Type) {
            case UploadRequestType::BufferCPUToGPU: {
                sData.CmdBuffer->Barrier(request.Resource, ResourceLayout::CopyDest);
                sData.CmdBuffer->Barrier(request.StagingBuffer, ResourceLayout::CopySource);
                sData.CmdBuffer->CopyBufferToBuffer(request.Resource, request.StagingBuffer);
                sData.CmdBuffer->Barrier(request.StagingBuffer, ResourceLayout::Common);
                sData.CmdBuffer->Barrier(request.Resource, ResourceLayout::NonPixelShader);
                break;
            }
            case UploadRequestType::TextureToGPU: {
                sData.CmdBuffer->Barrier(request.Resource, ResourceLayout::CopyDest);
                sData.CmdBuffer->Barrier(request.StagingBuffer, ResourceLayout::CopySource);
                sData.CmdBuffer->CopyBufferToTexture(request.Resource, request.StagingBuffer);
                sData.CmdBuffer->Barrier(request.StagingBuffer, ResourceLayout::Common);
                sData.CmdBuffer->Barrier(request.Resource, ResourceLayout::Shader);
                break;
            }
            case UploadRequestType::BuildAS: {
                // sData.CmdBuffer->UAVBarrier(request.Acceleration);
                // sData.CmdBuffer->UAVBarrier(request.Acceleration->GetScratch());
                // sData.CmdBuffer->BuildAccelerationStructure(request.Acceleration);
                // sData.CmdBuffer->UAVBarrier(request.Acceleration);
                // sData.CmdBuffer->UAVBarrier(request.Acceleration->GetScratch());
                break;
            }
        }
    }

    sData.CmdBuffer->End();
    sData.UploadQueue->Submit({ sData.CmdBuffer });

    // Wait and clear
    sData.Rhi->Wait();
    ClearRequests();
}

void Uploader::ClearRequests()
{
    sData.UploadBatchSize = 0;
    sData.BufferRequests = 0;
    sData.TextureRequests = 0;
    sData.ASRequests = 0;
    sData.CmdBuffer.reset();
    sData.Requests.clear();
}
