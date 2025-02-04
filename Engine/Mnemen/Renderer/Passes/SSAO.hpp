//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 12:06:40
//

#pragma once

#include <Renderer/RenderPass.hpp>

/// @brief A class that implements the Screen Space Ambient Occlusion (SSAO) effect.
///
/// The `SSAO` class is a specific render pass that applies the SSAO effect to a scene. This effect 
/// simulates the shading that occurs due to the occlusion of light in small crevices or corners of 
/// objects, giving the scene more depth and realism. It extends the `RenderPass` class.
class SSAO : public RenderPass
{
public:
    SSAO(RHI::Ref rhi);
    ~SSAO() = default;

    void Bake(Scene& scene) {}
    void Render(const Frame& frame, Scene& scene) override;
    void UI(const Frame& frame) override;
};
