//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-03 06:10:13
//

#pragma once

#include <Agility/d3d12.h>
#include <dxgi1_6.h>

#include <Core/Common.hpp>

class Device
{
public:
    using Ref = Ref<Device>;

    Device();
    ~Device();

    ID3D12Device14* GetDevice() { return mDevice; }
    IDXGIFactory6* GetFactory() { return mFactory; }
private:
    IDXGIFactory6* mFactory = nullptr;
    IDXGIAdapter1* mAdapter = nullptr;
    ID3D12Device14* mDevice = nullptr;
    ID3D12Debug1* mDebug = nullptr;
};
