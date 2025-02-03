//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-03 06:11:47
//

#pragma once

#include <Agility/d3d12.h>
#include <dxgi1_6.h>
#include <Core/Common.hpp>

class D3DUtils
{
public:
    static void Release(IUnknown* object);
    static UInt64 CalculateAdapterScore(IDXGIAdapter1* adapter);
};
