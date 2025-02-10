//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 12:06:45
//

#include "SSAO.hpp"
#include "RHI/Uploader.hpp"
#include <Core/Application.hpp>
#include <random>

float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

SSAO::SSAO(RHI::Ref rhi)
    : RenderPass(rhi)
{
    int width, height;
    Application::Get()->GetWindow()->PollSize(width, height);

    std::uniform_real_distribution<float> randomFloats(0.0, 1.0); // random floats between [0.0, 1.0]
    std::default_random_engine generator;
    std::vector<glm::vec3> ssaoKernel;
    for (unsigned int i = 0; i < 64; ++i)
    {
        glm::vec3 sample(
            randomFloats(generator) * 2.0 - 1.0, 
            randomFloats(generator) * 2.0 - 1.0, 
            randomFloats(generator)
        );
        float scale = (float)i / 64.0; 
        scale   = lerp(0.1f, 1.0f, scale * scale);
        sample *= scale;
        ssaoKernel.push_back(sample);  
    }

    std::vector<glm::vec3> ssaoNoise;
    for (unsigned int i = 0; i < 16; i++)
    {
        glm::vec3 noise(
            randomFloats(generator) * 2.0 - 1.0, 
            randomFloats(generator) * 2.0 - 1.0, 
            0.0f); 
        ssaoNoise.push_back(noise);
    }
    
    std::vector<UInt8> bytes;
    bytes.resize(ssaoNoise.size() * sizeof(glm::vec3));
    memcpy(bytes.data(), ssaoNoise.data(), bytes.size());

    TextureDesc desc = {};
    desc.Width = 4;
    desc.Height = 4;
    desc.Levels = 1;
    desc.Depth = 1;
    desc.Name = "Noise_Texture";
    desc.Usage = TextureUsage::RenderTarget | TextureUsage::Storage | TextureUsage::ShaderResource;
    desc.Format = TextureFormat::RGBA16Float;
    
    auto ldr = RendererTools::CreateSharedTexture("Noise_Texture", desc);
    ldr->AddView(ViewType::Storage);
    ldr->AddView(ViewType::ShaderResource);

    Uploader::EnqueueTextureUpload(bytes, ldr->Texture);

    TextureDesc text2 = {};
    text2.Width = width;
    text2.Height = height;   
    text2.Levels = 0;
    text2.Depth = 1;
    text2.Name = "ssaoColorBuffer";
    text2.Usage = TextureUsage::RenderTarget | TextureUsage::Storage | TextureUsage::ShaderResource;
    text2.Format = TextureFormat::R32Float;
    
    auto scb = RendererTools::CreateSharedTexture("ssaoColorBuffer", desc);
    scb->AddView(ViewType::Storage);
    scb->AddView(ViewType::ShaderResource);
}

void SSAO::Render(const Frame& frame, ::Ref<Scene> scene)
{
    // Alex
}

void SSAO::UI(const Frame& frame)
{
    // Alex
}
