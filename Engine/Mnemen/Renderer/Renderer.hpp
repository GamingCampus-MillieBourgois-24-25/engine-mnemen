//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 21:58:44
//

#pragma once

#include <RHI/RHI.hpp>
#include <World/Scene.hpp>

#include "RenderPass.hpp"
#include "Passes/Debug.hpp"

/// @brief A class responsible for managing and executing rendering passes.
///
/// The `Renderer` class encapsulates the rendering pipeline, managing multiple render passes 
/// and providing functionality to render a frame and update the UI. It interfaces with the 
/// rendering hardware interface (RHI) to execute the actual rendering work.
class Renderer
{
public:
    /// @brief A reference type for the `Renderer` class, using shared pointers.
    using Ref = Ref<Renderer>;

    /// @brief Constructs a `Renderer` instance with the specified rendering hardware interface (RHI).
    /// 
    /// This constructor initializes the renderer with the given RHI, which is responsible for
    /// managing the hardware-specific rendering operations.
    /// 
    /// @param rhi The rendering hardware interface (RHI) used for the renderer.
    Renderer(RHI::Ref rhi);

    /// @brief Destructor for the `Renderer` class.
    /// 
    /// Cleans up any resources used by the renderer, including render passes and RHI-related data.
    ~Renderer();

    /// @brief Executes the rendering process for a given frame and scene.
    /// 
    /// This method renders the scene for the specified frame, invoking the appropriate render passes
    /// in the correct order.
    /// 
    /// @param frame The frame data that includes rendering parameters.
    /// @param scene The scene to be rendered, which contains all the objects and data for rendering.
    void Render(const Frame& frame, ::Ref<Scene> scene);

    /// @brief Renders the user interface for the specified frame.
    /// 
    /// This method handles the UI rendering for the given frame. It typically includes overlay elements,
    /// HUD elements, or other UI components handled by ImGui.
    /// 
    /// @param frame The frame data used to render the user interface.
    void UI(const Frame& frame);

private:
    Vector<RenderPass::Ref> mPasses; ///< A collection of render passes associated with the renderer.
};

