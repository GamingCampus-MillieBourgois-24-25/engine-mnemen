//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 23:52:20
//

#include "Script.hpp"

#include <Core/Logger.hpp>

Script::~Script()
{
    if (mLoaded)
        AssetManager::GiveBack(mHandle->Path);
}

bool Script::SetSource(Asset::Handle handle)
{
    AssetManager::GiveBack(handle->Path);
    return true;
}

void Script::Awake()
{
}

void Script::Quit()
{
}

void Script::Update(float dt)
{
}
