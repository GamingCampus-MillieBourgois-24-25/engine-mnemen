//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-12 04:54:59
//

#pragma once

#include <RHI/AccelerationStructure.hpp>

class BLAS : public AccelerationStructure
{
public:
    using Ref = Ref<BLAS>;

    BLAS(Device::Ref device, DescriptorHeaps& heaps, Buffer::Ref vertex, Buffer::Ref index, UInt32 vtxCount, UInt32 idxCount, const String& name = "BLAS");
    ~BLAS() = default;
private:
    D3D12_RAYTRACING_GEOMETRY_DESC mGeometryDesc;
};
