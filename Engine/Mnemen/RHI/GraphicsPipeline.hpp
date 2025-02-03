//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-03 19:11:45
//

#pragma once

#include <RHI/RootSignature.hpp>
#include <RHI/Texture.hpp>
#include <Asset/Shader.hpp>

enum class FillMode
{
    Solid = D3D12_FILL_MODE_SOLID,
    Line = D3D12_FILL_MODE_WIREFRAME
};

enum class CullMode
{
    Back = D3D12_CULL_MODE_BACK,
    Front = D3D12_CULL_MODE_FRONT,
    None = D3D12_CULL_MODE_NONE
};

enum class DepthOperation
{
    Greater = D3D12_COMPARISON_FUNC_GREATER,
    Less = D3D12_COMPARISON_FUNC_LESS,
    Equal = D3D12_COMPARISON_FUNC_EQUAL,
    LEqual = D3D12_COMPARISON_FUNC_LESS_EQUAL,
    None = D3D12_COMPARISON_FUNC_NONE
};

struct GraphicsPipelineSpecs
{
    FillMode Fill = FillMode::Solid;
    CullMode Cull = CullMode::None;
    bool CCW = true;
    bool Line = false;

    Vector<TextureFormat> Formats;
    DepthOperation Depth = DepthOperation::None;
    TextureFormat DepthFormat = TextureFormat::Unknown;
    bool DepthEnabled = false;
    bool DepthClampEnable = false;
    bool DepthWrite = true;

    UnorderedMap<ShaderType, Shader> Bytecodes;
    RootSignature::Ref Signature = nullptr;
};

class GraphicsPipeline
{
public:
    using Ref = Ref<GraphicsPipeline>;

    GraphicsPipeline(Device::Ref device, GraphicsPipelineSpecs& specs);
    ~GraphicsPipeline();

    ID3D12PipelineState* GetPipeline() { return mPipeline; }
    RootSignature::Ref GetRootSignature() { return mSignature; }
private:
    ID3D12PipelineState* mPipeline = nullptr;
    RootSignature::Ref mSignature = nullptr;
};
