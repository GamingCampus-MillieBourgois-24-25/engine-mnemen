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

    /// @brief Draws the UI for controlling color grading settings.
    ///
    /// This function renders a user interface for adjusting the color grading parameters like
    /// brightness, exposure, saturation, etc.
    ///
    /// @param frame The frame to render the UI for.
    void UI(const Frame& frame) override;

private:
    /// @brief The compute pipeline used for the color grading effect.
    ComputePipeline::Ref mPipeline;

    /// @brief The brightness adjustment for the color grading effect.
    float mBrightness = 1.0f;

    /// @brief The exposure adjustment for the color grading effect.
    float mExposure = 1.0f;

    /// @brief The saturation adjustment for the color grading effect.
    float mSaturation = 1.0f;

    /// @brief The contrast adjustment for the color grading effect.
    float mContrast = 1.0f;

    /// @brief The hue shift adjustment for the color grading effect.
    float mHueShift = 1.0f;

    /// @brief The shadows adjustment for the color grading effect.
    glm::vec4 mShadows = glm::vec4(0.5f); ///< RGBA values for shadow adjustments.

    /// @brief The highlights adjustment for the color grading effect.
    glm::vec4 mHightLigths = glm::vec4(0.5f); ///< RGBA values for highlight adjustments.

    /// @brief The overall balance adjustment for the color grading effect.
    float mBalance = 1.0f;

    /// @brief The temperature adjustment for the color grading effect (e.g., warmth).
    float mTemperature = 0.0f;

    /// @brief The tint adjustment for the color grading effect (e.g., green or magenta tint).
    float mTint = 0.0f;

    /// @brief A color filter applied to the scene, allowing color modifications.
    glm::vec4 mColorFilter = glm::vec4(1.0f); ///< RGBA color filter.
};

