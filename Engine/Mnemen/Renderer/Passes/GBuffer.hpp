//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-17 10:10:38
//

#pragma once

#include <Renderer/RenderPass.hpp>

class GBuffer : public RenderPass
{
public:
    GBuffer(RHI::Ref rhi);
    ~GBuffer() = default;

    void Render(const Frame& frame, ::Ref<Scene> scene) override;
private:
    MeshPipeline::Ref mPipeline;
};
