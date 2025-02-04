//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 22:01:27
//

#include "Forward.hpp"

#include <Asset/AssetManager.hpp>

Forward::Forward(RHI::Ref rhi)
    : RenderPass(rhi)
{
    Asset::Handle meshShader = AssetManager::Get("Assets/Shaders/Forward/ForwardMesh.hlsl", AssetType::Shader);
    Asset::Handle fragmentShader = AssetManager::Get("Assets/Shaders/Forward/ForwardFragment.hlsl", AssetType::Shader);

    // Depth buffer
    {
        TextureDesc desc = {};
        desc.Width = 1280;
        desc.Height = 720;
        desc.Levels = 1;
        desc.Depth = 1;
        desc.Name = "Depth Buffer";
        desc.Usage = TextureUsage::DepthTarget;
        desc.Format = TextureFormat::Depth32;
        mDepthBuffer = mRHI->CreateTexture(desc);

        mDepthView = mRHI->CreateView(mDepthBuffer, ViewType::DepthTarget);
    }

    GraphicsPipelineSpecs specs = {};
    specs.Bytecodes[ShaderType::Mesh] = meshShader->Shader;
    specs.Bytecodes[ShaderType::Fragment] = fragmentShader->Shader;
    specs.Formats.push_back(TextureFormat::RGBA8);
    specs.Cull = CullMode::None;
    specs.Fill = FillMode::Solid;
    specs.Depth = DepthOperation::Less;
    specs.DepthEnabled = true;
    specs.DepthFormat = TextureFormat::Depth32;
    specs.CCW = false;
    specs.Signature = mRHI->CreateRootSignature({ RootType::PushConstant }, sizeof(glm::mat4) * 2);
    
    mPipeline = mRHI->CreateMeshPipeline(specs);
}

void Forward::Render(const Frame& frame, Scene& scene)
{
    SceneCamera camera = scene.GetMainCamera();

    frame.CommandBuffer->BeginMarker("Forward");
    frame.CommandBuffer->Barrier(frame.Backbuffer, ResourceLayout::ColorWrite);
    frame.CommandBuffer->Barrier(mDepthBuffer, ResourceLayout::DepthWrite);
    frame.CommandBuffer->SetViewport(0, 0, frame.Width, frame.Height);
    frame.CommandBuffer->SetRenderTargets({ frame.BackbufferView }, mDepthView);
    frame.CommandBuffer->ClearRenderTarget(frame.BackbufferView, 0.0f, 0.0f, 0.0f);
    frame.CommandBuffer->ClearDepth(mDepthView);
    frame.CommandBuffer->SetMeshPipeline(mPipeline);
    frame.CommandBuffer->GraphicsPushConstants(&camera, sizeof(camera), 0);
    frame.CommandBuffer->DispatchMesh(1);
    frame.CommandBuffer->Barrier(frame.Backbuffer, ResourceLayout::Shader);
    frame.CommandBuffer->Barrier(mDepthBuffer, ResourceLayout::Common);
    frame.CommandBuffer->EndMarker();
}

void Forward::UI(const Frame& frame)
{

}
