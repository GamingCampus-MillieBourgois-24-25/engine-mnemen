//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-03 10:01:44
//

#pragma once

#include <Core/Common.hpp>
#include <RHI/Device.hpp>

class Queue;

class Fence
{
public:
    using Ref = Ref<Fence>;

    Fence(Device::Ref device);
    ~Fence();

    void Wait(UInt64 value);
    UInt64 Signal(::Ref<Queue> queue);
    UInt64 GetCompletedValue() { return mFence->GetCompletedValue(); }

    ID3D12Fence* GetFence() { return mFence; }
    UInt64 GetValue() { return mValue; }
private:
    UInt64 mValue = 0;
    ID3D12Fence* mFence = nullptr;
};
