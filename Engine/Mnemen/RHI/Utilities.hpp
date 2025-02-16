//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-03 06:11:47
//

#pragma once

#include <Agility/d3d12.h>
#include <dxgi1_6.h>
#include <Core/Common.hpp>

/// @brief Utility class for Direct3D-related operations.
/// @details The `D3DUtils` class provides helper functions for performing common Direct3D tasks,
///          such as releasing Direct3D resources and calculating the quality or score of a GPU adapter.
class D3DUtils
{
public:
    /// @brief Releases a Direct3D object.
    /// @param object A pointer to the Direct3D object (IUnknown*) to release.
    /// @details This function is a helper to release COM objects. It calls the `Release` method 
    ///          on the given object and ensures that the reference count is properly decremented.
    static void Release(IUnknown* object);

    /// @brief Calculates the adapter score, which can be used to rank the quality of a GPU adapter.
    /// @param adapter A pointer to the `IDXGIAdapter1` object representing the GPU adapter.
    /// @return A score (UInt64) representing the quality or suitability of the GPU adapter for use.
    /// @details This function evaluates various features of the adapter (such as performance, capabilities, etc.)
    ///          and returns a score that can be used to compare different GPUs.
    static UInt64 CalculateAdapterScore(IDXGIAdapter1* adapter);
};

