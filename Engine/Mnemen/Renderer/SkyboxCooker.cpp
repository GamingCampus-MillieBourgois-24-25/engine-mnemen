//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-17 16:24:41
//

#include "SkyboxCooker.hpp"

#include <RHI/Uploader.hpp>
#include <Core/Timer.hpp>

SkyboxCooker::Data SkyboxCooker::sData;

void SkyboxCooker::Init(RHI::Ref rhi)
{
    sData.RHI = rhi;
    sData.Sampler = rhi->CreateSampler(SamplerAddress::Wrap, SamplerFilter::Linear, false);

    // Skybox gen
    {
        auto shader = AssetManager::Get("Assets/Shaders/SkyboxGeneration/Compute.hlsl", AssetType::Shader);
        auto signature = rhi->CreateRootSignature({ RootType::PushConstant }, sizeof(int) * 4);
        sData.SkyboxCompute = rhi->CreateComputePipeline(shader->Shader, signature);
    }
    // Irradiance gen
    {
        auto shader = AssetManager::Get("Assets/Shaders/Irradiance/Compute.hlsl", AssetType::Shader);
        auto signature = rhi->CreateRootSignature({ RootType::PushConstant }, sizeof(int) * 4);
        sData.IrradianceCompute = rhi->CreateComputePipeline(shader->Shader, signature);
    }
    // Prefilter gen
    {
        auto shader = AssetManager::Get("Assets/Shaders/Prefilter/Compute.hlsl", AssetType::Shader);
        auto signature = rhi->CreateRootSignature({ RootType::PushConstant }, sizeof(int) * 4);
        sData.PrefilterCompute = rhi->CreateComputePipeline(shader->Shader, signature);
    }

    LOG_INFO("Initialized Skybox Cooker!");
}

void SkyboxCooker::GenerateSkybox(Ref<Skybox> skybox)
{
    Timer timer;

    Image image;
    image.LoadHDR(skybox->Path);

    Texture::Ref hdri = nullptr;

    // Create HDRI
    {
        TextureDesc desc;
        desc.Width = image.Width;
        desc.Height = image.Height;
        desc.Depth = 1;
        desc.Levels = 1;
        desc.Format = TextureFormat::RGBA16Float;
        desc.Name = skybox->Path + " HDRI";
        desc.Usage = TextureUsage::ShaderResource | TextureUsage::Storage;

        hdri = sData.RHI->CreateTexture(desc);
        Uploader::EnqueueTextureUpload(image, hdri);
        Uploader::Flush();
    }

    // Create env map
    {
        TextureDesc desc;
        desc.Width = 512;
        desc.Height = 512;
        desc.Depth = 6;
        desc.Format = TextureFormat::RGBA16Float;
        desc.Name = skybox->Path + " Skybox";
        desc.Usage = TextureUsage::ShaderResource | TextureUsage::Storage;
        desc.Levels = 1;

        skybox->EnvironmentMap = sData.RHI->CreateTexture(desc);
    }

    // Create irradiance map
    {
        TextureDesc desc;
        desc.Width = 32;
        desc.Height = 32;
        desc.Depth = 6;
        desc.Format = TextureFormat::RGBA16Float;
        desc.Name = skybox->Path + " Irradiance";
        desc.Usage = TextureUsage::ShaderResource | TextureUsage::Storage;
        desc.Levels = 1;

        skybox->IrradianceMap = sData.RHI->CreateTexture(desc);
    }

    // Create prefilter map
    {
        TextureDesc desc;
        desc.Width = 512;
        desc.Height = 512;
        desc.Depth = 6;
        desc.Format = TextureFormat::RGBA16Float;
        desc.Name = skybox->Path + " Irradiance";
        desc.Usage = TextureUsage::ShaderResource | TextureUsage::Storage;
        desc.Levels = 5;

        skybox->IrradianceMap = sData.RHI->CreateTexture(desc);
    }

    // Create command buffer
    CommandBuffer::Ref cmdBuffer = sData.RHI->CreateCommandBuffer(true);
    cmdBuffer->Begin();

    // Create views
    int levels = skybox->PrefilterMap->GetDesc().Levels;

    skybox->EnvironmentMapSRV = sData.RHI->CreateView(skybox->EnvironmentMap, ViewType::ShaderResource, ViewDimension::TextureCube);
    skybox->IrradianceMapSRV = sData.RHI->CreateView(skybox->IrradianceMap, ViewType::ShaderResource, ViewDimension::TextureCube);
    skybox->PrefilterMapSRV = sData.RHI->CreateView(skybox->PrefilterMap, ViewType::ShaderResource, ViewDimension::TextureCube);

    View::Ref hdrSRV = sData.RHI->CreateView(hdri, ViewType::ShaderResource);
    View::Ref envUAV = sData.RHI->CreateView(skybox->EnvironmentMap, ViewType::Storage, ViewDimension::TextureCube); 
    View::Ref irrUAV = sData.RHI->CreateView(skybox->IrradianceMap, ViewType::Storage, ViewDimension::TextureCube);
    Vector<View::Ref> prefilterUAVs;
    for (int i = 0; i < levels; i++) {
        prefilterUAVs.push_back(sData.RHI->CreateView(skybox->PrefilterMap, ViewType::Storage, ViewDimension::TextureCube, TextureFormat::Unknown, i));
    }

    // Generate skybox
    {
        struct Data {
            int hdrIn;
            int envOut;
            int sampler;
            int pad;
        } data = {
            hdrSRV->GetDescriptor().Index,
            envUAV->GetDescriptor().Index,
            sData.Sampler->BindlesssSampler(),
            0
        };

        cmdBuffer->SetComputePipeline(sData.SkyboxCompute);
        cmdBuffer->ComputePushConstants(&data, sizeof(data), 0);
        cmdBuffer->Dispatch(512 / 32, 512 / 32, 6);
    }

    // Generate irradiance
    {        
        struct Data {
            int envIn;
            int irrOut;
            int sampler;
            int pad;
        } data = {
            skybox->EnvironmentMapSRV->GetDescriptor().Index,
            irrUAV->GetDescriptor().Index,
            sData.Sampler->BindlesssSampler(),
            0
        };

        cmdBuffer->SetComputePipeline(sData.IrradianceCompute);
        cmdBuffer->ComputePushConstants(&data, sizeof(data), 0);
        cmdBuffer->Dispatch(32 / 32, 32 / 32, 6);
    }

    // Generate prefilter
    {
        int envSize = skybox->PrefilterMap->GetDesc().Width;
        const float deltaRoughness = 1.0f / std::max(float(levels - 1u), 1.0f);

        cmdBuffer->SetComputePipeline(sData.PrefilterCompute);
        for (int level = 0, size = envSize; level < levels; ++level, size /= 2) {
            const UInt32 numGroups = std::max(1u, size / 32u);

            struct PushConstant {
                int envIn;
                int prefilterOut;
                int sampler;
                float roughness;
            } data = {
                skybox->EnvironmentMapSRV->GetDescriptor().Index,
                prefilterUAVs[level]->GetDescriptor().Index,
                sData.Sampler->BindlesssSampler(),
                level * deltaRoughness
            };

            cmdBuffer->ComputePushConstants(&data, sizeof(data), 0);
            cmdBuffer->Dispatch(numGroups, numGroups, 6);
        }
    }

    // Finish and execute
    cmdBuffer->End();
    sData.RHI->Submit({ cmdBuffer });
    sData.RHI->Wait();
}
