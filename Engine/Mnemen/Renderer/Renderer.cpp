//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 22:02:51
//

#include "Renderer.hpp"
#include "RendererTools.hpp"

#include "Passes/Forward.hpp"
#include "Passes/Composite.hpp"
#include "Passes/DOF.hpp"
#include "Passes/SSAO.hpp"
#include "Passes/ColorGrading.hpp"

#include <Core/Logger.hpp>

Renderer::Renderer(RHI::Ref rhi)
{
    RendererTools::Init(rhi);

    mPasses = {
        MakeRef<Forward>(rhi),
        MakeRef<SSAO>(rhi),
        MakeRef<DOF>(rhi),
        MakeRef<ColorGrading>(rhi),
        MakeRef<Composite>(rhi)
    };

    LOG_INFO("Initialized Render System");
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
