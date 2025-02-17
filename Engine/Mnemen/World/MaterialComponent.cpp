//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-17 10:55:13
//

#include "Entity.hpp"

#include <Core/File.hpp>

void MaterialComponent::LoadAlbedo(const String& string)
{
    if (!File::Exists(string)) {
        return;
    }

    if (Albedo) AssetManager::GiveBack(Albedo->Path);
    Albedo = AssetManager::Get(string, AssetType::Texture);
}

void MaterialComponent::LoadNormal(const String& string)
{
    if (!File::Exists(string)) {
        return;
    }

    if (Normal) AssetManager::GiveBack(Normal->Path);
    Normal = AssetManager::Get(string, AssetType::Texture);
}

void MaterialComponent::Free()
{
    if (Albedo) AssetManager::GiveBack(Albedo->Path);
    if (Normal) AssetManager::GiveBack(Normal->Path);
}
