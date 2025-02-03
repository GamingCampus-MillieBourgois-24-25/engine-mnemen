//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 22:02:51
//

#include "Renderer.hpp"

#include "Passes/Forward.hpp"

Renderer::Renderer(RHI::Ref rhi)
{
    mPasses = {
        MakeRef<Forward>(rhi)
    };
}

Renderer::~Renderer()
{
    mPasses.clear();
}

void Renderer::Render(const Frame& frame)
{
    for (auto& pass : mPasses) {
        pass->Render(frame);
    }
}

void Renderer::UI(const Frame& frame)
{
    
}
