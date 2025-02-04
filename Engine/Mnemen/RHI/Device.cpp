//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-03 06:11:18
//

#include <RHI/Device.hpp>
#include <RHI/Utilities.hpp>

#include <Core/Assert.hpp>
#include <Core/Logger.hpp>
#include <Core/UTF.hpp>

#include <unordered_map>

extern "C"
{
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

    __declspec(dllexport) extern const UInt32 D3D12SDKVersion = 614;
    __declspec(dllexport) extern const char* D3D12SDKPath = ".\\D3D12\\";
}

Device::Device()
{
    // Create factory.
    IDXGIFactory1* tempFactory;
    HRESULT result = CreateDXGIFactory1(IID_PPV_ARGS(&tempFactory));
    ASSERT(SUCCEEDED(result), "Failed to create DXGI factory!");
    tempFactory->QueryInterface(IID_PPV_ARGS(&mFactory));
    tempFactory->Release();

    // Create debug interface.
#if defined(MNEMEN_DEBUG)
    ID3D12Debug* debug;
    result = D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
    ASSERT(SUCCEEDED(result), "Failed to get debug interface!");
    debug->QueryInterface(IID_PPV_ARGS(&mDebug));
    debug->Release();

    mDebug->EnableDebugLayer();
    mDebug->SetEnableGPUBasedValidation(true);
#endif

    // Get adapter.
    UnorderedMap<IDXGIAdapter1*, UInt64> adapterScores;
    for (UInt32 adapterIndex = 0;; adapterIndex++) {
        IDXGIAdapter1* adapter;
        if (FAILED(mFactory->EnumAdapterByGpuPreference(adapterIndex, DXGI_GPU_PREFERENCE_UNSPECIFIED, IID_PPV_ARGS(&adapter))))
            break;

        adapterScores[adapter] = D3DUtils::CalculateAdapterScore(adapter);
    }

    Pair<IDXGIAdapter1*, UInt64> bestAdapter = { nullptr, 0 };
    for (auto& pair : adapterScores) {
        DXGI_ADAPTER_DESC1 desc;
        pair.first->GetDesc1(&desc);

        if (pair.second > bestAdapter.second) {
            bestAdapter = pair;
        }
        LOG_DEBUG("Found GPU {0} with score {1}", UTF::WideToAscii(desc.Description), pair.second);
    }
    mAdapter = bestAdapter.first;

    DXGI_ADAPTER_DESC1 desc;
    mAdapter->GetDesc1(&desc);
    LOG_INFO("Selecting GPU {0}", UTF::WideToAscii(desc.Description));

    // Create device.
    result = D3D12CreateDevice(mAdapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&mDevice));
    ASSERT(SUCCEEDED(result), "Failed to create D3D12 device!");

    // Create info queue.
    ID3D12InfoQueue* infoQueue = 0;
    result = mDevice->QueryInterface(IID_PPV_ARGS(&infoQueue));
    if (SUCCEEDED(result)) {
        infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
        infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);

        D3D12_MESSAGE_SEVERITY supressSeverities[] = {
            D3D12_MESSAGE_SEVERITY_INFO
        };
        D3D12_MESSAGE_ID supressIDs[] = {
            D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,
            D3D12_MESSAGE_ID_CLEARDEPTHSTENCILVIEW_MISMATCHINGCLEARVALUE,
            D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,
            D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE
        };

        D3D12_INFO_QUEUE_FILTER filter = {0};
        filter.DenyList.NumSeverities = ARRAYSIZE(supressSeverities);
        filter.DenyList.pSeverityList = supressSeverities;
        filter.DenyList.NumIDs = ARRAYSIZE(supressIDs);
        filter.DenyList.pIDList = supressIDs;

        infoQueue->PushStorageFilter(&filter);
        infoQueue->Release();
    }
}

Device::~Device()
{
    D3DUtils::Release(mDevice);
    D3DUtils::Release(mAdapter);
    D3DUtils::Release(mDebug);
    D3DUtils::Release(mFactory);
}
