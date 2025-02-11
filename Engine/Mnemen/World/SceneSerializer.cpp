//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-06 14:16:34
//

#include "SceneSerializer.hpp"

#include <Core/File.hpp>
#include <Core/Logger.hpp>

void SceneSerializer::SerializeScene(Ref<Scene> scene, const String& path)
{
    auto& entities = scene->GetEntityArray();

    nlohmann::json root;
    root["entities"] = nlohmann::json::array();
    for (auto entity : entities) {
        if (entity->Private)
            continue;

        nlohmann::json entityRoot;
        entityRoot["name"] = entity->Name;
        
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
            entityRoot["cameraPrimary"] = (bool)camera.Primary;
            entityRoot["cameraFOV"] = camera.FOV;
            entityRoot["cameraNear"] = camera.Near;
            entityRoot["cameraFar"] = camera.Far;
            entityRoot["cameraAspectRatio"] = camera.AspectRatio;
        }

        // Script Component
        ScriptComponent scripts = entity->GetComponent<ScriptComponent>();
        entityRoot["scripts"] = nlohmann::json::array();
        for (auto& instance : scripts.Instances) {
            entityRoot["scripts"].push_back(instance->Path);
        }

        root["entities"].push_back(entityRoot);
    }

    File::WriteJSON(root, path);
    LOG_INFO("Saved scene at {0}", path);
}

Ref<Scene> SceneSerializer::DeserializeScene(const String& path)
{
    nlohmann::json root = File::LoadJSON(path);

    Ref<Scene> scene = MakeRef<Scene>();

    auto entityArray = root["entities"];
    for (auto& jsonEntity : entityArray) {
        Entity* entity = scene->AddEntity(jsonEntity["name"]);

        TransformComponent& component = entity->GetComponent<TransformComponent>();
        component.Position.x = jsonEntity["position"][0];
        component.Position.y = jsonEntity["position"][1];
        component.Position.z = jsonEntity["position"][2];
        component.Rotation.x = jsonEntity["rotation"][0];
        component.Rotation.y = jsonEntity["rotation"][1];
        component.Rotation.z = jsonEntity["rotation"][2];
        component.Scale.x = jsonEntity["scale"][0];
        component.Scale.y = jsonEntity["scale"][1];
        component.Scale.z = jsonEntity["scale"][2];
        component.Update();

        // Has mesh?
        bool hasMesh = jsonEntity["hasMesh"].get<bool>();
        if (hasMesh) {
            auto& mesh = entity->AddComponent<MeshComponent>();
            mesh.ParentEntity = entity;
            mesh.Init(jsonEntity["meshPath"]);
        }

        // Has camera?
        bool hasCamera = jsonEntity["hasCamera"].get<bool>();
        if (hasCamera) {
            auto& camera = entity->AddComponent<CameraComponent>();
            camera.Primary = jsonEntity["cameraPrimary"].get<bool>();
            camera.FOV = jsonEntity["cameraFOV"].get<float>();
            camera.Near = jsonEntity["cameraNear"].get<float>();
            camera.Far = jsonEntity["cameraFar"].get<float>();
            camera.AspectRatio = jsonEntity["cameraAspectRatio"].get<float>();
            camera.Update(component.Position, component.Rotation);
        }

        // Has script?
        auto& script = entity->GetComponent<ScriptComponent>();
        for (auto& scriptPath : jsonEntity["scripts"]) {
            script.PushScript(scriptPath);
        }
    }

    LOG_INFO("Loaded scene at {0}", path);
    return scene;
}
