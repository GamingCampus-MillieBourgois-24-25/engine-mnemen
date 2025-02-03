//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-03 18:35:26
//

#pragma once

#include <Core/Common.hpp>
#include <Agility/d3d12shader.h>

enum class ShaderType
{
    None,
    Vertex,
    Fragment,
    Compute,
    Hull,
    Domain,
    Mesh,
    Amplification,
    Library
};

struct Shader
{
    bool Valid;
    ShaderType Type;
    Vector<UInt8> Bytecode;
};

class ShaderCompiler
{
public:
    static Shader Compile(const String& path, const String& entry, ShaderType type);
    static ID3D12ShaderReflection* Reflect(Shader shader);
};
