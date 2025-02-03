//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-03 19:17:51
//

#include <RHI/GraphicsPipeline.hpp>
#include <RHI/Utilities.hpp>
#include <Core/Assert.hpp>
#include <Core/Logger.hpp>

GraphicsPipeline::GraphicsPipeline(Device::Ref device, GraphicsPipelineSpecs& specs)
{
    Shader& vertexBytecode = specs.Bytecodes[ShaderType::Vertex];
    Shader& fragmentBytecode = specs.Bytecodes[ShaderType::Fragment];

    ID3D12ShaderReflection* pVertexReflection = ShaderCompiler::Reflect(vertexBytecode);
    D3D12_SHADER_DESC vertexDesc;
    pVertexReflection->GetDesc(&vertexDesc);

    Vector<D3D12_INPUT_ELEMENT_DESC> InputElementDescs;
    Vector<std::string> InputElementSemanticNames;

    D3D12_GRAPHICS_PIPELINE_STATE_DESC Desc = {};
    Desc.VS.pShaderBytecode = vertexBytecode.Bytecode.data();
    Desc.VS.BytecodeLength = vertexBytecode.Bytecode.size();
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
        Desc.NumRenderTargets = specs.Formats.size();
    }
    Desc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    Desc.RasterizerState.FillMode = D3D12_FILL_MODE(specs.Fill);
    Desc.RasterizerState.CullMode = D3D12_CULL_MODE(specs.Cull);
    Desc.RasterizerState.DepthClipEnable = false;
    Desc.RasterizerState.FrontCounterClockwise = specs.CCW;
    Desc.PrimitiveTopologyType = specs.Line ? D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE : D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    if (specs.DepthEnabled) {
        Desc.DepthStencilState.DepthEnable = true;
        if (specs.DepthWrite)
            Desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
        else
            Desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
        Desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC(specs.Depth);
        Desc.DSVFormat = DXGI_FORMAT(specs.DepthFormat);
        if (specs.DepthClampEnable) {
            Desc.RasterizerState.DepthBias = 0;
            Desc.RasterizerState.DepthBiasClamp = 0.0f;
            Desc.RasterizerState.SlopeScaledDepthBias = 0.0f;
        }
    }
    Desc.SampleDesc.Count = 1;

    InputElementSemanticNames.reserve(vertexDesc.InputParameters);
    InputElementDescs.reserve(vertexDesc.InputParameters);

    for (int ParameterIndex = 0; ParameterIndex < vertexDesc.InputParameters; ParameterIndex++)
    {
        D3D12_SIGNATURE_PARAMETER_DESC ParameterDesc = {};
        pVertexReflection->GetInputParameterDesc(ParameterIndex, &ParameterDesc);

        InputElementSemanticNames.push_back(ParameterDesc.SemanticName);

        D3D12_INPUT_ELEMENT_DESC InputElement = {};
        InputElement.SemanticName = InputElementSemanticNames.back().c_str();
        InputElement.SemanticIndex = ParameterDesc.SemanticIndex;
        InputElement.InputSlot = 0;
        InputElement.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
        InputElement.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
        InputElement.InstanceDataStepRate = 0;

        if (ParameterDesc.Mask == 1)
        {
            if (ParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) InputElement.Format = DXGI_FORMAT_R32_UINT;
            else if (ParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) InputElement.Format = DXGI_FORMAT_R32_SINT;
            else if (ParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) InputElement.Format = DXGI_FORMAT_R32_FLOAT;
        }
        else if (ParameterDesc.Mask <= 3)
        {
            if (ParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) InputElement.Format = DXGI_FORMAT_R32G32_UINT;
            else if (ParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) InputElement.Format = DXGI_FORMAT_R32G32_SINT;
            else if (ParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) InputElement.Format = DXGI_FORMAT_R32G32_FLOAT;
        }
        else if (ParameterDesc.Mask <= 7)
        {
            if (ParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) InputElement.Format = DXGI_FORMAT_R32G32B32_UINT;
            else if (ParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) InputElement.Format = DXGI_FORMAT_R32G32B32_SINT;
            else if (ParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) InputElement.Format = DXGI_FORMAT_R32G32B32_FLOAT;
        }
        else if (ParameterDesc.Mask <= 15)
        {
            if (ParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) InputElement.Format = DXGI_FORMAT_R32G32B32A32_UINT;
            else if (ParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) InputElement.Format = DXGI_FORMAT_R32G32B32A32_SINT;
            else if (ParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) InputElement.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        }

        InputElementDescs.push_back(InputElement);
    }
    Desc.InputLayout.pInputElementDescs = InputElementDescs.data();
    Desc.InputLayout.NumElements = static_cast<uint32_t>(InputElementDescs.size());

    if (specs.Signature) {
        Desc.pRootSignature = specs.Signature->GetSignature();
        mSignature = specs.Signature;
    }

    HRESULT result = device->GetDevice()->CreateGraphicsPipelineState(&Desc, IID_PPV_ARGS(&mPipeline));
    ASSERT(SUCCEEDED(result), "Failed to create graphics pipeline!");
}

GraphicsPipeline::~GraphicsPipeline()
{
    D3DUtils::Release(mPipeline);
}
