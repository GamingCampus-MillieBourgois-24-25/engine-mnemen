//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-12 05:01:14
//

#include <RHI/BLAS.hpp>

BLAS::BLAS(Device::Ref device, DescriptorHeaps& heaps, Buffer::Ref vertex, Buffer::Ref index, UInt32 vtxCount, UInt32 idxCount, const String& name)
    : AccelerationStructure(device, heaps)
{
    // mGeometryDesc = {};
    // mGeometryDesc.Type = D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;
    // mGeometryDesc.Flags = D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE;
    // mGeometryDesc.Triangles.IndexBuffer = index->GetAddress();
    // mGeometryDesc.Triangles.IndexCount = idxCount;
    // mGeometryDesc.Triangles.IndexFormat = DXGI_FORMAT_R32_UINT;
    // mGeometryDesc.Triangles.VertexCount = vtxCount;
    // mGeometryDesc.Triangles.VertexFormat = DXGI_FORMAT_R32G32B32_FLOAT;
    // mGeometryDesc.Triangles.VertexBuffer.StartAddress = vertex->GetAddress();
    // mGeometryDesc.Triangles.VertexBuffer.StrideInBytes = vertex->GetStride();
    // mGeometryDesc.Triangles.Transform3x4 = 0;
// 
    // mInputs = {};
    // mInputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;
    // mInputs.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE;
    // mInputs.NumDescs = 1;
    // mInputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;
    // mInputs.pGeometryDescs = &mGeometryDesc;
// 
    // Allocate(name);
}
