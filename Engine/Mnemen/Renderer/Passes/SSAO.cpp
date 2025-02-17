//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 12:06:45
//

#include "SSAO.hpp"
#include "RHI/Uploader.hpp"
#include <Core/Application.hpp>
#include <random>
#include <imgui.h>

float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

SSAO::SSAO(RHI::Ref rhi)
    : RenderPass(rhi)
{
    int width, height;
    Application::Get()->GetWindow()->PollSize(width, height);

    TextureDesc text2 = {};
    text2.Width = width;
    text2.Height = height;   
    text2.Levels = 1;
    text2.Depth = 1;
    text2.Name = "ssaoColorBuffer";
    text2.Usage = TextureUsage::RenderTarget | TextureUsage::Storage | TextureUsage::ShaderResource;
    text2.Format = TextureFormat::R32Float;
    
    auto scb = RendererTools::CreateSharedTexture("ssaoColorBuffer", text2);
    scb->AddView(ViewType::Storage);
    scb->AddView(ViewType::ShaderResource);
}

void SSAO::Render(const Frame& frame, ::Ref<Scene> scene) {
    
}

void SSAO::UI(const Frame& frame)
{
    // Alex

    if (ImGui::TreeNodeEx("SSAO", ImGuiTreeNodeFlags_Framed)) {
        ImGui::TreePop();
    }  
}
