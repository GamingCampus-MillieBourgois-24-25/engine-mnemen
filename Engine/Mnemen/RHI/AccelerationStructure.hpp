//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-12 04:40:23
//

#pragma once

#include <RHI/Device.hpp>
#include <RHI/DescriptorHeap.hpp>
#include <RHI/Resource.hpp>
#include <RHI/Buffer.hpp>

class AccelerationStructure : public Resource
{
public:
    AccelerationStructure(Device::Ref device, DescriptorHeaps& heaps);
    ~AccelerationStructure() = default;

    void FreeScratch();
    Buffer::Ref GetScratch() { return mScratch; }
protected:
    friend class Uploader;
    friend class CommandBuffer;

    void Allocate(const String& name = "Acceleration Structure");

    DescriptorHeaps mHeaps;
    D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS mInputs;
    UInt64 mUpdateSize = 0;
    Buffer::Ref mScratch = nullptr;
};
