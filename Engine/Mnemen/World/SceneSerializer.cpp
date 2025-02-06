//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-06 14:16:34
//

#include "SceneSerializer.hpp"

#include <nlohmann/json.hpp>
#include <Core/File.hpp>

void SceneSerializer::SerializeScene(Ref<Scene> scene, const String& path)
{
    auto& entities = scene->GetEntityArray();

    nlohmann::json root;
    root["entities"] = nlohmann::json::array();
    for (auto entity : entities) {
        if (entity->Private)
            continue;

        nlohmann::json entityRoot;
        
        // Transform Component
        TransformComponent transform = entity->GetComponent<TransformComponent>();
        entityRoot["position"][0] = transform.Position.x;
        entityRoot["position"][1] = transform.Position.y;
        entityRoot["position"][2] = transform.Position.z;
        entityRoot["scale"][0] = transform.Scale.x;
        entityRoot["scale"][1] = transform.Scale.y;
        entityRoot["scale"][2] = transform.Scale.z;
        entityRoot["rotation"][0] = transform.Rotation.x;
        entityRoot["rotation"][1] = transform.Rotation.y;
        entityRoot["rotation"][2] = transform.Rotation.z;

        // Mesh Component
        entityRoot["hasMesh"] = entity->HasComponent<MeshComponent>();
        if (entity->HasComponent<MeshComponent>()) {
            entityRoot["meshPath"] = entity->GetComponent<MeshComponent>().MeshAsset->Path;
        }
        
        // Camera Component
        entityRoot["hasCamera"] = entity->HasComponent<CameraComponent>();
        if (entity->HasComponent<CameraComponent>()) {
            CameraComponent camera = entity->GetComponent<CameraComponent>();
            entityRoot["cameraPrimary"] = camera.Primary;
            entityRoot["cameraFOV"] = camera.FOV;
            entityRoot["cameraNear"] = camera.Near;
            entityRoot["cameraFar"] = camera.Far;
            entityRoot["cameraAspectRatio"] = camera.AspectRatio;
        }

        // Script Component
        entityRoot["hasScript"] = entity->HasComponent<ScriptComponent>();
        if (entity->HasComponent<ScriptComponent>()) {
            entityRoot["scriptPath"] = entity->GetComponent<ScriptComponent>().Path;
        }

        root["entities"].push_back(entityRoot);
    }

    File::WriteJSON(root, path);
}
