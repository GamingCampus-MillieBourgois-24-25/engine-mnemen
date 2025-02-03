//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-03 19:03:08
//

#include <RHI/RootSignature.hpp>
#include <RHI/Utilities.hpp>
#include <Core/Assert.hpp>
#include <Core/Logger.hpp>

RootSignature::RootSignature(Device::Ref device)
{
    D3D12_ROOT_SIGNATURE_DESC RootSignatureDesc = {};
    RootSignatureDesc.NumParameters = 0;
    RootSignatureDesc.pParameters = nullptr;
    RootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    ID3DBlob* pRootSignatureBlob;
    ID3DBlob* pErrorBlob;
    D3D12SerializeRootSignature(&RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &pRootSignatureBlob, &pErrorBlob);
    if (pErrorBlob) {
        LOG_ERROR("D3D12 Root Signature error! %s", pErrorBlob->GetBufferPointer());
    }
    HRESULT Result = device->GetDevice()->CreateRootSignature(0, pRootSignatureBlob->GetBufferPointer(), pRootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&mRootSignature));
    ASSERT(SUCCEEDED(Result), "Failed to create root signature!");
    pRootSignatureBlob->Release();
}

RootSignature::RootSignature(Device::Ref device, const Vector<RootType>& roots, UInt64 pushConstantSize)
{
    std::vector<D3D12_ROOT_PARAMETER> Parameters(roots.size());
    std::vector<D3D12_DESCRIPTOR_RANGE> Ranges(roots.size());
    
    for (int i = 0; i < roots.size(); i++) {
        D3D12_DESCRIPTOR_RANGE& descriptorRange = Ranges[i];
        descriptorRange.NumDescriptors = 1;
        descriptorRange.BaseShaderRegister = i;
        descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE(roots[i]);
        descriptorRange.RegisterSpace = 0;
        
        D3D12_ROOT_PARAMETER& parameter = Parameters[i];
        if (roots[i] == RootType::PushConstant) {
            parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
            parameter.Constants.ShaderRegister = i;
            parameter.Constants.RegisterSpace = 0;
            parameter.Constants.Num32BitValues = pushConstantSize / 4;
        } else {
            parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            parameter.DescriptorTable.NumDescriptorRanges = 1;
            parameter.DescriptorTable.pDescriptorRanges = &descriptorRange;
            parameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
        }
    }

    D3D12_ROOT_SIGNATURE_DESC RootSignatureDesc = {};
    RootSignatureDesc.NumParameters = Parameters.size();
    RootSignatureDesc.pParameters = Parameters.data();
    RootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
                            | D3D12_ROOT_SIGNATURE_FLAG_CBV_SRV_UAV_HEAP_DIRECTLY_INDEXED
                            | D3D12_ROOT_SIGNATURE_FLAG_SAMPLER_HEAP_DIRECTLY_INDEXED;

    ID3DBlob* pRootSignatureBlob;
    ID3DBlob* pErrorBlob;
    D3D12SerializeRootSignature(&RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &pRootSignatureBlob, &pErrorBlob);
    if (pErrorBlob) {
        LOG_ERROR("D3D12 Root Signature error! %s", pErrorBlob->GetBufferPointer());
    }
    HRESULT Result = device->GetDevice()->CreateRootSignature(0, pRootSignatureBlob->GetBufferPointer(), pRootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&mRootSignature));
    ASSERT(SUCCEEDED(Result), "Failed to create root signature!");
    pRootSignatureBlob->Release();
}

RootSignature::~RootSignature()
{
    D3DUtils::Release(mRootSignature);
}
