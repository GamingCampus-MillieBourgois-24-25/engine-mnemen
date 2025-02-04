//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 22:14:31
//

#include "MeshPipeline.hpp"

#include <Core/Logger.hpp>

MeshPipeline::MeshPipeline(Device::Ref devicePtr, GraphicsPipelineSpecs& specs)
{
    Shader& meshBytecode = specs.Bytecodes[ShaderType::Mesh];
    Shader& fragmentBytecode = specs.Bytecodes[ShaderType::Fragment];

    D3DX12_MESH_SHADER_PIPELINE_STATE_DESC Desc = {};
    if (specs.UseAmplification) {
        // Desc.AS.pShaderBytecode = ampBytecode.bytecode.data();
        // Desc.AS.BytecodeLength = ampBytecode.bytecode.size() * sizeof(UInt32);
    }
    Desc.MS.pShaderBytecode = meshBytecode.Bytecode.data();
    Desc.MS.BytecodeLength = meshBytecode.Bytecode.size();
    Desc.PS.pShaderBytecode = fragmentBytecode.Bytecode.data();
    Desc.PS.BytecodeLength = fragmentBytecode.Bytecode.size();
    for (int RTVIndex = 0; RTVIndex < specs.Formats.size(); RTVIndex++) {
        Desc.BlendState.RenderTarget[RTVIndex].SrcBlend = D3D12_BLEND_ONE;
        Desc.BlendState.RenderTarget[RTVIndex].DestBlend = D3D12_BLEND_ZERO;
        Desc.BlendState.RenderTarget[RTVIndex].BlendOp = D3D12_BLEND_OP_ADD;
        Desc.BlendState.RenderTarget[RTVIndex].SrcBlendAlpha = D3D12_BLEND_ONE;
        Desc.BlendState.RenderTarget[RTVIndex].DestBlendAlpha = D3D12_BLEND_ZERO;
        Desc.BlendState.RenderTarget[RTVIndex].BlendOpAlpha = D3D12_BLEND_OP_ADD;
        Desc.BlendState.RenderTarget[RTVIndex].LogicOp = D3D12_LOGIC_OP_NOOP;
        Desc.BlendState.RenderTarget[RTVIndex].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

        Desc.RTVFormats[RTVIndex] = DXGI_FORMAT(specs.Formats[RTVIndex]);
    }
    Desc.NumRenderTargets = specs.Formats.size();
    Desc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    Desc.RasterizerState.FillMode = D3D12_FILL_MODE(specs.Fill);
    Desc.RasterizerState.CullMode = D3D12_CULL_MODE(specs.Cull);
    Desc.RasterizerState.DepthClipEnable = !specs.DepthClampEnable;
    Desc.RasterizerState.FrontCounterClockwise = specs.CCW;
    Desc.PrimitiveTopologyType = specs.Line ? D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE : D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    if (specs.DepthEnabled) {
        Desc.DepthStencilState.DepthEnable = true;
        Desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
        Desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC(specs.Depth);
        Desc.DSVFormat = DXGI_FORMAT(specs.DepthFormat);
    }
    Desc.SampleDesc.Count = 1;
    if (specs.Signature) {
        Desc.pRootSignature = specs.Signature->GetSignature();
        mSignature = specs.Signature;
    }

    auto Stream = CD3DX12_PIPELINE_MESH_STATE_STREAM(Desc);

    D3D12_PIPELINE_STATE_STREAM_DESC streamDesc;
    streamDesc.pPipelineStateSubobjectStream = &Stream;
    streamDesc.SizeInBytes = sizeof(Stream);

    HRESULT Result = devicePtr->GetDevice()->CreatePipelineState(&streamDesc, IID_PPV_ARGS(&mPipeline));
    if (FAILED(Result)) {
        LOG_ERROR("[D3D12] Failed creating D3D12 graphics pipeline!");
        return;
    }
}

MeshPipeline::~MeshPipeline()
{
    mPipeline->Release();
}
