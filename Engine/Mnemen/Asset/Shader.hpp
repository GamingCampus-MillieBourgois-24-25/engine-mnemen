//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-03 18:35:26
//

#pragma once

#include <Core/Common.hpp>
#include <Agility/d3d12shader.h>

/// @enum ShaderType
/// @brief Represents different types of shaders.
///
/// This enumeration defines various shader types used in rendering.
enum class ShaderType
{
    None,           ///< No shader type specified.
    Vertex,         ///< Vertex shader.
    Fragment,       ///< Fragment (pixel) shader.
    Compute,        ///< Compute shader.
    Hull,           ///< Hull (tessellation control) shader.
    Domain,         ///< Domain (tessellation evaluation) shader.
    Mesh,           ///< Mesh shader.
    Amplification,  ///< Amplification shader.
    Library         ///< Shader library.
};

/// @struct Shader
/// @brief Represents a compiled shader.
///
/// Contains shader validity status, type, and compiled bytecode.
struct Shader
{
    bool Valid; ///< Indicates whether the shader compiled successfully.
    ShaderType Type; ///< Type of the shader.
    Vector<UInt8> Bytecode; ///< Compiled shader bytecode.
};

/// @class ShaderCompiler
/// @brief Handles shader compilation and reflection.
///
/// Provides functionality to compile shaders from source files and retrieve reflection data.
class ShaderCompiler
{
public:
    /// @brief Compiles a shader from a file.
    /// @param path The file path to the shader source.
    /// @param entry The entry point function name.
    /// @param type The type of shader being compiled.
    /// @return A compiled Shader object.
    static Shader Compile(const String& path, const String& entry, ShaderType type);

    /// @brief Retrieves reflection data for a compiled shader.
    /// @param shader The compiled shader.
    /// @return A pointer to the Direct3D 12 shader reflection interface.
    static ID3D12ShaderReflection* Reflect(Shader shader);
};

