//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-12 04:43:57
//

#include <RHI/AccelerationStructure.hpp>

#undef max

AccelerationStructure::AccelerationStructure(Device::Ref device, DescriptorHeaps& heaps)
    : Resource(device), mHeaps(heaps)
{
    mInputs = {};
}

void AccelerationStructure::Allocate(const String& name)
{
    // D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO prebuildInfo = {};
    // mParentDevice->GetDevice()->GetRaytracingAccelerationStructurePrebuildInfo(&mInputs, &prebuildInfo);
    // mUpdateSize = prebuildInfo.UpdateScratchDataSizeInBytes;

    // Create scratch
    // mScratch = MakeRef<Buffer>(mParentDevice, mHeaps, prebuildInfo.ScratchDataSizeInBytes, 0, BufferType::Storage, "Scratch Acceleration Structure " + name);

    // Create AS
    // D3D12_HEAP_PROPERTIES props = {};
    // props.Type = D3D12_HEAP_TYPE_DEFAULT;
// 
    // D3D12_RESOURCE_DESC desc = {};
    // desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS | D3D12_RESOURCE_FLAG_RAYTRACING_ACCELERATION_STRUCTURE;
    // desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    // desc.Width = prebuildInfo.ResultDataMaxSizeInBytes;
    // desc.Height = 1;
    // desc.DepthOrArraySize = 1;
    // desc.MipLevels = 1;
    // desc.SampleDesc.Count = 1;
    // desc.SampleDesc.Quality = 0;
    // desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    // CreateResource(&props, &desc, D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE);
    // SetName(name);
}

void AccelerationStructure::FreeScratch()
{
    // mScratch.reset();
}
