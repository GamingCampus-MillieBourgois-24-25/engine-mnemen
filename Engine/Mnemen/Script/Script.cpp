//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 23:52:20
//

#include "Script.hpp"

void Script::SetSource(Asset::Handle handle)
{
    mHandle = handle;

    mVirtualMachine.executeString(mHandle->Script.Source);
    mAwake = mVirtualMachine.method("main", "awake", "call()");
    mQuit = mVirtualMachine.method("main", "quit", "call()");
    mUpdate = mVirtualMachine.method("main", "update", "call(_)");
}

void Script::Awake()
{
    mAwake();
}

void Script::Quit()
{
    mQuit();
}

void Script::Update(float dt)
{
    mUpdate(dt);
}
