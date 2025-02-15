//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 12:26:03
//

#include "Launcher.hpp"

#include <Utility/Dialog.hpp>

#include <imgui.h>

Launcher::Launcher(ApplicationSpecs specs)
    : Application(specs)
{
    auto entity = mScene->AddEntity();
    entity.AddComponent<CameraComponent>(true);
    
    ImGui::StyleColorsDark();

    Image image;
    image.Load("Assets/Icon/icon.png");

    TextureDesc desc;
    desc.Width = image.Width;
    desc.Height = image.Height;
    desc.Format = TextureFormat::RGBA8;
    desc.Depth = 1;
    desc.Levels = 1;
    desc.Name = "Icon";
    mLogo = mRHI->CreateTexture(desc);
    
    mView = mRHI->CreateView(mLogo, ViewType::ShaderResource);

    Uploader::EnqueueTextureUpload(image, mLogo);

    if (File::Exists(".cache/launcher.json")) {
        LoadProjects();
    }
    if (!File::Exists(".cache")) {
        File::CreateDirectoryFromPath(".cache");
        if (!File::Exists(".cache/launcher.json")) {
            File::CreateFileFromPath(".cache/launcher.json");
        }
    }
}

Launcher::~Launcher()
{
    nlohmann::json root;
    root["projects"] = nlohmann::json::array();

    for (auto& pair : mLoadedProjects) {
        root["projects"].push_back({
            { "name", pair.first },
            { "path", pair.second }
        });
    }
    File::WriteJSON(root, ".cache/launcher.json");
}

void Launcher::OnUpdate(float dt)
{
    
}

void Launcher::OnPhysicsTick()
{

}

void Launcher::OnImGui(const Frame& frame)
{
    ImGui::SetNextWindowSize(ImVec2(frame.Width, frame.Height));
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    ImGui::Begin("##", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);
    
    ImGui::BeginChild("left pane", ImVec2(300, 0), ImGuiChildFlags_AlwaysAutoResize);
    CenteredImage((ImTextureID)mView->GetDescriptor().GPU.ptr, ImVec2(256, 256));
    CenteredText("Mnemen Launcher");
    ImGui::Separator();
    CenteredText("Amélie Heinrich");
    CenteredText("Alexandre Mans");
    CenteredText("Coralie Thiery");
    CenteredText("Bastian N'Diaye");
    CenteredText("Jonathan de Vaulchier");
    CenteredText("Axel Moras");
    CenteredText("Timéo Delmon");
    ImGui::Separator();
    CenteredText("Version : 0.1.0");
    CenteredText("OS: Windows");
    CenteredText("Graphics: DirectX 12");
    CenteredText("Audio : MiniAudio");
    CenteredText("Physics : Jolt");
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("right pane", ImVec2(1280 - 300, 0), ImGuiChildFlags_AlwaysAutoResize);
    if (ImGui::Button("New", ImVec2(ImGui::GetContentRegionAvail().x, 100))) {

    }
    ImGui::Separator();
    for (auto& project : mLoadedProjects) {
        ImGui::Text(project.first.c_str());
        ImGui::TextColored(ImVec4(1, 1, 1, 0.6), project.second.c_str());
        ImGui::Separator();
    }
    ImGui::EndChild();
    
    ImGui::End();
}

void Launcher::LoadProjects()
{
    nlohmann::json root = File::LoadJSON(".cache/launcher.json");
    for (auto project : root["projects"]) {
        mLoadedProjects[project["name"]] = project["path"];
    }
}

void Launcher::CenteredImage(ImTextureID id, ImVec2 size)
{
    ImGuiStyle& style = ImGui::GetStyle();

    float fullSize = size.x + style.FramePadding.x * 2.0f;
    float avail = ImGui::GetContentRegionAvail().x;
    float off = (avail - fullSize) * 0.5f;
    if (off > 0.0f)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
    ImGui::Image(id, size);
}

void Launcher::CenteredText(const char* str)
{
    ImGuiStyle& style = ImGui::GetStyle();

    float size = ImGui::CalcTextSize(str).x + style.FramePadding.x * 2.0f;
    float avail = ImGui::GetContentRegionAvail().x;
    float off = (avail - size) * 0.5f;
    if (off > 0.0f)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
    ImGui::Text(str);
}

bool Launcher::CenteredButton(const char* str)
{
    ImGuiStyle& style = ImGui::GetStyle();

    float size = ImGui::CalcTextSize(str).x + style.FramePadding.x * 2.0f;
    float avail = ImGui::GetContentRegionAvail().x;
    float off = (avail - size) * 0.5f;
    if (off > 0.0f)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
    return ImGui::Button(str);
}
