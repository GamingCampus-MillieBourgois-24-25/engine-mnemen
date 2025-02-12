//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 23:58:43
//

#include "Entity.hpp"

void ScriptComponent::AddEmptyScript()
{
    Ref<Instance> instance = MakeRef<Instance>();
    instance->ID = Util::NewUUID();
    Instances.push_back(instance);
}

void ScriptComponent::PushScript(const String& path)
{
    Ref<Instance> instance = MakeRef<Instance>();
    instance->ID = Util::NewUUID();
    instance->Path = path;
    Instances.push_back(instance);
}
