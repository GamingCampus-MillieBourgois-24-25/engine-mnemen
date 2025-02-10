//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 00:02:48
//

#include "Components.hpp"

MeshComponent::~MeshComponent()
{
    AssetManager::GiveBack(MeshAsset->Path);
}

void MeshComponent::Init(const String& string)
{
    MeshAsset = AssetManager::Get(string, AssetType::Mesh);
}
