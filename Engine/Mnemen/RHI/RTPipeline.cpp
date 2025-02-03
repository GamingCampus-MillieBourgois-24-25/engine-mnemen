//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-18 22:51:24
//

#include <RHI/RTPipeline.hpp>
#include <Core/Logger.hpp>

RTPipeline::RTPipeline(Device::Ref device, DescriptorHeaps& heaps, RTPipelineSpecs specs)
{
    if (!specs.Signature) {
        LOG_CRITICAL("ho is u not giving a root signature to the rt pipeline?!");
        return;
    }

    mSignature = specs.Signature;

    D3D12_DXIL_LIBRARY_DESC lib = {};
    lib.DXILLibrary.BytecodeLength = specs.Library.Bytecode.size() * sizeof(uint32_t);
    lib.DXILLibrary.pShaderBytecode = specs.Library.Bytecode.data();
    lib.NumExports = 0;

    D3D12_HIT_GROUP_DESC hitGroup = {};
    hitGroup.HitGroupExport = L"HitGroup";
    hitGroup.Type = D3D12_HIT_GROUP_TYPE_TRIANGLES;
    hitGroup.ClosestHitShaderImport = L"ClosestHit";

    D3D12_RAYTRACING_SHADER_CONFIG shaderCfg = {};
    shaderCfg.MaxAttributeSizeInBytes = specs.AttribSize;
    shaderCfg.MaxPayloadSizeInBytes = specs.PayloadSize;

    D3D12_GLOBAL_ROOT_SIGNATURE globalSig = {};
    globalSig.pGlobalRootSignature = specs.Signature->GetSignature();

    D3D12_RAYTRACING_PIPELINE_CONFIG pipelineCfg = {};
    pipelineCfg.MaxTraceRecursionDepth = specs.MaxRecursion;

    D3D12_STATE_SUBOBJECT subObjects[] = {
        { D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY, &lib },
        { D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP, &hitGroup },
        { D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG, &shaderCfg },
        { D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE, &globalSig },
        { D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG, &pipelineCfg }
    };

    D3D12_STATE_OBJECT_DESC desc = {};
    desc.Type = D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE;
    desc.NumSubobjects = 5;
    desc.pSubobjects = subObjects;

    HRESULT result = device->GetDevice()->CreateStateObject(&desc, IID_PPV_ARGS(&mPipeline));
    if (FAILED(result)) {    
        LOG_CRITICAL("Failed to create raytracing pipeline!");
    }

   mIDBuffer = MakeRef<Buffer>(device, heaps, 3 * D3D12_RAYTRACING_SHADER_TABLE_BYTE_ALIGNMENT, D3D12_RAYTRACING_SHADER_TABLE_BYTE_ALIGNMENT, BufferType::Constant, "ID Buffer");

   ID3D12StateObjectProperties* props;
   mPipeline->QueryInterface(&props);

   void *pData;
   auto writeId = [&](const wchar_t* name) {
       void* id = props->GetShaderIdentifier(name);
       memcpy(pData, id, D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
       pData = static_cast<char*>(pData) + D3D12_RAYTRACING_SHADER_TABLE_BYTE_ALIGNMENT;
   };

   mIDBuffer->Map(0, 0, &pData);
   writeId(L"RayGeneration");
   writeId(L"Miss");
   writeId(L"HitGroup");
   mIDBuffer->Unmap(0, 0);

   props->Release();
}

RTPipeline::~RTPipeline()
{
    mPipeline->Release();
}
