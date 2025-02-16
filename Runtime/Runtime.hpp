//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 12:23:59
//

#pragma once

#include <Mnemen.hpp>

class Runtime : public Application
{
public:
    Runtime(ApplicationSpecs specs);
    ~Runtime();

    virtual void OnUpdate(float dt) override;
    virtual void OnPhysicsTick() override;
    virtual void OnImGui(const Frame& frame) override;
};
