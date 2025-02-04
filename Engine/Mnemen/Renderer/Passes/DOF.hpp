//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 12:06:40
//

#pragma once

#include <Renderer/RenderPass.hpp>

class DOF : public RenderPass
{
public:
    DOF(RHI::Ref rhi);
    ~DOF() = default;

    void Bake(Scene& scene) {}
    void Render(const Frame& frame, Scene& scene) override;
    void UI(const Frame& frame) override;
};
