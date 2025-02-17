//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-17 16:18:05
//

#pragma once

#include "Skybox.hpp"

/*      @NOTE(Amélie)

        Ideally what would be good is to generate these in async compute and just use a default skybox,
        however I have never done GPU multi-queue engines and I am NOT doing that shyte for a school project.
        Sorry!

        Also, I should really implement an actual system to queue commands after/before the frame because it's
        going to be kinda barbaric to queue skybox cooking operations when the editor/scene requests it... oh well...
        Too bad!

        Speaking of messes, GenerateSkybox handles the creation of the skybox textures, views, and generation. Biiig ass function.
        Maybe I should strip it down into smaller bits but I'll do for now :)

        Also, side note, the math for irradiance generation and all that 2nd order spherical harmonics crap is magic
        to me, so please refer to learnopengl.com for that purpose. Thanks!
*/

class SkyboxCooker
{
public:
    static void Init(RHI::Ref rhi);
    static void GenerateSkybox(Ref<Skybox> skybox);
private:
    static struct Data {
        RHI::Ref RHI;
        Sampler::Ref Sampler;
        ComputePipeline::Ref SkyboxCompute;
        ComputePipeline::Ref IrradianceCompute;
        ComputePipeline::Ref PrefilterCompute;
    } sData;
};
