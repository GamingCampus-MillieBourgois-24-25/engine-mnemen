//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-15 16:16:52
//

#include "PostProcessVolume.hpp"

#include <Core/File.hpp>

void PostProcessVolume::Load(const String& path)
{
    Path = path;

    nlohmann::json root = File::LoadJSON(path);

    if (root.contains("geometry")) {
        nlohmann::json g = root["geometry"];
        VisualizeMeshlets = g["visualizeMeshlets"].get<bool>();
    }

    if (root.contains("colorGrading")) {
        nlohmann::json cg = root["colorGrading"];

        EnableColorGrading = cg["enable"].get<bool>();
        Brightness = cg["brightness"].get<float>();
        Exposure = cg["exposure"].get<float>();
        Saturation = cg["saturation"].get<float>();
        Contrast = cg["contrast"].get<float>();
        HueShift = cg["hueShift"].get<float>();
        Balance = cg["balance"].get<float>();
        Temperature = cg["temperature"].get<float>();
        Tint = cg["tint"].get<float>();
        
        Shadows = glm::vec4(
            cg["shadows"][0].get<float>(),
            cg["shadows"][1].get<float>(),
            cg["shadows"][2].get<float>(),
            cg["shadows"][3].get<float>()
        );
        Highlights = glm::vec4(
            cg["shadows"][0].get<float>(),
            cg["shadows"][1].get<float>(),
            cg["shadows"][2].get<float>(),
            cg["shadows"][3].get<float>()
        );
        ColorFilter = glm::vec4(
            cg["shadows"][0].get<float>(),
            cg["shadows"][1].get<float>(),
            cg["shadows"][2].get<float>(),
            cg["shadows"][3].get<float>()
        );
    }

    if (root.contains("depthOfField")) {
        nlohmann::json dof = root["depthOfField"];

        EnableDOF = dof["enable"].get<bool>();
        FocusRange = dof["focusRange"].get<float>();
        FocusPoint = dof["focusPoint"].get<float>();
    }

    if (root.contains("gammaCorrect")) {
        nlohmann::json gc = root["gammaCorrect"];

        GammaCorrection = gc["factor"].get<float>();
    }
}

void PostProcessVolume::Save(const String& path)
{
    Path = path;

    nlohmann::json root;

    root["geometry"]["visualizeMeshlets"] = VisualizeMeshlets;

    root["colorGrading"] = {
        {"enable", EnableColorGrading},
        {"brightness", Brightness},
        {"exposure", Exposure},
        {"saturation", Saturation},
        {"contrast", Contrast},
        {"hueShift", HueShift},
        {"balance", Balance},
        {"temperature", Temperature},
        {"tint", Tint},
        {"shadows", {Shadows.x, Shadows.y, Shadows.z, Shadows.w}},
        {"highlights", {Highlights.x, Highlights.y, Highlights.z, Highlights.w}},
        {"colorFilter", {ColorFilter.x, ColorFilter.y, ColorFilter.z, ColorFilter.w}}
    };

    root["depthOfField"] = {
        {"enable", EnableDOF},
        {"focusRange", FocusRange},
        {"focusPoint", FocusPoint}
    };

    root["gammaCorrect"] = {
        {"factor", GammaCorrection}
    };

    File::WriteJSON(root, path);
}

