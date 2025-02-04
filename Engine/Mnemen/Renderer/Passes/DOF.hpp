//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 12:06:40
//

#pragma once

#include <Renderer/RenderPass.hpp>

/// @brief A class that implements the Depth of Field (DOF) effect as a render pass.
///
/// The `DOF` class is a specific render pass that applies the Depth of Field (DOF) effect to a scene.
/// The Depth of Field effect simulates the blurring of objects that are out of focus, creating a more
/// cinematic and realistic look by mimicking the behavior of real-world camera lenses. It extends the
/// `RenderPass` class.
class DOF : public RenderPass
{
public:
    DOF(RHI::Ref rhi);
    ~DOF() = default;

    void Bake(Scene& scene) {}
    void Render(const Frame& frame, Scene& scene) override;
    void UI(const Frame& frame) override;
};
