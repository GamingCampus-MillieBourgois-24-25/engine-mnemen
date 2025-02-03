//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-03 10:11:31
//

#include <RHI/Queue.hpp>
#include <RHI/Utilities.hpp>
#include <RHI/Fence.hpp>
#include <RHI/CommandBuffer.hpp>

#include <Core/Assert.hpp>

Queue::Queue(Device::Ref device, QueueType type)
    : mType(type)
{
    D3D12_COMMAND_QUEUE_DESC desc = {};
    desc.Type = D3D12_COMMAND_LIST_TYPE(type);

    HRESULT result = device->GetDevice()->CreateCommandQueue(&desc, IID_PPV_ARGS(&mQueue));
    ASSERT(SUCCEEDED(result), "Failed to create queue!");
}

Queue::~Queue()
{
    D3DUtils::Release(mQueue);
}

void Queue::Wait(::Ref<Fence> fence, UInt64 value)
{
    mQueue->Wait(fence->GetFence(), value);
}

void Queue::Signal(::Ref<Fence> fence, UInt64 value)
{
    mQueue->Signal(fence->GetFence(), value);
}

void Queue::Submit(const Vector<::Ref<CommandBuffer>>& buffers)
{
    std::vector<ID3D12CommandList*> lists;
    for (auto& buffer : buffers) {
        lists.push_back(buffer->GetList());
    }

    mQueue->ExecuteCommandLists(lists.size(), lists.data());
}
