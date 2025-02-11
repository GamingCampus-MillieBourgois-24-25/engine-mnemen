//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-06 14:16:34
//

#include "SceneSerializer.hpp"

#include <Core/File.hpp>
#include <Core/Logger.hpp>

void SceneSerializer::SerializeScene(Ref<Scene> scene, const String& path)
{
    auto registry = scene->GetRegistry();

    nlohmann::json root;
    root["entities"] = nlohmann::json::array();
    
    registry->view<entt::entity>().each([&](entt::entity id) {
        Entity entity(registry);
        entity.ID = id;

        if (entity.HasComponent<PrivateComponent>())
            return;

        auto entityJson = SerializeEntity(entity);
        root["entities"].push_back(entityJson);
    });

    File::WriteJSON(root, path);
    LOG_INFO("Saved scene at {0}", path);
}

Ref<Scene> SceneSerializer::DeserializeScene(const String& path)
{
    Ref<Scene> scene = MakeRef<Scene>();

    nlohmann::json root = File::LoadJSON(path);
    UnorderedMap<UInt32, Entity> entityMap;

    for (const auto& entityJson : root["entities"]) {
        DeserializeEntity(scene, entityJson, entityMap);
    }

    for (const auto& entityJson : root["entities"]) {
        Entity entity = entityMap[entityJson["id"].get<UInt32>()];
        if (!entityJson["parent"].is_null()) {
            Entity parent = entityMap[entityJson["parent"].get<UInt32>()];
            entity.SetParent(parent);
        }
    }

    LOG_INFO("Loaded scene at {0}", path);
    return scene;
}

nlohmann::json SceneSerializer::SerializeEntity(Entity entity)
{
    nlohmann::json entityJson;
    entityJson["id"] = static_cast<UInt32>(entity.ID);
    entityJson["name"] = entity.GetComponent<TagComponent>().Tag;

    if (entity.HasComponent<ParentComponent>()) {
        entityJson["parent"] = static_cast<UInt32>(entity.GetParent().ID);
    } else {
        entityJson["parent"] = nullptr;
    }

    if (entity.HasComponent<TransformComponent>()) {
        auto& transform = entity.GetComponent<TransformComponent>();
        entityJson["transform"] = {
            {"position", {transform.Position.x, transform.Position.y, transform.Position.z}},
            {"rotation", {transform.Rotation.x, transform.Rotation.y, transform.Rotation.z, transform.Rotation.w}},
            {"scale", {transform.Scale.x, transform.Scale.y, transform.Scale.z}}
        };
    }

    if (entity.HasComponent<MeshComponent>()) {
        auto& mesh = entity.GetComponent<MeshComponent>();
        entityJson["mesh"] = {
            { "path", mesh.MeshAsset->Path }
        };
    }
    
    // Camera Component
    if (entity.HasComponent<CameraComponent>()) {
        CameraComponent camera = entity.GetComponent<CameraComponent>();
        entityJson["camera"] = {
            { "primary", (bool)camera.Primary },
            { "fov", camera.FOV },
            { "near", camera.Near },
            { "far", camera.Far },
            { "aspectRatio", camera.AspectRatio }
        };
    }

    // Script Component
    ScriptComponent scripts = entity.GetComponent<ScriptComponent>();
    entityJson["scripts"] = nlohmann::json::array();
    for (auto& instance : scripts.Instances) {
        entityJson["scripts"].push_back(instance->Path);
    }

    nlohmann::json childrenJson = nlohmann::json::array();
    for (auto& child : entity.GetChildren()) {
        childrenJson.push_back(SerializeEntity(child));
    }
    entityJson["children"] = childrenJson;

    return entityJson;
}

Entity SceneSerializer::DeserializeEntity(Ref<Scene> scene, const nlohmann::json& entityJson, UnorderedMap<UInt32, Entity>& entityMap)
{
    Entity entity = scene->AddEntity(entityJson["name"]);
    entityMap[entityJson["id"].get<UInt32>()] = entity;

    if (entityJson.contains("transform")) {
        auto& transform = entity.GetComponent<TransformComponent>();
        auto t = entityJson["transform"];
        transform.Position = {t["position"][0], t["position"][1], t["position"][2]};
        transform.Rotation = glm::quat(t["rotation"][3], t["rotation"][0], t["rotation"][1], t["rotation"][2]);
        transform.Scale = {t["scale"][0], t["scale"][1], t["scale"][2]};
    }
    if (entityJson.contains("mesh")) {
        auto& mesh = entity.AddComponent<MeshComponent>();
        auto m = entityJson["mesh"];
        mesh.Init(m["path"]);
    }
    if (entityJson.contains("camera")) {
        auto& camera = entity.AddComponent<CameraComponent>();
        auto c = entityJson["camera"];
        camera.Primary = c["primary"];
        camera.FOV = c["fov"];
        camera.Near = c["near"];
        camera.Far = c["far"];
        camera.AspectRatio = c["aspectRatio"];
    }
    for (auto& script : entityJson["scripts"]) {
        auto& sc = entity.GetComponent<ScriptComponent>();
        sc.PushScript(script);
    }

    return entity;
}
