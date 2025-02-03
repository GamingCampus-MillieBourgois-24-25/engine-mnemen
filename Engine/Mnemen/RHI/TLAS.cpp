//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-12 05:10:54
//

#include <RHI/TLAS.hpp>

TLAS::TLAS(Device::Ref device, DescriptorHeaps& heaps, Buffer::Ref instanceBuffer, UInt32 numInstance, const String& name)
    : AccelerationStructure(device, heaps)
{
    // mInputs = {};
    // mInputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;
    // mInputs.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_UPDATE | D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE;
    // mInputs.NumDescs = numInstance;
    // mInputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;
    // mInputs.InstanceDescs = instanceBuffer->GetAddress();
    // Allocate(name);

    // mSRV = heaps[DescriptorHeapType::ShaderResource]->Allocate();
// 
    // D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
    // desc.ViewDimension = D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;
    // desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    // desc.Format = DXGI_FORMAT_UNKNOWN;
    // desc.RaytracingAccelerationStructure.Location = mResource->GetGPUVirtualAddress();
    // 
    // mParentDevice->GetDevice()->CreateShaderResourceView(nullptr, &desc, mSRV.CPU);
}

TLAS::~TLAS()
{
    // mSRV.Parent->Free(mSRV);
}
