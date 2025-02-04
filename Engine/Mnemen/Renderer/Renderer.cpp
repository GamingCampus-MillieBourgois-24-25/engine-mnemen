//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 22:02:51
//

#include "Renderer.hpp"
#include "RendererTools.hpp"

#include "Passes/Forward.hpp"
#include "Passes/Composite.hpp"

Renderer::Renderer(RHI::Ref rhi)
{
    RendererTools::Init(rhi);

    mPasses = {
        MakeRef<Forward>(rhi),
        MakeRef<Composite>(rhi)
    };
}

Renderer::~Renderer()
{
    mPasses.clear();
}

void Renderer::Render(const Frame& frame, Scene& scene)
{
    for (auto& pass : mPasses) {
        pass->Render(frame, scene);
    }
}

void Renderer::UI(const Frame& frame)
{
    
}
