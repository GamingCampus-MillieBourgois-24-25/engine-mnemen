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

#define BIT(b) 1 << b

typedef int8_t Int8;
typedef uint8_t UInt8;
typedef int16_t Int16;
typedef uint16_t UInt16;
typedef int32_t Int32;
typedef uint32_t UInt32;
typedef int64_t Int64;
typedef uint64_t UInt64;
typedef float Float32;
typedef double Float64;

template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T, typename... Arguments>
constexpr Ref<T> MakeRef(Arguments&&... arguments)
{
    return std::make_shared<T>(std::forward<Arguments>(arguments)...);
}

template<typename T>
using Weak = std::weak_ptr<T>;

template<typename T>
using Vector = std::vector<T>;

template<typename T, UInt64 Size>
using Array = std::array<T, Size>;

template<typename T>
using QueueArray = std::queue<T>;

template<typename Key, typename Value>
using UnorderedMap = std::unordered_map<Key, Value>;

template<typename First, typename Second>
using Pair = std::pair<First, Second>;

using String = std::string;
using WideString = std::wstring;
