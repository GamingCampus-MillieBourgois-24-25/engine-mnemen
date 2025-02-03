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

    GraphicsPipelineSpecs specs = {};
    specs.Bytecodes[ShaderType::Mesh] = meshShader->Shader;
    specs.Bytecodes[ShaderType::Fragment] = fragmentShader->Shader;
    specs.Formats.push_back(TextureFormat::RGBA8);
    specs.Cull = CullMode::None;
    specs.Fill = FillMode::Solid;
    specs.CCW = false;
    specs.Signature = mRHI->CreateRootSignature({}, 0);
    
    mPipeline = mRHI->CreateMeshPipeline(specs);
}

void Forward::Render(const Frame& frame)
{
    frame.CommandBuffer->BeginMarker("Forward");
    frame.CommandBuffer->Barrier(frame.Backbuffer, ResourceLayout::ColorWrite);
    frame.CommandBuffer->SetViewport(0, 0, frame.Width, frame.Height);
    frame.CommandBuffer->SetRenderTargets({ frame.BackbufferView }, nullptr);
    frame.CommandBuffer->ClearRenderTarget(frame.BackbufferView, 0.0f, 0.0f, 0.0f);
    frame.CommandBuffer->SetMeshPipeline(mPipeline);
    frame.CommandBuffer->DispatchMesh(1);
    frame.CommandBuffer->Barrier(frame.Backbuffer, ResourceLayout::Shader);
    frame.CommandBuffer->EndMarker();
}

void Forward::UI(const Frame& frame)
{

}
