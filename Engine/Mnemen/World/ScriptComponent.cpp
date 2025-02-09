//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 23:58:43
//

#include "Components.hpp"

void ScriptComponent::PushScript(const String& path)
{
    Ref<Instance> instance = MakeRef<Instance>();
    instance->Path = path;
    instance->Handle.SetSource(AssetManager::Get(path, AssetType::Script));
    Instances.push_back(instance);
}
