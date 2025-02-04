//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 11:22:11
//

#pragma once

#include <Renderer/RenderPass.hpp>

/// @brief A class that implements a composite render pass.
///
/// The `Composite` class is a specific render pass that combines various rendering elements into a
/// final image or scene output. The composite pass here just tones back to low dynamic range and
/// copies to the backbuffer.
class Composite : public RenderPass
{
public:
    /// @brief Constructs a Composite render pass with the specified rendering hardware interface (RHI).
    /// 
    /// This constructor initializes the composite render pass, responsible for applying final
    /// adjustments and effects to the rendered image. The RHI is passed to the base class `RenderPass`
    /// to manage hardware-specific rendering operations.
    /// 
    /// @param rhi The rendering hardware interface (RHI) used for the composite render pass.
    Composite(RHI::Ref rhi);

    /// @brief Destructor for the Composite class.
    /// 
    /// The destructor is defaulted and ensures proper cleanup of any resources used during the 
    /// composite render pass.
    ~Composite() = default;

    /// @brief Bakes the composite pass into the scene.
    /// 
    /// This method currently does not perform any operations, but it may be extended for any 
    /// preprocessing steps necessary for combining the rendered elements into a final output.
    /// 
    /// @param scene The scene to bake the composite pass into.
    void Bake(Scene& scene);

    /// @brief Executes the composite render pass for a given frame and scene.
    /// 
    /// This method converts the main color buffer back to low dynamic range and copies to the backbuffer.
    /// 
    /// @param frame The frame data that includes rendering parameters.
    /// @param scene The scene to apply the composite render pass to.
    void Render(const Frame& frame, Scene& scene) override;

    /// @brief Renders the user interface for configuring composite settings.
    /// 
    /// This method renders the UI elements that allow users to adjust settings related to the composite pass.
    /// 
    /// @param frame The frame data used to render the composite UI.
    void UI(const Frame& frame) override;

private:
    ComputePipeline::Ref mPipeline; ///< A reference to the compute pipeline used for the composite pass.
    RootSignature::Ref mSignature;  ///< A reference to the root signature for the composite pass.
};

