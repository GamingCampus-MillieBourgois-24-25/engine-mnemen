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
    mVirtualMachine = wrenpp::VM(); // Reset VM
    mAwake = {};
    mQuit = {};
    mUpdate = {};
    mLoaded = false;

    auto result = mVirtualMachine.executeString(handle->Script.Source);
    if (result == wrenpp::Result::Success) {
        if (mHandle) {
            AssetManager::GiveBack(mHandle->Path);
        }
        mHandle = handle;
        mAwake = mVirtualMachine.method("main", "awake", "call()");
        mQuit = mVirtualMachine.method("main", "quit", "call()");
        mUpdate = mVirtualMachine.method("main", "update", "call(_)");
        mLoaded = true;
        return true;
    } else  {
        LOG_ERROR("???");
    }
    return false;
}

void Script::Awake()
{
    if (mLoaded)
        mAwake();
}

void Script::Quit()
{
    if (mLoaded)
        mQuit();
}

void Script::Update(float dt)
{
    if (mLoaded)
        mUpdate(dt);
}
