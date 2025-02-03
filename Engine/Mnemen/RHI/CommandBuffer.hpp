//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-03 11:52:45
//

#pragma once

#include <RHI/Queue.hpp>
#include <RHI/View.hpp>
#include <RHI/GraphicsPipeline.hpp>
#include <RHI/ComputePipeline.hpp>
#include <RHI/Resource.hpp>
#include <RHI/Buffer.hpp>
#include <RHI/AccelerationStructure.hpp>
#include <RHI/TLAS.hpp>

enum class Topology
{
    LineList = D3D_PRIMITIVE_TOPOLOGY_LINELIST,
    LineStrip = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP,
    PointList = D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
    TriangleList = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
    TriangleStrip = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
};

class CommandBuffer
{
public:
    using Ref = Ref<CommandBuffer>;
 
    CommandBuffer(Device::Ref device, Queue::Ref queue, DescriptorHeaps heaps, bool singleTime = false);
    ~CommandBuffer();

    void Begin();
    void End();

    void BeginMarker(const String& name);
    void EndMarker();

    void UAVBarrier(::Ref<Resource> resource);
    void Barrier(::Ref<Resource> resource, ResourceLayout layout, UInt32 mip = VIEW_ALL_MIPS);
    
    void SetViewport(float x, float y, float width, float height);
    void SetTopology(Topology topology);
    void SetGraphicsPipeline(GraphicsPipeline::Ref pipeline);
    void SetComputePipeline(ComputePipeline::Ref pipeline);
    void SetRenderTargets(const Vector<View::Ref> targets, View::Ref depth);
    void SetVertexBuffer(Buffer::Ref buffer);
    void SetIndexBuffer(Buffer::Ref buffer);

    void GraphicsPushConstants(const void *data, UInt32 size, int index);
    void ComputePushConstants(const void *data, UInt32 size, int index);

    void ClearDepth(View::Ref view);
    void ClearRenderTarget(View::Ref view, float r, float g, float b);

    void Draw(int vertexCount);
    void DrawIndexed(int indexCount);
    void Dispatch(int x, int y, int z);

    void CopyTextureToTexture(::Ref<Resource> dst, ::Ref<Resource> src) { CopyBufferToBuffer(dst, src); } // It's all buffers anyway innit?
    void CopyBufferToBuffer(::Ref<Resource> dst, ::Ref<Resource> src);
    void CopyBufferToTexture(::Ref<Resource> dst, ::Ref<Resource> src);

    void UpdateTLAS(TLAS::Ref tlas, Buffer::Ref instaceBuffer, int numInstances);
    void BuildAccelerationStructure(::Ref<AccelerationStructure> as);

    void BeginGUI(int width, int height);
    void EndGUI();

    ID3D12GraphicsCommandList10* GetList() { return mList; }
    operator ID3D12CommandList*() { return mList; }
private:
    bool mSingleTime;
    Device::Ref mDevice = nullptr;
    Queue::Ref mParentQueue = nullptr;
    DescriptorHeaps mHeaps;
    ID3D12CommandAllocator* mAllocator = nullptr;
    ID3D12GraphicsCommandList10* mList = nullptr;
};
