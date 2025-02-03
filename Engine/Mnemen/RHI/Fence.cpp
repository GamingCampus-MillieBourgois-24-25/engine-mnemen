//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-03 10:03:19
//

#include <RHI/Fence.hpp>
#include <RHI/Utilities.hpp>
#include <RHI/Queue.hpp>

#include <Core/Assert.hpp>

Fence::Fence(Device::Ref device)
    : mValue(0)
{
    HRESULT result = device->GetDevice()->CreateFence(mValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence));
    ASSERT(SUCCEEDED(result), "Failed to create fence!");
}

Fence::~Fence()
{
    D3DUtils::Release(mFence);
}

UInt64 Fence::Signal(::Ref<Queue> queue)
{
    mValue++;
    queue->GetQueue()->Signal(mFence, mValue);
    return mValue;
}

void Fence::Wait(UInt64 value)
{
    if (GetCompletedValue() < value) {
        HANDLE event = ::CreateEventA(nullptr, false, false, "Fence Wait Event");
        mFence->SetEventOnCompletion(value, event);
        if (::WaitForSingleObject(event, 10'000'000) == WAIT_TIMEOUT) {
            ASSERT(false, "!! GPU TIME-OUT !!");
        }
    }
}
