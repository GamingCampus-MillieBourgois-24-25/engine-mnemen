//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 00:02:48
//

#include "Entity.hpp"

void MeshComponent::Free()
{
    if (MeshAsset && Loaded) {
        AssetManager::GiveBack(MeshAsset->Path);
        Loaded = false;
    }
}

void MeshComponent::Init(const String& string)
{
    if (Loaded) {
        AssetManager::GiveBack(MeshAsset->Path);
    }
    MeshAsset.reset();
    MeshAsset = AssetManager::Get(string, AssetType::Mesh);
    Loaded = true;
}
