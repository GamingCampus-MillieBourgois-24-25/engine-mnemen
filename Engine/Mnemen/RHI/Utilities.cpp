//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-03 06:12:55
//

#include <RHI/Utilities.hpp>
#include <Core/Logger.hpp>

void D3DUtils::Release(IUnknown* object)
{
    if (object) {
        object->Release();
        object = nullptr;
    }
}

UInt64 D3DUtils::CalculateAdapterScore(IDXGIAdapter1* adapter)
{
    ID3D12Device* device;
    HRESULT result = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));
    if (FAILED(result))
        return 0;

    DXGI_ADAPTER_DESC1 desc;
    adapter->GetDesc1(&desc);
    if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
        return 0;

    UInt64 resultScore = 0;
    resultScore += desc.DedicatedVideoMemory;

    D3D12_FEATURE_DATA_D3D12_OPTIONS5 raytracingData = {};
    D3D12_FEATURE_DATA_D3D12_OPTIONS6 VRSData = {};
    D3D12_FEATURE_DATA_D3D12_OPTIONS7 meshShaderData = {};
    D3D12_FEATURE_DATA_D3D12_OPTIONS21 workGraphData = {};
        
    device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &raytracingData, sizeof(raytracingData));
    device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS6, &VRSData, sizeof(VRSData));
    device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS7, &meshShaderData, sizeof(meshShaderData));
    device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS21, &workGraphData, sizeof(workGraphData));

    if (raytracingData.RaytracingTier >= D3D12_RAYTRACING_TIER_1_1) {
        resultScore += 10000;
    }
    if (VRSData.VariableShadingRateTier >= D3D12_VARIABLE_SHADING_RATE_TIER_1) {
        resultScore += 10000;
    }
    if (meshShaderData.MeshShaderTier >= D3D12_MESH_SHADER_TIER_1) {
        resultScore += 10000;
    }
    if (workGraphData.WorkGraphsTier >= D3D12_WORK_GRAPHS_TIER_1_0) {
        resultScore += 10000;
    }
    device->Release();

    return resultScore;
}
