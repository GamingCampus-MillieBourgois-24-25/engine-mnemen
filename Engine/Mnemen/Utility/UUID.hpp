//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-09 17:22:22
//

#pragma once

#include <Core/Common.hpp>

namespace Util
{
    /// @typedef UUID
    /// @brief A type alias for a 64-bit unsigned integer representing a UUID.
    ///
    /// This type is used to represent a unique identifier as a 64-bit unsigned integer (UInt64).
    typedef UInt64 UUID;

    /// @brief Generates a new UUID.
    ///
    /// This function generates and returns a new UUID. The specific method of generating the UUID
    /// is implementation-dependent but will ensure that each UUID is unique.
    ///
    /// @return A new UUID of type UInt64.
    UUID NewUUID();
}

