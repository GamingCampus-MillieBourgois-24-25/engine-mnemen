//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 22:00:58
//

#pragma once

#include <Renderer/RenderPass.hpp>

/// @brief A class that implements the forward rendering pass.
///
/// The `Forward` class is a specific render pass that uses the forward rendering technique to render
/// the scene. In forward rendering, each object in the scene is rendered with its full lighting 
/// computation in a single pass, which can be simpler but less efficient for scenes with many lights. 
/// This class extends the `RenderPass` class.
class Forward : public RenderPass
{
public:
    /// @brief Constructs a Forward render pass with the specified rendering hardware interface (RHI).
    /// 
    /// This constructor initializes the forward render pass, which is responsible for rendering the 
    /// scene using the forward rendering technique. The RHI is passed to the base class `RenderPass`
    /// for managing hardware-specific rendering operations.
    /// 
    /// @param rhi The rendering hardware interface (RHI) used for the forward render pass.
    Forward(RHI::Ref rhi);

    /// @brief Destructor for the Forward class.
    /// 
    /// The destructor is defaulted and ensures proper cleanup of any resources used by the forward
    /// rendering pass.
    ~Forward() = default;

    /// @brief Executes the forward rendering pass for a given frame and scene.
    /// 
    /// This method renders the scene using the forward rendering technique. Each object in the scene
    /// is rendered with its complete lighting and shading applied in a single pass.
    /// 
    /// @param frame The frame data that includes rendering parameters.
    /// @param scene The scene to be rendered.
    void Render(const Frame& frame, ::Ref<Scene> scene) override;

    /// @brief Renders the user interface for configuring forward pass settings.
    /// 
    /// This method renders the UI elements that allow users to adjust settings related to the forward 
    /// rendering pass, such as lighting.
    /// 
    /// @param frame The frame data used to render the forward rendering UI.
    void UI(const Frame& frame) override;

private:
    MeshPipeline::Ref mPipeline; ///< A reference to the mesh pipeline used for rendering.
};

