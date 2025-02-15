//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 12:23:59
//

#pragma once

#include <Mnemen.hpp>

class Launcher : public Application
{
public:
    Launcher(ApplicationSpecs specs);
    ~Launcher();

    virtual void OnUpdate(float dt) override;
    virtual void OnPhysicsTick() override;
    virtual void OnImGui(const Frame& frame) override;
    virtual void PostPresent() override;

    String SelectedProject = "";
private:
    void Save();
    void CreateNewProject(String name, String folder);
    void LoadProjects();
    void CenteredImage(ImTextureID id, ImVec2 size);
    void CenteredText(const char* str);
    bool CenteredButton(const char* str);
    void LaunchProject(const String& path);

    Texture::Ref mLogo;
    View::Ref mView;
    UnorderedMap<String, String> mLoadedProjects;
    float mSelectedPhysicsHertz = 90.0f;
    CompressionFormat mFormat = CompressionFormat::BC3;
    char mBuffer[256] = {};
};
