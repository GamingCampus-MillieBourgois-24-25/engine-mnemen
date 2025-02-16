//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 12:06:40
//

#pragma once

#include <Renderer/RenderPass.hpp>

/// @class ColorGrading
/// @brief A class that handles color grading adjustments in the rendering pipeline.
///
/// This class applies various color grading effects to the rendered scene, including brightness,
/// exposure, saturation, contrast, hue shift, shadows, highlights, and color filters.
class ColorGrading : public RenderPass
{
public:
    /// @brief Constructs a ColorGrading object.
    ///
    /// This constructor initializes the `ColorGrading` object with the provided render hardware interface (RHI).
    ///
    /// @param rhi The render hardware interface reference.
    ColorGrading(RHI::Ref rhi);

    /// @brief Destructor for the ColorGrading class.
    ~ColorGrading() = default;
   
    /// @brief Prepares the scene for baking.
    ///
    /// This function is currently empty, but it can be customized for preparing or processing scene data
    /// before rendering.
    ///
    /// @param scene The scene to be baked.
    void Bake(::Ref<Scene> scene) {}

    /// @brief Renders the color grading effect for the given frame and scene.
    ///
    /// This function applies the color grading adjustments to the given frame and scene.
    ///
    /// @param frame The frame to render.
    /// @param scene The scene to apply color grading to.
    void Render(const Frame& frame, ::Ref<Scene> scene) override;

private:
    /// @brief The compute pipeline used for the color grading effect.
    ComputePipeline::Ref mPipeline;
};

