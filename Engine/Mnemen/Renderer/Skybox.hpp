//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-17 16:14:44
//

#pragma once

#include "Renderer.hpp"

struct Skybox
{
    String Path; ///< The path of the environment (for VRAM usage purposes, we discard the original .hdr image as soon as we finish generating the IBL textures)

    Texture::Ref EnvironmentMap; ///< Environment map, for skybox rendering or fallback for raytracing miss
    View::Ref EnvironmentMapSRV; ///< Shader resource view for the environment map

    Texture::Ref IrradianceMap; ///< Irradiance map generated with 2nd order spherical harmonics
    View::Ref IrradianceMapSRV; ///< Shader resource view for the irradiance map

    Texture::Ref PrefilterMap; ///< Prefilter map for the specular term
    View::Ref PrefilterMapSRV; ///< Shader resource view for the prefilter map
};
