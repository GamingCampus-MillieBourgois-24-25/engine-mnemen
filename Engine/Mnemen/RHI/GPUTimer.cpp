//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-05 19:43:09
//

#include "GPUTimer.hpp"

#include <Core/Assert.hpp>

GPUTimer::Data GPUTimer::sData = {};

void GPUTimer::Init(RHI::Ref rhi)
{
    sData.RHI = rhi;
    sData.MaxTimers = 256;
    
    D3D12_QUERY_HEAP_DESC queryHeapDesc = {};
    queryHeapDesc.Count = sData.MaxTimers * 2;
    queryHeapDesc.Type = D3D12_QUERY_HEAP_TYPE_TIMESTAMP;
    queryHeapDesc.NodeMask = 0;
    
    HRESULT result = rhi->mDevice->GetDevice()->CreateQueryHeap(&queryHeapDesc, IID_PPV_ARGS(&sData.QueryHeap));
    ASSERT(SUCCEEDED(result), "Failed to create GPU timer query heap!");

    D3D12_RESOURCE_DESC bufferDesc = {};
    bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    bufferDesc.Width = sData.MaxTimers * 2 * sizeof(UInt64);
    bufferDesc.Height = 1;
    bufferDesc.DepthOrArraySize = 1;
    bufferDesc.MipLevels = 1;
    bufferDesc.Format = DXGI_FORMAT_UNKNOWN;
    bufferDesc.SampleDesc.Count = 1;
    bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    bufferDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    D3D12_HEAP_PROPERTIES heapProps = {};
    heapProps.Type = D3D12_HEAP_TYPE_READBACK;

    rhi->mDevice->GetDevice()->CreateCommittedResource(
        &heapProps, D3D12_HEAP_FLAG_NONE, &bufferDesc,
        D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&sData.ReadbackBuffer));

    rhi->mGraphicsQueue->GetQueue()->GetTimestampFrequency(&sData.Frequency);

    sData.Timestamps.resize(sData.MaxTimers * 2, 0);
}

void GPUTimer::Exit()
{
    sData.ReadbackBuffer->Release();
    sData.QueryHeap->Release();
}

void GPUTimer::Start(CommandBuffer::Ref cmdBuffer, UInt32 timerIndex)
{
    cmdBuffer->GetList()->EndQuery(sData.QueryHeap, D3D12_QUERY_TYPE_TIMESTAMP, timerIndex * 2);
}

void GPUTimer::Stop(CommandBuffer::Ref cmdBuffer, UInt32 timerIndex)
{
    cmdBuffer->GetList()->EndQuery(sData.QueryHeap, D3D12_QUERY_TYPE_TIMESTAMP, timerIndex * 2 + 1);
}

void GPUTimer::Resolve(CommandBuffer::Ref cmdBuffer)
{
    cmdBuffer->GetList()->ResolveQueryData(sData.QueryHeap, D3D12_QUERY_TYPE_TIMESTAMP, 0, sData.MaxTimers * 2, sData.ReadbackBuffer, 0);
}

void GPUTimer::Readback()
{
    sData.RHI->Wait();
    D3D12_RANGE readRange = { 0, sData.MaxTimers * 2 * sizeof(UInt64) };
    UInt64* data = nullptr;
    if (SUCCEEDED(sData.ReadbackBuffer->Map(0, &readRange, reinterpret_cast<void**>(&data)))) {
        std::copy(data, data + sData.MaxTimers * 2, sData.Timestamps.begin());
        sData.ReadbackBuffer->Unmap(0, nullptr);
    }
}

double GPUTimer::GetTime(UInt32 timerIndex)
{
    UInt64 start = sData.Timestamps[timerIndex * 2];
    UInt64 end = sData.Timestamps[timerIndex * 2 + 1];
    return (end - start) * 1000.0 / sData.Frequency; // Convert to milliseconds
}
