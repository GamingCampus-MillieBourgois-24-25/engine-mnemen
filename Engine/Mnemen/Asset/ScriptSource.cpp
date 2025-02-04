//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 23:39:18
//

#include "ScriptSource.hpp"

#include <Core/File.hpp>

void ScriptSource::Load(const String& Path)
{
    Source = File::ReadFile(Path);
}
