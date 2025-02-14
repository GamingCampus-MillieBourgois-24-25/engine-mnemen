//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-14 15:55:42
//

#include "String.hpp"

#include <algorithm>

String StringUtil::Lowercase(const String& str)
{
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c){ return std::tolower(c); });
    return str;
}
