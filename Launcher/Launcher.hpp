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
private:
    void LoadProjects();
    void CenteredImage(ImTextureID id, ImVec2 size);
    void CenteredText(const char* str);
    bool CenteredButton(const char* str);

    Texture::Ref mLogo;
    View::Ref mView;
    UnorderedMap<String, String> mLoadedProjects;
};
