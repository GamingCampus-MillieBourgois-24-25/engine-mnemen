//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-03 06:10:13
//

#pragma once

#include <Agility/d3d12.h>
#include <dxgi1_6.h>

#include <Core/Common.hpp>

/// @brief Represents a Direct3D 12 device.
class Device
{
public:
    /// @brief Type alias for referencing
    using Ref = Ref<Device>;

    /// @brief Initializes the Direct3D 12 device.
    Device();

    /// @brief Destroys the device and releases resources.
    ~Device();

    /// @brief Retrieves the underlying Direct3D 12 device.
    /// @return Pointer to the Direct3D 12 device.
    ID3D12Device14* GetDevice() { return mDevice; }

    /// @brief Retrieves the DXGI factory.
    /// @return Pointer to the DXGI factory.
    IDXGIFactory6* GetFactory() { return mFactory; }

private:
    IDXGIFactory6* mFactory = nullptr; ///< DXGI factory for creating adapters.
    IDXGIAdapter1* mAdapter = nullptr; ///< Selected GPU adapter.
    ID3D12Device14* mDevice = nullptr; ///< Direct3D 12 device.
    ID3D12Debug1* mDebug = nullptr; ///< Debug interface for validation (if enabled).
};
