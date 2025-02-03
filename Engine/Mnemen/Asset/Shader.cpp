//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-03 18:38:56
//

#include <Asset/Shader.hpp>

#include <Core/File.hpp>
#include <Core/Logger.hpp>
#include <Core/Assert.hpp>
#include <RHI/Utilities.hpp>

#include <DXC/dxcapi.h>
#include <wrl/client.h>

const char* GetProfileFromType(ShaderType type)
{
    switch (type) {
        case ShaderType::Vertex: {
            return "vs_6_7";
        }
        case ShaderType::Fragment: {
            return "ps_6_7";
        }
        case ShaderType::Compute: {
            return "cs_6_7";
        }
        case ShaderType::Hull: {
            return "hs_6_7";
        }
        case ShaderType::Domain: {
            return "ds_6_7";
        }
        case ShaderType::Mesh: {
            return "ms_6_7";
        }
        case ShaderType::Amplification: {
            return "as_6_7";
        }
        case ShaderType::Library: {
            return "lib_6_7";
        }
    }
    return "???";
}

Shader ShaderCompiler::Compile(const String& path, const String& entry, ShaderType type)
{
    Shader result = {};

    String wrappedSource = File::ReadFile(path); 
    const char* source = wrappedSource.c_str();

    wchar_t wideTarget[512];
    swprintf_s(wideTarget, 512, L"%hs", GetProfileFromType(type));
    
    wchar_t wideEntry[512];
    swprintf_s(wideEntry, 512, L"%hs", entry.c_str());

    IDxcUtils* pUtils = nullptr;
    IDxcCompiler* pCompiler = nullptr;
    ASSERT(SUCCEEDED(DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&pUtils))), "Failed to create DXC utils!");
    ASSERT(SUCCEEDED(DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&pCompiler))), "Failed too create DXC compiler!");

    IDxcIncludeHandler* pIncludeHandler = nullptr;
    ASSERT(SUCCEEDED(pUtils->CreateDefaultIncludeHandler(&pIncludeHandler)), "Failed to create default include handler!");

    IDxcBlobEncoding* pSourceBlob = nullptr;
    ASSERT(SUCCEEDED(pUtils->CreateBlob(source, wrappedSource.size(), 0, &pSourceBlob)), "Failed to create source blob!");

    LPCWSTR pArgs[] = {
        L"-Zi",
        L"-Fd",
        L"-Fre",
        L"-Qembed_debug",
        L"-Wno-payload-access-perf",
        L"-Wno-payload-access-shader"
    };

    IDxcOperationResult* pResult = nullptr;
    ASSERT(SUCCEEDED(pCompiler->Compile(pSourceBlob, L"Shader", wideEntry, wideTarget, pArgs, ARRAYSIZE(pArgs), nullptr, 0, pIncludeHandler, &pResult)), "Failed to create result blob!");

    IDxcBlobEncoding* pErrors = nullptr;
    pResult->GetErrorBuffer(&pErrors);

    if (pErrors && pErrors->GetBufferSize() != 0) {
        IDxcBlobUtf8* pErrorsU8 = nullptr;
        pErrors->QueryInterface(IID_PPV_ARGS(&pErrorsU8));
        LOG_ERROR("[DXC] Shader errors: {0}", (char*)pErrorsU8->GetStringPointer());
        pErrorsU8->Release();
        pErrors->Release();
        return { false };
    }

    HRESULT Status;
    pResult->GetStatus(&Status);

    IDxcBlob* pShaderBlob = nullptr;
    pResult->GetResult(&pShaderBlob);

    result.Type = type;
    result.Bytecode.resize(pShaderBlob->GetBufferSize());
    memcpy(result.Bytecode.data(), pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize());
    LOG_DEBUG("Compiled shader {0}", path.c_str());

    D3DUtils::Release(pShaderBlob);
    D3DUtils::Release(pErrors);
    D3DUtils::Release(pResult);
    D3DUtils::Release(pSourceBlob);
    D3DUtils::Release(pIncludeHandler);
    D3DUtils::Release(pCompiler);
    D3DUtils::Release(pUtils);
    return result;
}

ID3D12ShaderReflection* ShaderCompiler::Reflect(Shader shader)
{
    ID3D12ShaderReflection* pReflection = nullptr;
    
    IDxcUtils* pUtils = nullptr;
    DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&pUtils));
    
    DxcBuffer ShaderBuffer = {};
    ShaderBuffer.Ptr = shader.Bytecode.data();
    ShaderBuffer.Size = shader.Bytecode.size();
    
    ASSERT(SUCCEEDED(pUtils->CreateReflection(&ShaderBuffer, IID_PPV_ARGS(&pReflection))), "Failed to get shader reflection!");
    pUtils->Release();
    return pReflection;
}
