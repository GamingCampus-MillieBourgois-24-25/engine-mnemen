//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 22:02:51
//

#include "Renderer.hpp"
#include "RendererTools.hpp"
#include "SkyboxCooker.hpp"

#include "Passes/GBuffer.hpp"
#include "Passes/Deferred.hpp"
#include "Passes/Composite.hpp"
#include "Passes/DOF.hpp"
#include "Passes/SSAO.hpp"
#include "Passes/ColorGrading.hpp"
#include "Passes/Debug.hpp"

#include <Core/Logger.hpp>
#include <Core/Profiler.hpp>

#include <imgui.h>
#include <FontAwesome/FontAwesome.hpp>

Renderer::Renderer(RHI::Ref rhi)
{
    RendererTools::Init(rhi);
    SkyboxCooker::Init(rhi);

    mPasses = {
        MakeRef<GBuffer>(rhi),
        MakeRef<SSAO>(rhi),
        MakeRef<Deferred>(rhi),
        MakeRef<DOF>(rhi),
        MakeRef<ColorGrading>(rhi),
        MakeRef<Composite>(rhi),
        MakeRef<Debug>(rhi)
    };

    LOG_INFO("Initialized Render System");
}

Renderer::~Renderer()
{
    mPasses.clear();
}

void Renderer::Render(const Frame& frame, ::Ref<Scene> scene)
{
    PROFILE_FUNCTION();
    for (auto& pass : mPasses) {
        pass->Render(frame, scene);
    }
}
