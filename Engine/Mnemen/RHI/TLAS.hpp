//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-12 05:05:49
//

#pragma once

#include <RHI/AccelerationStructure.hpp>
#include <glm/glm.hpp>

struct RaytracingInstance
{
    glm::mat3x4 Transform;
    uint32_t InstanceID	: 24;
    uint32_t InstanceMask : 8;
    uint32_t InstanceContributionToHitGroupIndex : 24;
    uint32_t Flags : 8;
    uint64_t AccelerationStructure;
};

class TLAS : public AccelerationStructure
{
public:
    using Ref = Ref<TLAS>;

    TLAS(Device::Ref device, DescriptorHeaps& heaps, Buffer::Ref instanceBuffer, UInt32 numInstance, const String& name = "TLAS");
    ~TLAS();

    int Bindless() const { return mSRV.Index; }
private:
    DescriptorHeap::Descriptor mSRV;
};
