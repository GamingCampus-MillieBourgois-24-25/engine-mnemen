//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 12:06:40
//

#pragma once

#include <Renderer/RenderPass.hpp>

/// @brief A class that implements a color grading render pass.
///
/// The `ColorGrading` class is a specific render pass that applies color grading effects to the scene.
/// Color grading is used to adjust the overall color balance and tones of the rendered image, allowing
/// for artistic effects or corrections. This class extends the `RenderPass` class.
class ColorGrading : public RenderPass
{
public:
    ColorGrading(RHI::Ref rhi);
    ~ColorGrading() = default;
   
    void Bake(::Ref<Scene> scene) {}
    void Render(const Frame& frame, ::Ref<Scene> scene) override;
    void UI(const Frame& frame) override;
private:
    ComputePipeline::Ref mPipeline;
    float mBrightness = 1.0f;
    float mExposure = 1.0f;
    float mSaturation = 1.0f;
    float mContrast = 1.0f;
};
