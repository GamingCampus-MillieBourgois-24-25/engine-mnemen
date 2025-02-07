//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 23:58:43
//

#include "Components.hpp"

void ScriptComponent::Load(const String& path)
{
    Path = path;
    Handle.SetSource(AssetManager::Get(path, AssetType::Script));
}
