//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 21:58:44
//

#pragma once

#include "../RHI/RHI.hpp"
#include "RenderPass.hpp"

class Renderer
{
public:
    using Ref = Ref<Renderer>;

    Renderer(RHI::Ref rhi);
    ~Renderer();

    void Render(const Frame& frame);
    void UI(const Frame& frame);
private:
    Vector<RenderPass::Ref> mPasses;
};
