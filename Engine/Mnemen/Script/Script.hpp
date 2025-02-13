//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-13 16:20:41
//

#pragma once

#include <Core/Common.hpp>
#include <sol/sol.hpp>

class Script
{
public:
    using Ref = Ref<Script>;

    Script(const String& path);
    ~Script();

    bool IsValid() { return mValid; }
    sol::load_result* GetHandle() { return &mHandle; }
private:
    bool mValid = false;
    sol::load_result mHandle;
};
