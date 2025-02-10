//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 11:57:57
//

/*
    These are some debug types to make the code look better in my opinion. Makes it a bit more uniform and clean.
*/

#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>
#include <array>
#include <string>
#include <queue>
#include <set>

/// @brief Bitwise shift macro for setting the bit at position b.
/// 
/// This macro is used to generate a bitmask with a 1 at position `b`.
/// 
/// @param b The bit position to shift.
#define BIT(b) 1 << b

/// @brief Type definition for a signed 8-bit integer.
typedef int8_t Int8;

/// @brief Type definition for an unsigned 8-bit integer.
typedef uint8_t UInt8;

/// @brief Type definition for a signed 16-bit integer.
typedef int16_t Int16;

/// @brief Type definition for an unsigned 16-bit integer.
typedef uint16_t UInt16;

/// @brief Type definition for a signed 32-bit integer.
typedef int32_t Int32;

/// @brief Type definition for an unsigned 32-bit integer.
typedef uint32_t UInt32;

/// @brief Type definition for a signed 64-bit integer.
typedef int64_t Int64;

/// @brief Type definition for an unsigned 64-bit integer.
typedef uint64_t UInt64;

/// @brief Type definition for a 32-bit floating-point value.
typedef float Float32;

/// @brief Type definition for a 64-bit floating-point value.
typedef double Float64;

/// @brief Shared pointer type alias.
/// 
/// A shared pointer to the type `T`.
/// 
/// @tparam T The type to be pointed to by the shared pointer.
template<typename T>
using Ref = std::shared_ptr<T>;

/// @brief Creates a shared pointer of type `T` with the provided arguments.
/// 
/// This function creates and returns a shared pointer of type `T`, 
/// forwarding the provided arguments to the constructor of `T`.
/// 
/// @tparam T The type to be pointed to by the shared pointer.
/// @tparam Arguments The types of the arguments to forward to the constructor of `T`.
/// @param arguments Arguments to be forwarded to the constructor of `T`.
/// @return A shared pointer to the newly created object of type `T`.
template<typename T, typename... Arguments>
constexpr Ref<T> MakeRef(Arguments&&... arguments)
{
    return std::make_shared<T>(std::forward<Arguments>(arguments)...);
}

/// @brief Unique pointer type alias.
/// 
/// A unique pointer to the type `T`.
/// 
/// @tparam T The type to be pointed to by the unique pointer.
template<typename T>
using Unique = std::unique_ptr<T>;

/// @brief Creates a unique pointer of type `T` with the provided arguments.
/// 
/// This function creates and returns a unique pointer of type `T`, 
/// forwarding the provided arguments to the constructor of `T`.
/// 
/// @tparam T The type to be pointed to by the unique pointer.
/// @tparam Arguments The types of the arguments to forward to the constructor of `T`.
/// @param arguments Arguments to be forwarded to the constructor of `T`.
/// @return A unique pointer to the newly created object of type `T`.
template<typename T, typename... Arguments>
constexpr Unique<T> MakeUnique(Arguments&&... arguments)
{
    return std::make_unique<T>(std::forward<Arguments>(arguments)...);
}

/// @brief Weak pointer type alias.
/// 
/// A weak pointer to the type `T`, which does not affect the reference count.
template<typename T>
using Weak = std::weak_ptr<T>;

/// @brief Set type alias.
/// 
/// A set of type `T`.
template<typename T>
using Set = std::set<T>;

/// @brief Vector type alias.
/// 
/// A vector container for holding a sequence of elements of type `T`.
template<typename T>
using Vector = std::vector<T>;

/// @brief Array type alias for a fixed-size array.
/// 
/// An array container for holding a fixed number (`Size`) of elements of type `T`.
/// 
/// @tparam T The type of the elements in the array.
/// @tparam Size The number of elements in the array.
template<typename T, UInt64 Size>
using Array = std::array<T, Size>;

/// @brief Queue array type alias.
/// 
/// A queue container for holding a sequence of elements of type `T`.
template<typename T>
using QueueArray = std::queue<T>;

/// @brief Unordered map type alias.
/// 
/// An unordered map container for holding key-value pairs, where keys are of type `Key`
/// and values are of type `Value`.
/// 
/// @tparam Key The type of the keys in the map.
/// @tparam Value The type of the values in the map.
template<typename Key, typename Value>
using UnorderedMap = std::unordered_map<Key, Value>;

/// @brief Pair type alias.
/// 
/// A pair of elements, where the first element is of type `First` and the second element is of type `Second`.
/// 
/// @tparam First The type of the first element in the pair.
/// @tparam Second The type of the second element in the pair.
template<typename First, typename Second>
using Pair = std::pair<First, Second>;

/// @brief Type definition for a standard string.
using String = std::string;

/// @brief Type definition for a wide string.
using WideString = std::wstring;
