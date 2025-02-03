//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-04 02:27:24
//

#pragma once

#include <Core/Common.hpp>

struct Image
{
    int Width;
    int Height;
    int Levels;
    bool Compressed = false;
    Vector<UInt8> Pixels;

    void Load(const String& path);
};
