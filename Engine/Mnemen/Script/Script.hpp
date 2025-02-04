//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 23:48:04
//

#pragma once

#include <Wren++/Wren++.h>
#include <Asset/AssetManager.hpp>

class Script
{
public:
    Script() = default;
    ~Script() = default;

    void SetSource(Asset::Handle handle);
    void Awake();
    void Quit();
    void Update(float dt);
private:
    wrenpp::VM mVirtualMachine;
    wrenpp::Method mAwake;
    wrenpp::Method mQuit;
    wrenpp::Method mUpdate;

    Asset::Handle mHandle;
};
