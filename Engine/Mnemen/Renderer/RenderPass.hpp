//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 22:00:07
//

#pragma once

#include <RHI/RHI.hpp>

class RenderPass
{
public:
    using Ref = Ref<RenderPass>;

    RenderPass(RHI::Ref rhi);
    ~RenderPass() = default;

    virtual void Render(const Frame& frame) = 0;
    virtual void UI(const Frame& frame) = 0;
protected:
    RHI::Ref mRHI;
};