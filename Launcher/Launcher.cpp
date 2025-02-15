//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 12:26:03
//

#include "Launcher.hpp"

#include <Utility/Dialog.hpp>
#include <windows.h>
#include <sstream>
#include <functional>
#include <imgui.h>

namespace fs = std::filesystem;

bool CopyFolderRecursively(const fs::path& sourceDir, const fs::path& destDir)
{
    try {
        // Check if the source directory exists
        if (!fs::exists(sourceDir) || !fs::is_directory(sourceDir)) {
            std::cerr << "Source directory does not exist or is not a directory: " << sourceDir << std::endl;
            return false;
        }

        // Create the destination directory if it does not exist
        if (!fs::exists(destDir)) {
            fs::create_directories(destDir);
        }

        // Iterate over the contents of the source directory
        for (const auto& entry : fs::directory_iterator(sourceDir)) {
            const fs::path& sourcePath = entry.path();
            fs::path destPath = destDir / sourcePath.filename();

            if (fs::is_directory(sourcePath)) {
                // Recursively copy subdirectories
                if (!CopyFolderRecursively(sourcePath, destPath)) {
                    return false;
                }
            } else if (fs::is_regular_file(sourcePath)) {
                // Copy files
                fs::copy(sourcePath, destPath, fs::copy_options::overwrite_existing);
            }
        }

        return true;
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        return false;
    }
}

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

void Launcher::Save()
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

Launcher::~Launcher()
{
    Save();
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

    const char* items[] = {
        "BC3 (Faster, Lower Quality)",
        "BC7 (Slower, Higher Quality)"
    };

    ImGui::BeginChild("right pane", ImVec2(1280 - 300, 0), ImGuiChildFlags_AlwaysAutoResize);
    ImGui::InputText("Name", mBuffer, 256);
    if (ImGui::Button("Create...", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
        String path = Dialog::OpenFolder();
        if (!path.empty() && strlen(mBuffer) > 0) {
            CreateNewProject(mBuffer, path);
        }
    }
    if (ImGui::TreeNodeEx("Creation Settings", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::SliderFloat("Physics Update (Hz)", &mSelectedPhysicsHertz, 30.0f, 120.0f, "%.0f");
        ImGui::Combo("Compression Format", (int*)&mFormat, items, 2);
        ImGui::TreePop();
    }
    ImGui::Separator();
    for (auto& project : mLoadedProjects) {
        ImGui::Text(project.first.c_str());
        ImGui::SameLine();
        ImGui::PushID(project.first.c_str());
        if (ImGui::Button("Open")) {
            LaunchProject(project.second);
        }
        ImGui::PopID();
        ImGui::TextColored(ImVec4(1, 1, 1, 0.6), project.second.c_str());
        ImGui::Separator();
    }
    ImGui::EndChild();
    
    ImGui::End();
}

void Launcher::PostPresent()
{
    if (!SelectedProject.empty()) {
        mWindow->Close();
    }
}

void Launcher::CreateNewProject(String name, String folder)
{
    for (int i = 0; i < name.size(); i++) {
        if (name[i] == ' ') name[i] = '_';
    }

    std::stringstream ss;
    ss << folder << "/" << name << ".mpj";

    std::stringstream ssAssets;
    ssAssets << folder << "/" << "Assets/";
    if (!File::Exists(ssAssets.str())) {
        File::CreateDirectoryFromPath(ssAssets.str());
    }

    Ref<Project> project = MakeRef<Project>();
    project->PathAbsolute = ss.str();
    project->Name = name;
    project->StartScenePathRelative = "Assets/Scenes/Default.msf";
    project->SceneMap["Start"] = project->StartScenePathRelative;
    project->StartSceneName = "Start";
    project->Settings.Format = mFormat;
    project->Settings.PhysicsRefreshRate = mSelectedPhysicsHertz;
    project->Save(project->PathAbsolute);

    // Copy all that shiii
    fs::copy("imgui.ini", folder + "/imgui.ini");
    CopyFolderRecursively("Assets", ssAssets.str());

    mLoadedProjects[project->Name] = project->PathAbsolute;
    Save();
}

void Launcher::LoadProjects()
{
    nlohmann::json root = File::LoadJSON(".cache/launcher.json");
    for (auto project : root["projects"]) {
        mLoadedProjects[project["name"]] = project["path"];
    }
}

void Launcher::LaunchProject(const String& path)
{
    SelectedProject = path;
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
