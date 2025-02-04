//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 11:22:11
//

#pragma once

#include <Renderer/RenderPass.hpp>

class Composite : public RenderPass
{
public:
    Composite(RHI::Ref rhi);
    ~Composite() = default;

    void Bake(Scene& scene) {}
    void Render(const Frame& frame, Scene& scene) override;
    void UI(const Frame& frame) override;
private:
    ComputePipeline::Ref mPipeline;
    RootSignature::Ref mSignature;
};
