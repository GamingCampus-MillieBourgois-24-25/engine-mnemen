//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-03 10:08:14
//

#pragma once

#include <RHI/Device.hpp>

class Fence;
class CommandBuffer;

enum class QueueType
{
    AllGraphics = D3D12_COMMAND_LIST_TYPE_DIRECT,
    Compute = D3D12_COMMAND_LIST_TYPE_COMPUTE,
    Copy = D3D12_COMMAND_LIST_TYPE_COPY,
    VideoDecode = D3D12_COMMAND_LIST_TYPE_VIDEO_DECODE,
    VideoEncode = D3D12_COMMAND_LIST_TYPE_VIDEO_ENCODE
};

class Queue
{
public:
    using Ref = Ref<Queue>;

    Queue(Device::Ref device, QueueType type);
    ~Queue();

    void Wait(::Ref<Fence> fence, UInt64 value);
    void Signal(::Ref<Fence> fence, UInt64 value);
    void Submit(const Vector<::Ref<CommandBuffer>>& buffers);

    ID3D12CommandQueue* GetQueue() { return mQueue; }
    QueueType GetType() { return mType; }
private:
    ID3D12CommandQueue* mQueue = nullptr;
    QueueType mType;
};
