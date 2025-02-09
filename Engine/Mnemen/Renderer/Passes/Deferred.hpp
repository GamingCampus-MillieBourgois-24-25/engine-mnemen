//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 22:00:58
//

#pragma once

#include <Renderer/RenderPass.hpp>

/// @brief A class that implements the deferred rendering pass.
///
/// The `Deferred` class is a specific render pass that uses the deferred rendering technique to render
/// the scene.
/// This class extends the `RenderPass` class.
class Deferred : public RenderPass
{
public:
    /// @brief Constructs a Deferred render pass with the specified rendering hardware interface (RHI).
    /// 
    /// This constructor initializes the deferred render pass, which is responsible for rendering the 
    /// scene using the deferred rendering technique. The RHI is passed to the base class `RenderPass`
    /// for managing hardware-specific rendering operations.
    /// 
    /// @param rhi The rendering hardware interface (RHI) used for the deferred render pass.
    Deferred(RHI::Ref rhi);

    /// @brief Destructor for the Deferred class.
    /// 
    /// The destructor is defaulted and ensures proper cleanup of any resources used by the deferred
    /// rendering pass.
    ~Deferred() = default;

    /// @brief Executes the deferred rendering pass for a given frame and scene.
    /// 
    /// This method renders the scene using the deferred rendering technique.
    /// 
    /// @param frame The frame data that includes rendering parameters.
    /// @param scene The scene to be rendered.
    void Render(const Frame& frame, ::Ref<Scene> scene) override;

    /// @brief Renders the user interface for configuring deferred pass settings.
    /// 
    /// This method renders the UI elements that allow users to adjust settings related to the deferred 
    /// rendering pass, such as lighting.
    /// 
    /// @param frame The frame data used to render the deferred rendering UI.
    void UI(const Frame& frame) override;

private:
    MeshPipeline::Ref mPipeline; ///< A reference to the mesh pipeline used for rendering.
    ComputePipeline::Ref mLightPipeline; ///< A reference to the light pipeline used for calculating screen space lighting.
};

