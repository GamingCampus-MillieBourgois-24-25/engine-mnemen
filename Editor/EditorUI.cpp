//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-11 19:04:17
//

#include "Editor.hpp"

#include <FontAwesome/FontAwesome.hpp>
#include <RHI/Uploader.hpp>

#include <imgui.h>
#include <imgui_internal.h>

#include <Utility/Dialog.hpp>
#include <Utility/String.hpp>

void Editor::ProjectEditor()
{
    ImGui::Begin(ICON_FA_GITHUB " Project Settings");

    ImGui::End();
}

void Editor::Viewport(const Frame& frame)
{
    if (!mScene)
        return;

    ImGui::Begin(ICON_FA_GAMEPAD " Viewport");
    auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
	auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
	auto viewportOffset = ImGui::GetWindowPos();
    mViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
	mViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

    // Play/Stop
    ImGui::SetCursorPosX(10.0f);
    if (ImGui::Button(ICON_FA_PLAY)) {
        SaveScene();
        if (!mCurrentScenePath.empty()) {
            OnAwake();
        }
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_STOP)) {
        if (mScenePlaying) {
            OnStop();
            mMarkForStop = true;
        }
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Translate", mOperation == ImGuizmo::OPERATION::TRANSLATE)) mOperation = ImGuizmo::OPERATION::TRANSLATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Rotate", mOperation == ImGuizmo::OPERATION::ROTATE)) mOperation = ImGuizmo::OPERATION::ROTATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Scale", mOperation == ImGuizmo::OPERATION::SCALE)) mOperation = ImGuizmo::OPERATION::SCALE;

    auto ldr = RendererTools::Get("LDRColorBuffer");
    mViewportFocused = ImGui::IsWindowFocused();

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
    
    frame.CommandBuffer->ClearRenderTarget(frame.BackbufferView, 0.0f, 0.0f, 0.0f);
    frame.CommandBuffer->Barrier(ldr->Texture, ResourceLayout::Shader);
    ImGui::Image((ImTextureID)ldr->GetView(ViewType::ShaderResource)->GetDescriptor().GPU.ptr, mViewportSize);

    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
            if (payload) {
                const wchar_t* path = (const wchar_t*)payload->Data;
                std::filesystem::path scenePath(path);
                std::string sceneString = scenePath.string();
                if (sceneString.find(".msf") != std::string::npos) {
                    for (int i = 0; i < sceneString.size(); i++) {
                        sceneString[i] = sceneString[i] == '\\' ? '/' : sceneString[i];
                    }
                    OpenScene(sceneString);
                }
            }
        }
        ImGui::EndDragDropTarget();
    }

    // Gizmos
    if (mSelectedEntity && !mScenePlaying) {
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(mViewportBounds[0].x, mViewportBounds[0].y,
            mViewportBounds[1].x - mViewportBounds[0].x,
            mViewportBounds[1].y - mViewportBounds[0].y);

        glm::mat4 view = mCamera.View();
        glm::mat4 projection = mCamera.Projection();

        auto& transform = mSelectedEntity.GetComponent<TransformComponent>();

        // Get the entity's world transform
        glm::mat4 worldMatrix = mSelectedEntity.GetWorldTransform();

        // Let ImGuizmo manipulate the world matrix
        ImGuizmo::Manipulate(glm::value_ptr(view),
            glm::value_ptr(projection),
            mOperation,
            ImGuizmo::MODE::LOCAL,
            glm::value_ptr(worldMatrix));

        if (ImGuizmo::IsUsingAny()) {
            // Disable camera movement while using gizmo
            mGizmoFocused = true;

            // Decompose the new world matrix into position, rotation, and scale
            glm::vec3 newPosition = glm::vec3(0.0f), newRotationEuler = glm::vec3(0.0f), newScale = glm::vec3(1.0f);
            ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(worldMatrix),
                glm::value_ptr(newPosition),
                glm::value_ptr(newRotationEuler),
                glm::value_ptr(newScale));

            // Convert Euler angles to quaternion for rotation
            glm::quat newRotationQuat = Math::EulerToQuat(newRotationEuler);

            if (mSelectedEntity.HasParent()) {
                // If entity has a parent, compute new local transform
                Entity parent = mSelectedEntity.GetParent();
                glm::mat4 parentWorld = parent.GetWorldTransform();

                // Calculate new local transform: Local = Inverse(ParentWorld) * NewWorld
                glm::mat4 newLocalTransform = glm::inverse(parentWorld) * worldMatrix;

                // Update position and scale as before
                ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(newLocalTransform),
                    glm::value_ptr(newPosition),
                    glm::value_ptr(newRotationEuler),
                    glm::value_ptr(newScale));
                newRotationQuat = Math::EulerToQuat(newRotationEuler);

                transform.Position = newPosition;
                transform.Rotation = newRotationQuat;
                transform.Scale = newScale;
                transform.Update();
            }
            else {
                // No parent, world matrix is directly applied
                transform.Position = newPosition;
                transform.Rotation = newRotationQuat;
                transform.Scale = newScale;
                transform.Update();
            }
        }
        else {
            mGizmoFocused = false;
        }
    }

    // Debug draw some stuff
    if (mSelectedEntity && !mScenePlaying) {
        if (mSelectedEntity.HasComponent<CameraComponent>()) {
            auto cam = mSelectedEntity.GetComponent<CameraComponent>();
            Debug::DrawFrustum(cam.View, cam.Projection, glm::vec3(1.0f));
        }
    }

    ImGui::End();
}

void Editor::BeginDockSpace()
{
    if (!mScene)
        return;

    static bool dockspaceOpen = true;
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen) {
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
	ImGui::PopStyleVar();
    if (opt_fullscreen)
		ImGui::PopStyleVar(2);
    ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	float minWinSizeX = style.WindowMinSize.x;
	style.WindowMinSize.x = 370.0f;
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}
	style.WindowMinSize.x = minWinSizeX;

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu(ICON_FA_FILE " File")) {
            if (ImGui::BeginMenu(ICON_FA_FILE_ARCHIVE_O " Save")) {
                if (ImGui::MenuItem(ICON_FA_CHECK " Save Current", "Ctrl+S")) {
                    SaveScene();
                }
                if (ImGui::MenuItem(ICON_FA_QUESTION " Save As...", "Ctrl+S")) {
                    SaveSceneAs();
                }
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem(ICON_FA_FOLDER_OPEN " Open", "Ctrl+O")) {
                String path = Dialog::Open({ ".msf" });
                if (!path.empty()) {
                    OpenScene(path);
                }
            }
            if (ImGui::MenuItem(ICON_FA_CHECK " New", "Ctrl+N")) {
                mMarkForClose = true;
            }
            if (ImGui::MenuItem(ICON_FA_PENCIL " Close")) {
                mMarkForClose = true;
            }
            if (ImGui::MenuItem(ICON_FA_WINDOW_CLOSE " Exit", "Ctrl+Q")) {
                SaveScene();
                mWindow->Close();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu(ICON_FA_TASKS " Window")) {
            if (ImGui::BeginMenu("Theme")) {
                if (ImGui::MenuItem(ICON_FA_GITHUB " Mnemen")) SetColors();
                if (ImGui::MenuItem(ICON_FA_MOON_O " ImGui Dark")) ImGui::StyleColorsDark();
                if (ImGui::MenuItem(ICON_FA_SUN_O " ImGui Light")) ImGui::StyleColorsLight();
                if (ImGui::MenuItem(ICON_FA_SMILE_O " ImGui Classic")) ImGui::StyleColorsClassic();
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

void Editor::HierarchyPanel()
{
    if (!mScene)
        return;

    ImGui::Begin(ICON_FA_GLOBE " Scene Hierarchy");
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.5f));

    // Add entity button
    if (ImGui::Button(ICON_FA_PLUS " Add Entity", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
        mSelectedEntity = mScene->AddEntity("New Entity");
    }
    ImGui::Button("Drag me here to detach!", ImVec2(ImGui::GetContentRegionAvail().x, 0));
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_MOVE")) {
            if (payload) {
                entt::entity droppedID = *(const entt::entity*)payload->Data;
                Entity droppedEntity(mScene->GetRegistry());
                droppedEntity.ID = droppedID;
                droppedEntity.RemoveParent();
            }
        }
        ImGui::EndDragDropTarget();
    }
    ImGui::PopStyleVar();
    ImGui::Separator();

    // Draw root entities only
    ImGui::BeginChild("EntityNodes");

    // Fetch all entities first
    auto view = mScene->GetRegistry()->view<TagComponent>();
    std::vector<Entity> entities;

    // Collect entities
    for (auto id : view) {
        Entity entity(mScene->GetRegistry());
        entity.ID = id;

        if (entity.HasComponent<PrivateComponent>())
            continue;
        if (entity.ID == entt::null)
            continue;

        if (!entity.HasParent()) {
            entities.push_back(entity);
        }
    }

    // Sort entities alphabetically based on TagComponent
    std::sort(entities.begin(), entities.end(), [](Entity& a, Entity& b) {
        return a.GetComponent<TagComponent>().Tag < b.GetComponent<TagComponent>().Tag;
    });

    // Draw sorted entities
    for (const auto& entity : entities) {
        DrawEntityNode(entity);
    }

    ImGui::EndChild();
    ImGui::End();
}

void Editor::DrawEntityNode(Entity entity)
{
    if (!mScene)
        return;

    auto& tag = entity.GetComponent<TagComponent>();

    // Prepare label with icon and name
    char temp[256];
    sprintf(temp, "%s %s", ICON_FA_CUBE, tag.Tag.c_str());

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

    // If no children, make it a leaf
    if (entity.GetChildren().empty())
        flags |= ImGuiTreeNodeFlags_Leaf;

    // Highlight the selected entity
    if (mSelectedEntity.ID == entity.ID)
        flags |= ImGuiTreeNodeFlags_Selected;

    bool nodeOpen = ImGui::TreeNodeEx((void*)(UInt64)entity.ID, flags, temp);

    if (ImGui::IsItemClicked()) {
        mSelectedEntity = {};
        mSelectedEntity = entity;
    }

    // **Right-click menu for detaching entity**
    if (ImGui::BeginPopupContextItem()) {
        if (ImGui::MenuItem(ICON_FA_TRASH " Delete")) {
            mMarkForDeletion = true;
        }
        if (entity.HasParent()) {
            if (ImGui::MenuItem(ICON_FA_CHILD " Detach from Parent")) {
                entity.RemoveParent();
            }
        }
        ImGui::EndPopup();
    }

    // Drag & Drop Source (Start Dragging)
    if (ImGui::BeginDragDropSource()) {
        entt::entity entityID = entity.ID;
        ImGui::SetDragDropPayload("ENTITY_MOVE", &entityID, sizeof(entt::entity));
        ImGui::Text("Move %s", tag.Tag.c_str());
        ImGui::EndDragDropSource();
    }

    // Drag & Drop Target (Accept Child Entity)
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_MOVE")) {
            entt::entity droppedID = *(const entt::entity*)payload->Data;

            Entity droppedEntity(mScene->GetRegistry());
            droppedEntity.ID = droppedID;

            if (droppedEntity.ID != entity.ID) {
                droppedEntity.SetParent(entity);
            }
        }
        ImGui::EndDragDropTarget();
    }

    // Recursively draw children
    if (nodeOpen) {
        for (Entity child : entity.GetChildren()) {
            DrawEntityNode(child);
        }
        ImGui::TreePop();
    }
}

void Editor::AssetPanel()
{
    if (!mScene)
        return;

    ImGui::Begin(ICON_FA_ARCHIVE " Assets");
    if (ImGui::BeginTabBar("Basic")) {
        if (ImGui::BeginTabItem(ICON_FA_FOLDER_O " Asset Directory")) {
            AssetBrowser();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem(ICON_FA_FILE_IMAGE_O " Loaded Assets")) {
            if (ImGui::Button("Run Asset Cache")) {
                AssetCacher::Init("Assets");
            }

            const char* tags[] = {
                ICON_FA_QUESTION " Unknown",
                ICON_FA_CUBE " Models",
                ICON_FA_FILE_IMAGE_O " Textures",
                ICON_FA_PAINT_BRUSH " Shaders",
                ICON_FA_SUN_O " Environment Maps",
                ICON_FA_CODE " Scripts",
                ICON_FA_MUSIC " Audio Files",
                ICON_FA_CAMERA_RETRO " Post Process Volumes"
            };
            for (int i = 1; i < (int)AssetType::MAX; i++) {
                ImGui::PushStyleColor(ImGuiCol_Header, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.6f));
                ImGui::PushStyleColor(ImGuiCol_HeaderHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.7f));
                ImGui::PushStyleColor(ImGuiCol_HeaderActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.8f, 0.8f));

                if (ImGui::TreeNodeEx(tags[i], ImGuiTreeNodeFlags_Framed)) {
                    for (auto& asset : AssetManager::sData.mAssets) {
                        if (asset.second->Type != (AssetType)i)
                            continue;
                        static const char* enumToIcon[] = {
                            ICON_FA_QUESTION,
                            ICON_FA_CUBE,
                            ICON_FA_FILE_IMAGE_O,
                            ICON_FA_PAINT_BRUSH,
                            ICON_FA_SUN_O,
                            ICON_FA_CODE,
                            ICON_FA_MUSIC,
                            ICON_FA_CAMERA_RETRO
                        };

                        char temp[256];
                        sprintf(temp, "%s %s", enumToIcon[(int)asset.second->Type], asset.first.c_str());
                        if (ImGui::TreeNode(temp)) {
                            ImGui::Text("Ref Count : %d", asset.second->RefCount);
                            ImGui::TreePop();
                        }
                    }
                    ImGui::TreePop();
                }
                ImGui::PopStyleColor(3);
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::End();
}

void Editor::EntityEditor()
{
    if (!mScene)
        return;

    ImGui::Begin(ICON_FA_WRENCH " Entity Editor");
    if (mSelectedEntity) {
        auto& tag = mSelectedEntity.GetComponent<TagComponent>();

        if (mEntityNameBuffer.find(mSelectedEntity.ID) == mEntityNameBuffer.end()) {
            mEntityNameBuffer[mSelectedEntity.ID] = tag.Tag;
        }

        std::string& inputBuffer = mEntityNameBuffer[mSelectedEntity.ID];

        // ImGui Input Text
        char tempBuffer[256];
        strncpy(tempBuffer, inputBuffer.c_str(), sizeof(tempBuffer));
        tempBuffer[sizeof(tempBuffer) - 1] = '\0';

        if (ImGui::InputText("##", tempBuffer, sizeof(tempBuffer))) {
            inputBuffer = tempBuffer;
        }

        // Update entity tag on deselection
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            tag.Tag = inputBuffer;
        }

        // Transform
        if (ImGui::TreeNodeEx(ICON_FA_HOME " Transform", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen)) {
            TransformComponent& transform = mSelectedEntity.GetComponent<TransformComponent>();
            DrawVec3Control("Position", transform.Position, 0.0f);
            DrawVec3Control("Scale", transform.Scale, 1.0f);
            
            glm::vec3 euler = Math::QuatToEuler(transform.Rotation);
            DrawVec3Control("Rotation", euler, 0.0f);
            transform.Rotation = Math::EulerToQuat(euler);
            transform.Update();

            ImGui::TreePop();
        }
        
        // CAMERA
        if (mSelectedEntity.HasComponent<CameraComponent>()) {
            if (ImGui::TreeNodeEx(ICON_FA_CAMERA " Camera Component", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen)) {
                CameraComponent& camera = mSelectedEntity.GetComponent<CameraComponent>();

                bool shouldDelete = false;
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7.0f, 0.7f, 0.7f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7.0f, 0.8f, 0.8f));
                ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.5f));
                if (ImGui::Button(ICON_FA_TRASH " Delete", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
                    shouldDelete = true;
                }
                ImGui::PopStyleColor(3);

                if (camera.Volume) {
                    char temp[512];
                    sprintf(temp, "%s %s", ICON_FA_FILE, camera.Volume->Path.c_str());
                    if (ImGui::Button(temp, ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
                        String path = Dialog::Open({ ".mfx" });
                        if (!path.empty()) {
                            camera.Load(path);
                        }
                    }
                } else {
                    if (ImGui::Button(ICON_FA_FILE " Open...", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
                        String path = Dialog::Open({ ".mfx" });
                        if (!path.empty()) {
                            camera.Load(path);
                        }
                    }
                }
                ImGui::PopStyleVar();
                if (ImGui::BeginDragDropTarget()) {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
                        const wchar_t* path = (const wchar_t*)payload->Data;
                        std::filesystem::path fxPath(path);
                        std::string fxString = fxPath.string();
                        if (fxString.find(".mfx") != std::string::npos) {
                            for (int i = 0; i < fxString.size(); i++) {
                                fxString[i] = fxString[i] == '\\' ? '/' : fxString[i];
                            }
                            camera.Load(fxString);
                        }
                    }
                    ImGui::EndDragDropTarget();
                }
                ImGui::Separator();

                ImGui::Checkbox("Primary", (bool*)&camera.Primary);
                ImGui::SliderFloat("FOV", &camera.FOV, 0.0f, 360.0f);
                ImGui::SliderFloat("Near", &camera.Near, 0.1f, camera.Far);
                ImGui::SliderFloat("Far", &camera.Far, camera.Near, 1000.0f);
                ImGui::TreePop();

                if (shouldDelete) {
                    mSelectedEntity.RemoveComponent<CameraComponent>();
                }
            }
        }

        // MESH
        if (mSelectedEntity.HasComponent<MeshComponent>()) {
            if (ImGui::TreeNodeEx(ICON_FA_CUBE " Mesh Component", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen)) {
                auto& mesh = mSelectedEntity.GetComponent<MeshComponent>();

                bool shouldDelete = false;
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7.0f, 0.7f, 0.7f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7.0f, 0.8f, 0.8f));
                ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.5f));
                if (ImGui::Button(ICON_FA_TRASH " Delete", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
                    shouldDelete = true;
                }
                ImGui::PopStyleColor(3);
                
                if (mesh.MeshAsset) {
                    char temp[512];
                    sprintf(temp, "%s %s", ICON_FA_FILE, mesh.MeshAsset->Path.c_str());
                    if (ImGui::Button(temp, ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
                        String path = Dialog::Open({ ".gltf", ".obj", ".fbx" });
                        if (!path.empty()) {
                            mModelChange = path;
                        }
                    }
                } else {
                    if (ImGui::Button(ICON_FA_FILE " Open...", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
                        String path = Dialog::Open({ ".gltf", ".obj", ".fbx" });
                        if (!path.empty()) {
                            mModelChange = path;
                        }
                    }
                }
                ImGui::PopStyleVar();
                if (ImGui::BeginDragDropTarget()) {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
                        const wchar_t* path = (const wchar_t*)payload->Data;
                        std::filesystem::path modelPath(path);
                        std::string modelString = modelPath.string();
                        if (modelString.find(".gltf") != std::string::npos || modelString.find(".obj") != std::string::npos || modelString.find(".fbx") != std::string::npos) {
                            for (int i = 0; i < modelString.size(); i++) {
                                modelString[i] = modelString[i] == '\\' ? '/' : modelString[i];
                            }
                            mModelChange = modelString;
                        }
                    }
                    ImGui::EndDragDropTarget();
                }
                ImGui::TreePop();

                if (shouldDelete) {
                    mesh.Free();
                    mSelectedEntity.RemoveComponent<MeshComponent>();
                }
            }
        }

        // MATERIAL
        if (mSelectedEntity.HasComponent<MaterialComponent>()) {
            if (ImGui::TreeNodeEx(ICON_FA_TREE " Material Component", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen)) {
                auto& material = mSelectedEntity.GetComponent<MaterialComponent>();
                
                bool shouldDelete = false;
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7.0f, 0.7f, 0.7f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7.0f, 0.8f, 0.8f));
                ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.5f));
                if (ImGui::Button(ICON_FA_TRASH " Delete", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
                    shouldDelete = true;
                }
                ImGui::PopStyleColor(3);
                ImGui::PopStyleVar();

                ImGui::Checkbox("Inherit from Model", &material.InheritFromModel);
            
                ImGui::TreePop();
            
                if (shouldDelete) {
                    material.Free();
                    mSelectedEntity.RemoveComponent<MaterialComponent>();
                }
            }
        }

        // SCRIPT
        ScriptComponent& scripts = mSelectedEntity.GetComponent<ScriptComponent>();
        for (int i = 0; i < scripts.Instances.size(); i++) {
            Ref<ScriptComponent::EntityScript> script = scripts.Instances[i];
            ImGui::PushID((UInt64)script->ID);
            if (ImGui::TreeNodeEx(ICON_FA_CODE " Script Component", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen)) {
                bool shouldDelete = false;
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7.0f, 0.7f, 0.7f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7.0f, 0.8f, 0.8f));
                ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.5f));
                if (ImGui::Button(ICON_FA_TRASH " Delete", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
                    shouldDelete = true;
                }
                ImGui::PopStyleColor(3);
                if (ImGui::Button(ICON_FA_QUESTION " Reload", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
                    if (script->Handle)
                        script->Handle->Script->Reload();
                }

                if (script->Handle) {
                    char temp[512];
                    sprintf(temp, "%s %s", ICON_FA_FILE, script->Handle->Path.c_str());
                    if (ImGui::Button(temp, ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
                        String path = Dialog::Open({ ".lua" });
                        if (!path.empty()) {
                            // Load script
                            script->Load(path);
                        }
                    }
                } else {
                    if (ImGui::Button(ICON_FA_FILE " Open...", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
                        String path = Dialog::Open({ ".lua" });
                        if (!path.empty()) {
                            script->Load(path);
                        }
                    }
                }
                ImGui::PopStyleVar();
                if (ImGui::BeginDragDropTarget()) {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
                        const wchar_t* path = (const wchar_t*)payload->Data;
				        std::filesystem::path scriptPath(path);
                        std::string scriptString = scriptPath.string();
                        if (scriptString.find(".lua") != std::string::npos) {
                            for (int i = 0; i < scriptString.size(); i++) {
                                scriptString[i] = scriptString[i] == '\\' ? '/' : scriptString[i];
                            }
                            script->Load(scriptString);
                        }
                    }
                    ImGui::EndDragDropTarget();
                }

                if (shouldDelete) {
                    scripts.Instances.erase(scripts.Instances.begin() + i);
                }
                ImGui::TreePop();
            }
            ImGui::PopID();
        }

        // Audio
        if (mSelectedEntity.HasComponent<AudioSourceComponent>()) {
            if (ImGui::TreeNodeEx(ICON_FA_MUSIC " Audio Source Component", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen)) {
                auto& audio = mSelectedEntity.GetComponent<AudioSourceComponent>();

                bool shouldDelete = false;
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7.0f, 0.7f, 0.7f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7.0f, 0.8f, 0.8f));
                ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.5f));
                if (ImGui::Button(ICON_FA_TRASH " Delete", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
                    shouldDelete = true;
                }
                ImGui::PopStyleColor(3);
                
                if (audio.Handle) {
                    char temp[512];
                    sprintf(temp, "%s %s", ICON_FA_FILE, audio.Handle->Path.c_str());
                    if (ImGui::Button(temp, ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
                        String path = Dialog::Open({ ".mp3", ".wav", ".ogg" });
                        path = StringUtil::Lowercase(path);
                        if (!path.empty()) {
                            audio.Init(path);
                        }
                    }
                } else {
                    if (ImGui::Button(ICON_FA_FILE " Open...", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
                        String path = Dialog::Open({ ".mp3", ".wav", ".ogg" });
                        path = StringUtil::Lowercase(path);
                        if (!path.empty()) {
                            audio.Init(path);
                        }
                    }
                }
                ImGui::PopStyleVar();
                if (ImGui::BeginDragDropTarget()) {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
                        const wchar_t* path = (const wchar_t*)payload->Data;
                        std::filesystem::path audioPath(path);
                        std::string audioString = audioPath.string();
                        audioString = StringUtil::Lowercase(audioString);
                        if (audioString.find(".mp3") != std::string::npos || audioString.find(".wav") != std::string::npos || audioString.find(".ogg") != std::string::npos) {
                            for (int i = 0; i < audioString.size(); i++) {
                                audioString[i] = audioString[i] == '\\' ? '/' : audioString[i];
                            }
                            audio.Init(audioString);
                        }
                    }
                    ImGui::EndDragDropTarget();
                }

                ImGui::Separator();
                ImGui::SliderFloat("Volume", &audio.Volume, 0.0f, 100.0f, "%.1f");
                ImGui::Checkbox("Play On Awake", &audio.PlayOnAwake);
                ImGui::Checkbox("Looping", &audio.Looping);
                ImGui::TreePop();

                if (shouldDelete) {
                    audio.Free();
                    mSelectedEntity.RemoveComponent<AudioSourceComponent>();
                }
            }
        }

        ImGui::Separator();

        // Add component
        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.5f));
        if (ImGui::Button(ICON_FA_PLUS " Add Component", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
            ImGui::OpenPopup("AddComponent");
        }
        if (ImGui::BeginPopup("AddComponent")) {
            if (!mSelectedEntity.HasComponent<MeshComponent>()) {
                if (ImGui::MenuItem(ICON_FA_CUBE " Mesh Component")) {
                    mSelectedEntity.AddComponent<MeshComponent>();
                }
            }
            if (!mSelectedEntity.HasComponent<CameraComponent>()) {
                if (ImGui::MenuItem(ICON_FA_VIDEO_CAMERA " Camera Component")) {
                    mSelectedEntity.AddComponent<CameraComponent>(true);
                }
            }
            if (!mSelectedEntity.HasComponent<AudioSourceComponent>()) {
                if (ImGui::MenuItem(ICON_FA_MUSIC " Audio Source Component")) {
                    mSelectedEntity.AddComponent<AudioSourceComponent>();
                }
            }
            if (!mSelectedEntity.HasComponent<MaterialComponent>()) {
                if (ImGui::MenuItem(ICON_FA_TREE " Material Component")) {
                    mSelectedEntity.AddComponent<MaterialComponent>();
                }
            }
            if (ImGui::MenuItem(ICON_FA_CODE " Script Component")) {
                mSelectedEntity.GetComponent<ScriptComponent>().AddEmptyScript();
            }
            ImGui::EndPopup();
        }
        if (ImGui::Button(ICON_FA_TRASH " Delete", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
            mScene->RemoveEntity(mSelectedEntity);
            mSelectedEntity = nullptr;
        }
        ImGui::PopStyleVar();
    }
    ImGui::End();
}

void Editor::AssetBrowser()
{
    if (!mScene)
        return;

    if (mCurrentDirectory != std::filesystem::path(mBaseDirectory)) {
        if (ImGui::Button(ICON_FA_FOLDER " Back")) {
            mCurrentDirectory = mCurrentDirectory.parent_path();
        }
        ImGui::SameLine();
    }
    ImGui::InputText("##FilePath", mFileNameBuffer, sizeof(mFileNameBuffer));
    ImGui::SameLine();
    if (ImGui::BeginMenu(ICON_FA_PLUS " Create...")) {
        if (ImGui::MenuItem(ICON_FA_FILE " File")) {
            char newBuffer[256];
            sprintf(newBuffer, "%s/%s", mCurrentDirectory.string().c_str(), mFileNameBuffer);
            if (!File::Exists(newBuffer) && strlen(mFileNameBuffer) > 0) {
                File::CreateFileFromPath(newBuffer);
            }
            memset(newBuffer, 0, sizeof(newBuffer));
        }
        if (ImGui::MenuItem(ICON_FA_FOLDER " Directory")) {
            char newBuffer[256];
            sprintf(newBuffer, "%s/%s", mCurrentDirectory.string().c_str(), mFileNameBuffer);
            if (!File::Exists(newBuffer) && strlen(mFileNameBuffer) > 0) {
                File::CreateDirectoryFromPath(newBuffer);
            }
            memset(newBuffer, 0, sizeof(newBuffer));
        }
        if (ImGui::MenuItem(ICON_FA_CODE " Script")) {
            char newBuffer[256];
            sprintf(newBuffer, "%s/%s%s", mCurrentDirectory.string().c_str(), mFileNameBuffer, ".lua");
            if (!File::Exists(newBuffer) && strlen(mFileNameBuffer) > 0) {
                File::CreateFileFromPath(newBuffer);
                File::WriteString(newBuffer, R"(
                    return function(entityID)
                        local self = {}

                        function self.awake()
                        end

                        function self.update(dt)
                        end

                        function self.quit()
                        end

                        return self
                    end
                )");
            }
            memset(mFileNameBuffer, 0, sizeof(mFileNameBuffer));
        }
        if (ImGui::MenuItem(ICON_FA_GLOBE " Scene")) {
            char newBuffer[256];
            sprintf(newBuffer, "%s/%s%s", mCurrentDirectory.string().c_str(), mFileNameBuffer, ".msf");
            if (!File::Exists(newBuffer) && strlen(mFileNameBuffer) > 0) {
                File::CreateFileFromPath(newBuffer);

                Ref<Scene> scene = MakeRef<Scene>();
                SceneSerializer::SerializeScene(scene, newBuffer);
            }
            memset(mFileNameBuffer, 0, sizeof(mFileNameBuffer));
        }
        if (ImGui::MenuItem(ICON_FA_CAMERA_RETRO " Post Process Volume")) {
            char newBuffer[256];
            sprintf(newBuffer, "%s/%s%s", mCurrentDirectory.string().c_str(), mFileNameBuffer, ".mfx");
            if (!File::Exists(newBuffer) && strlen(mFileNameBuffer) > 0) {
                File::CreateFileFromPath(newBuffer);

                PostProcessVolume volume;
                volume.Save(newBuffer);
            }
            memset(mFileNameBuffer, 0, sizeof(mFileNameBuffer));
        }
        ImGui::EndMenu();
    }

    static float padding = 16.0f;
	static float thumbnailSize = 92.0f;
	float cellSize = thumbnailSize + padding;

	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = (int)(panelWidth / cellSize);
	if (columnCount < 1)
		columnCount = 1;
    
    ImGui::Columns(columnCount, 0, false);

    for (auto& directoryEntry : std::filesystem::directory_iterator(mCurrentDirectory))
	{
		const auto& path = directoryEntry.path();
		std::string filenameString = path.filename().string();
		
		ImGui::PushID(filenameString.c_str());
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        
        const char* icon = ICON_FA_FILE;
        if (directoryEntry.is_directory()) {
            icon = ICON_FA_FOLDER;
        }
        auto extension = path.extension().string();
        if (extension.find("hlsl") != std::string::npos) {
            icon = ICON_FA_PAINT_BRUSH;
        } else if (extension.find("lua") != std::string::npos) {
            icon = ICON_FA_CODE;
        } else if (extension.find("png") != std::string::npos || extension.find("jpg") != std::string::npos || extension.find("jpeg") != std::string::npos) {
            icon = ICON_FA_FILE_IMAGE_O;
        } else if (extension.find("gltf") != std::string::npos || extension.find("obj") != std::string::npos || extension.find("fbx") != std::string::npos) {
            icon = ICON_FA_CUBE;
        } else if (extension.find("msf") != std::string::npos) {
            icon = ICON_FA_GLOBE;
        } else if (extension.find(".mp3") != std::string::npos || extension.find(".ogg") != std::string::npos || extension.find(".wav") != std::string::npos) {
            icon = ICON_FA_MUSIC;
        } else if (extension.find(".mfx") != std::string::npos) {
            icon = ICON_FA_CAMERA_RETRO;
        }

        ImGui::PushFont(mRHI->GetLargeIconFont());
        ImVec2 buttonPos = ImGui::GetCursorPos();
        ImGui::Button(icon, ImVec2(thumbnailSize, thumbnailSize));
        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
            ImGui::OpenPopup("FileContextMenu");
        }
        ImGui::PopFont();

        if (ImGui::BeginPopup("FileContextMenu")) {
            if (ImGui::Button("Delete")) {
                File::Delete(directoryEntry.path().string());
            }
            ImGui::EndPopup();
        }
		
        if (ImGui::BeginDragDropSource())
		{
			std::filesystem::path relativePath(path);
			const wchar_t* itemPath = relativePath.c_str();
			ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
			ImGui::EndDragDropSource();
		}
		ImGui::PopStyleColor();
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if (directoryEntry.is_directory())
				mCurrentDirectory /= path.filename();
		}

        float buttonWidth = ImGui::GetItemRectSize().x;
        float contentWidth = ImGui::GetContentRegionAvail().x;
		float offset = (contentWidth - buttonWidth) / 2.0f;
        ImGui::SetCursorPos(ImVec2(buttonPos.x + offset, buttonPos.y + ImGui::GetItemRectSize().y + 10));
        ImGui::Text(filenameString.c_str());
        
        ImGui::NextColumn();
		ImGui::PopID();
	}
	ImGui::Columns(1);
}

void Editor::LogWindow()
{
    if (!mScene)
        return;

    ImGui::Begin(ICON_FA_TERMINAL " Log");
    if (ImGui::Button(ICON_FA_ERASER " Clear")) {
        Logger::sEntries.clear();
    }
    ImGui::SameLine();
    mLogFilter.Draw();
    const char* logChildName = "LogChild"; 
    ImGui::BeginChild(logChildName, ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
    for (auto& entry : Logger::sEntries) {
        if (mLogFilter.PassFilter(entry.Message.c_str()))
            ImGui::TextColored(entry.Color, entry.Message.c_str());
    }
    ImGui::EndChild();
    ImGui::End();
}

void Editor::FXVolumeEditor()
{
    ImGui::Begin(ICON_FA_PAINT_BRUSH " Post Process Volume");
    if (mSelectedVolume) {
        char temp[512];
        sprintf(temp, "%s %s", ICON_FA_FILE, mSelectedVolume->Path.c_str());
        if (ImGui::Button(temp, ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
            String path = Dialog::Open({ ".mfx" });
            if (!path.empty()) {
                mSelectedVolume = AssetManager::Get(path, AssetType::PostFXVolume);
            }
        }
    } else {
        if (ImGui::Button(ICON_FA_FILE " Open...", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
            String path = Dialog::Open({ ".mfx" });
            if (!path.empty()) {
                mSelectedVolume = AssetManager::Get(path, AssetType::PostFXVolume);
            }
        }
    }
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
            const wchar_t* path = (const wchar_t*)payload->Data;
            std::filesystem::path fxPath(path);
            std::string fxString = fxPath.string();
            if (fxString.find(".mfx") != std::string::npos) {
                for (int i = 0; i < fxString.size(); i++) {
                    fxString[i] = fxString[i] == '\\' ? '/' : fxString[i];
                }
                mSelectedVolume = AssetManager::Get(fxString, AssetType::PostFXVolume);
            }
        }
        ImGui::EndDragDropTarget();
    }
    if (mSelectedVolume) {
        auto* volume = &mSelectedVolume->Volume;

        if (ImGui::TreeNodeEx("Geometry Pass", ImGuiTreeNodeFlags_Framed)) {
            ImGui::Checkbox("Visualize Meshlets", &volume->VisualizeMeshlets);
            ImGui::TreePop();
        }
        if (ImGui::TreeNodeEx("Color Grading", ImGuiTreeNodeFlags_Framed)) {
            ImGui::Checkbox("Enable", &volume->EnableColorGrading);
            ImGui::SliderFloat("Brightness", &volume->Brightness, 0.0f, 10.0f, "%.2f");
            ImGui::SliderFloat("Exposure", &volume->Exposure, 0.0f, 10.0f, "%.2f");
            ImGui::SliderFloat("Saturation", &volume->Saturation, -10.0f, 10.0f, "%.2f");
            ImGui::SliderFloat("Contrast", &volume->Contrast, -10.0f, 10.0f, "%.2f");
            ImGui::SliderFloat("Hue Shift", &volume->HueShift, -180.0f, 180.0f, "%.1f");
            ImGui::SliderFloat("Temperature", &volume->Temperature, -1.0f, 1.0f, "%.1f");
            ImGui::SliderFloat("Tint", &volume->Tint, -1.0f, 1.0f, "%.1f");
            if (ImGui::TreeNodeEx("Split Toning", ImGuiTreeNodeFlags_Framed)){
                ImGui::ColorPicker3("Shadows", glm::value_ptr(volume->Shadows), ImGuiColorEditFlags_PickerHueBar);
                ImGui::ColorPicker3("Hightlights", glm::value_ptr(volume->Highlights), ImGuiColorEditFlags_PickerHueBar);
                ImGui::SliderFloat("Balance", &volume->Balance, -100.0f, 100.0f, "%.1f");
                ImGui::TreePop();
            }
            if (ImGui::TreeNodeEx("Color Filter", ImGuiTreeNodeFlags_Framed)) {
                ImGui::ColorPicker3("Color Filter", glm::value_ptr(volume->ColorFilter), ImGuiColorEditFlags_PickerHueBar);
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
        if (ImGui::TreeNodeEx("Depth of Field", ImGuiTreeNodeFlags_Framed)) {
            ImGui::Checkbox("Enable", &volume->EnableDOF);
            ImGui::SliderFloat("Focus Point", &volume->FocusPoint, 0.1f, 200.0f, "%.3f");
            ImGui::SliderFloat("Focal Range", &volume->FocusRange, 0.1f, 200.0f, "%.3f");
            ImGui::TreePop();
        }
        if (ImGui::TreeNodeEx("Composite", ImGuiTreeNodeFlags_Framed)) {
            ImGui::SliderFloat("Gamma", &volume->GammaCorrection, 0.1f, 10.0f);
            ImGui::TreePop();
        }
    }
    ImGui::End();
}

void Editor::DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue, float columnWidth)
{
    if (!mScene)
        return;

    ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];
	ImGui::PushID(label.c_str());
	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();
	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("X", buttonSize))
		values.x = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);
	ImGui::SameLine();
	ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Y", buttonSize))
		values.y = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);
	ImGui::SameLine();
	ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Z", buttonSize))
		values.z = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);
	ImGui::SameLine();
	ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::PopStyleVar();
	ImGui::Columns(1);
	ImGui::PopID();
}

void Editor::EndDockSpace()
{
    ImGui::End();
}
