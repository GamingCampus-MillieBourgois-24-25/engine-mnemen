//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-06 14:16:34
//

#include "SceneSerializer.hpp"

#include <Core/File.hpp>
#include <Core/Logger.hpp>

#include <Utility/Math.hpp>

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

    if (entity.HasParent()) {
        entityJson["parent"] = static_cast<UInt32>(entity.GetParent().ID);
    } else {
        entityJson["parent"] = nullptr;
    }

    if (entity.HasComponent<TransformComponent>()) {
        glm::mat4 local = entity.GetWorldTransform();
        glm::vec3 p, r, s;
        Math::DecomposeTransform(local, p, r, s);
        glm::quat q = Math::EulerToQuat(r);

        entityJson["transform"] = {
            {"position", {p.x, p.y, p.z}},
            {"rotation", {q.x, q.y, q.z, q.w}},
            {"scale", {s.x, s.y, s.z}}
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
            { "volumePath", camera.Volume->Path }
        };
    }

    // Script Component
    ScriptComponent scripts = entity.GetComponent<ScriptComponent>();
    entityJson["scripts"] = nlohmann::json::array();
    for (auto& instance : scripts.Instances) {
        entityJson["scripts"].push_back(instance->Handle->Path);
    }

    // Audio Source component
    if (entity.HasComponent<AudioSourceComponent>()) {
        AudioSourceComponent source = entity.GetComponent<AudioSourceComponent>();
        entityJson["audioSource"] = {
            { "volume", source.Volume },
            { "looping", source.Looping },
            { "playOnAwake", source.PlayOnAwake },
            { "path", source.Handle ? source.Handle->Path : nullptr }
        };
    }

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
        transform.Update();
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
        if (!c["volumePath"].get<std::string>().empty())
            camera.Volume = AssetManager::Get(c["volumePath"], AssetType::PostFXVolume);
    }
    if (entityJson.contains("audioSource")) {
        auto& audio = entity.AddComponent<AudioSourceComponent>();
        auto a = entityJson["audioSource"];
        if (!a["path"].is_null()) {
            audio.Init(a["path"]);
        }
        audio.Looping = a["looping"];
        audio.PlayOnAwake = a["playOnAwake"];
        audio.Volume = a["volume"];
    }
    for (auto& script : entityJson["scripts"]) {
        auto& sc = entity.GetComponent<ScriptComponent>();
        sc.PushScript(script);
    }

    return entity;
}
