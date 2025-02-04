//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 22:00:58
//

#pragma once

#include <Renderer/RenderPass.hpp>

class Forward : public RenderPass
{
public:
    Forward(RHI::Ref rhi);
    ~Forward() = default;

    void Render(const Frame& frame, Scene& scene) override;
    void UI(const Frame& frame) override;
private:
    MeshPipeline::Ref mPipeline;

    Texture::Ref mDepthBuffer;
    View::Ref mDepthView;
    Array<Buffer::Ref, FRAMES_IN_FLIGHT> mCameraBuffer;
};
