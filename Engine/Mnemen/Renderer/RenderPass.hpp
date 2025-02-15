//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 22:00:07
//

#pragma once

#include <RHI/RHI.hpp>

#include "World/Scene.hpp"
#include "RendererTools.hpp"

/// @brief A class representing a render pass in the rendering pipeline.
/// 
/// The `RenderPass` class serves as a base class for defining specific render passes within 
/// the rendering pipeline. Each render pass processes the scene and renders it according to 
/// its purpose (e.g., forward rendering, shadow rendering, etc.). It also handles UI updates 
/// during the rendering process.
class RenderPass
{
public:
    /// @brief A reference to a `RenderPass` object.
    using Ref = Ref<RenderPass>;

    /// @brief Constructs a `RenderPass` object with a specified rendering hardware interface (RHI).
    /// 
    /// This constructor initializes the render pass with the given RHI, which is responsible 
    /// for executing GPU operations during the rendering process.
    /// 
    /// @param rhi The RHI reference to be used for rendering operations.
    RenderPass(RHI::Ref rhi);

    /// @brief Destroys the `RenderPass` object.
    /// 
    /// The destructor releases any resources associated with the render pass, ensuring proper 
    /// cleanup. The render pass is responsible for its own cleanup when it goes out of scope.
    ~RenderPass() = default;

    /// @brief Renders the scene for this render pass.
    /// 
    /// This pure virtual function defines how a particular render pass should render the given 
    /// frame and scene. Derived classes will implement this method to define the specifics 
    /// of rendering (e.g., forward pass, deferred pass, etc.).
    /// 
    /// @param frame The current frame that holds information such as time and buffers.
    /// @param scene The scene to be rendered during this pass.
    virtual void Render(const Frame& frame, ::Ref<Scene> scene) = 0;

protected:
    RHI::Ref mRHI; ///< The rendering hardware interface (RHI) used for GPU operations during this pass.
};