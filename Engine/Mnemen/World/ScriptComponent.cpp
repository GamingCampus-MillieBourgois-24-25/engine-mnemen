//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 23:58:43
//

#include "Entity.hpp"

#include <Core/File.hpp>

ScriptComponent::EntityScript::EntityScript()
{
    ID = Util::NewUUID();
}

ScriptComponent::EntityScript::~EntityScript()
{
    if (Handle) {
        AssetManager::GiveBack(Handle->Path);
    }
}

void ScriptComponent::EntityScript::Load(const String& path)
{
    if (Handle) {
        AssetManager::GiveBack(Handle->Path);
    }
    Handle = AssetManager::Get(path, AssetType::Script);
    if (Handle)
        Instance = MakeRef<ScriptInstance>(Handle->Script->GetHandle());
}

void ScriptComponent::AddEmptyScript()
{
    Ref<EntityScript> script = MakeRef<EntityScript>();
    Instances.push_back(script);
}

void ScriptComponent::PushScript(const String& path)
{
    if (!File::Exists(path))
        return;

    Ref<EntityScript> script = MakeRef<EntityScript>();
    script->Load(path);
    Instances.push_back(script);
}
