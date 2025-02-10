//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 12:26:03
//

#include "Editor.hpp"

#include <FontAwesome/FontAwesome.hpp>
#include <RHI/Uploader.hpp>

#include <imgui.h>
#include <imgui_internal.h>

Editor::Editor(ApplicationSpecs specs)
    : Application(specs)
{
    mCurrentScenePath = specs.StartScene;
    mScenePlaying = false;
    mCameraEntity = mScene->AddEntity("Editor Camera");
    mCameraEntity->Private = true;
    
    auto& cam = mCameraEntity->AddComponent<CameraComponent>();
    cam.Primary = true;

    SetColors();

    mBaseDirectory = "Assets";
    mCurrentDirectory = "Assets";
    memset(mInputField, 0, sizeof(mInputField));
}

Editor::~Editor()
{

}

void Editor::OnUpdate(float dt)
{
    int width, height;
    mWindow->PollSize(width, height);

    mCamera.UpdateMatrices(std::max(mViewportSize.x, 1.0f), std::max(mViewportSize.y, 1.0f));
    if (mViewportFocused && !mScenePlaying)
        mCamera.Input(dt);
    if (!mScenePlaying)
        UpdateShortcuts();

    auto& cam = mCameraEntity->GetComponent<CameraComponent>();
    cam.Primary = !mScenePlaying;
    cam.Projection = mCamera.Projection();
    cam.View = mCamera.View();
}

void Editor::PostPresent()
{
    if (mMarkForDeletion) {
        mScene->RemoveEntity(mSelectedEntity);
        mSelectedEntity = nullptr;
        mMarkForDeletion = false;
    }
    if (!mModelChange.empty()) {
        if (mSelectedEntity) {
            MeshComponent& mesh = mSelectedEntity->GetComponent<MeshComponent>();
            AssetManager::GiveBack(mesh.MeshAsset->Path);
            mesh.Init(mModelChange);
        }
        mModelChange = "";
    }
    Uploader::Flush();
    AssetManager::Purge();
}

void Editor::OnPhysicsTick()
{

}

void Editor::OnImGui(const Frame& frame)
{
    PROFILE_FUNCTION();

    BeginDockSpace();

    Profiler::OnUI();

    mRenderer->UI(frame);

    // Viewport
    {
        ImGui::Begin(ICON_FA_GAMEPAD " Viewport");

        ImGui::SetCursorPosX(10.0f);
        if (ImGui::Button(ICON_FA_PLAY)) {
            OnAwake();
        }
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_STOP)) {
            if (mScenePlaying)
                OnStop();
        }

        auto ldr = RendererTools::Get("LDRColorBuffer");
        mViewportFocused = ImGui::IsWindowFocused();

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	    mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

        frame.CommandBuffer->ClearRenderTarget(frame.BackbufferView, 0.0f, 0.0f, 0.0f);
        frame.CommandBuffer->Barrier(ldr->Texture, ResourceLayout::Shader);
        ImGui::Image((ImTextureID)ldr->GetView(ViewType::ShaderResource)->GetDescriptor().GPU.ptr, mViewportSize);

        ImGui::End();
    }

    // Entity Tree
    HierarchyPanel();

    // Asset Panel
    AssetPanel();

    // Entity editor
    EntityEditor();

    // Log Window
    LogWindow();

    ImGui::End();
}

void Editor::BeginDockSpace()
{
    static bool dockspaceOpen = true;
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
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
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}
	style.WindowMinSize.x = minWinSizeX;

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu(ICON_FA_FILE " File")) {
            if (ImGui::MenuItem("Exit", "Ctrl+Q")) {
                SceneSerializer::SerializeScene(mScene, mCurrentScenePath);
                mWindow->Close();
            }
            if (ImGui::MenuItem("Save", "Ctrl+S")) {
                SceneSerializer::SerializeScene(mScene, mCurrentScenePath);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu(ICON_FA_TASKS " Window")) {
            if (ImGui::BeginMenu("Theme")) {
                if (ImGui::MenuItem("Mnemen")) SetColors();
                if (ImGui::MenuItem("ImGui Dark")) ImGui::StyleColorsDark();
                if (ImGui::MenuItem("ImGui Light")) ImGui::StyleColorsLight();
                if (ImGui::MenuItem("ImGui Classic")) ImGui::StyleColorsClassic();
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

void Editor::HierarchyPanel()
{
    ImGui::Begin(ICON_FA_GLOBE " Scene Hierarchy");
    for (auto& entity : mScene->GetEntityArray()) {
        if (entity->Private)
            continue;

        char temp[256];
        sprintf(temp, "%s %s", ICON_FA_CUBE, entity->Name.c_str());
        ImGui::PushID((UInt64)entity->ID);
        if (ImGui::Selectable(temp)) {
            mSelectedEntity = entity;
        }
        ImGui::PopID();
    }
    ImGui::End();
}

void Editor::AssetPanel()
{
    ImGui::Begin(ICON_FA_ARCHIVE " Assets");
    if (ImGui::BeginTabBar("Basic")) {
        if (ImGui::BeginTabItem(ICON_FA_FILE_IMAGE_O " Loaded Assets")) {
            const char* tags[] = {
                ICON_FA_QUESTION " Unknown",
                ICON_FA_CUBE " Models",
                ICON_FA_FILE_IMAGE_O " Textures",
                ICON_FA_PAINT_BRUSH " Shaders",
                ICON_FA_SUN_O " Environment Maps",
                ICON_FA_CODE " Scripts"
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
                            ICON_FA_CODE
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
        if (ImGui::BeginTabItem(ICON_FA_FOLDER_O " Asset Directory")) {
            AssetBrowser();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::End();
}

void Editor::EntityEditor()
{
    ImGui::Begin(ICON_FA_WRENCH " Entity Editor");
    if (mSelectedEntity) {
        strcpy(mInputField, mSelectedEntity->Name.c_str());
        ImGui::InputText("##", mInputField, 512);
        mSelectedEntity->Name = String(mInputField);
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7.0f, 0.7f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7.0f, 0.8f, 0.8f));
        if (ImGui::Button(ICON_FA_TRASH)) {
            mMarkForDeletion = true;
        }
        ImGui::PopStyleColor(3);

        // Transform
        if (ImGui::TreeNodeEx(ICON_FA_HOME " Transform", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen)) {
            TransformComponent& transform = mSelectedEntity->GetComponent<TransformComponent>();
            DrawVec3Control("Position", transform.Position);
            DrawVec3Control("Scale", transform.Scale);
            DrawVec3Control("Rotation", transform.Rotation);
            ImGui::TreePop();
        }
        
        // CAMERA
        if (mSelectedEntity->HasComponent<CameraComponent>()) {
            if (ImGui::TreeNodeEx(ICON_FA_CAMERA " Camera Component", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen)) {
                CameraComponent& camera = mSelectedEntity->GetComponent<CameraComponent>();
                ImGui::Checkbox("Primary", &camera.Primary);
                ImGui::SliderFloat("FOV", &camera.FOV, 0.0f, 360.0f);
                ImGui::SliderFloat("Near", &camera.Near, 0.0f, camera.Far);
                ImGui::SliderFloat("Far", &camera.Far, camera.Near, 1000.0f);
                ImGui::TreePop();
            }
        }

        // MESH
        if (mSelectedEntity->HasComponent<MeshComponent>()) {
            if (ImGui::TreeNodeEx(ICON_FA_CUBE " Mesh Component", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen)) {
                auto& mesh = mSelectedEntity->GetComponent<MeshComponent>();
                ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.5f));
                if (mesh.MeshAsset) {
                    char temp[512];
                    sprintf(temp, "%s %s", ICON_FA_FILE, mesh.MeshAsset->Path.c_str());
                    ImGui::Button(temp, ImVec2(ImGui::GetContentRegionAvail().x, 0));
                } else {
                    ImGui::Button(ICON_FA_FILE " Drag something...", ImVec2(ImGui::GetContentRegionAvail().x, 0));
                }
                ImGui::PopStyleVar();
                if (ImGui::BeginDragDropTarget()) {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
                        const wchar_t* path = (const wchar_t*)payload->Data;
                        std::filesystem::path modelPath(path);
                        std::string modelString = modelPath.string();
                        if (modelString.find(".gltf") != std::string::npos) {
                            for (int i = 0; i < modelString.size(); i++) {
                                modelString[i] = modelString[i] == '\\' ? '/' : modelString[i];
                            }
                            mModelChange = modelString;
                        }
                    }
                    ImGui::EndDragDropTarget();
                }
                ImGui::TreePop();
            }
        }

        // SCRIPT
        ScriptComponent& scripts = mSelectedEntity->GetComponent<ScriptComponent>();
        for (int i = 0; i < scripts.Instances.size(); i++) {
            Ref<ScriptComponent::Instance> script = scripts.Instances[i];
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
                
                if (script->Handle.IsLoaded()) {
                    char temp[512];
                    sprintf(temp, "%s %s", ICON_FA_FILE, script->Path.c_str());
                    ImGui::Button(temp, ImVec2(ImGui::GetContentRegionAvail().x, 0));
                } else {
                    ImGui::Button(ICON_FA_FILE " Drag something...", ImVec2(ImGui::GetContentRegionAvail().x, 0));
                }
                ImGui::PopStyleVar();
                if (ImGui::BeginDragDropTarget()) {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
                        const wchar_t* path = (const wchar_t*)payload->Data;
				        std::filesystem::path scriptPath(path);
                        std::string scriptString = scriptPath.string();
                        if (scriptString.find(".wren") != std::string::npos) {
                            for (int i = 0; i < scriptString.size(); i++) {
                                scriptString[i] = scriptString[i] == '\\' ? '/' : scriptString[i];
                            }
                            if (script->Handle.SetSource(AssetManager::Get(scriptString, AssetType::Script)))
                                script->Path = scriptString;
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

        // Add component
        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.5f));
        if (ImGui::Button(ICON_FA_PLUS " Add Component", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
            ImGui::OpenPopup("AddComponent");
        }
        ImGui::PopStyleVar();

        if (ImGui::BeginPopup("AddComponent")) {
            if (!mSelectedEntity->HasComponent<MeshComponent>()) {
                if (ImGui::MenuItem(ICON_FA_CUBE " Mesh Component")) {
                    mSelectedEntity->AddComponent<MeshComponent>();
                }
            }
            if (!mSelectedEntity->HasComponent<CameraComponent>()) {
                if (ImGui::MenuItem(ICON_FA_VIDEO_CAMERA " Camera Component")) {
                    mSelectedEntity->AddComponent<CameraComponent>();
                }
            }
            if (ImGui::MenuItem(ICON_FA_CODE " Script Component")) {
                mSelectedEntity->GetComponent<ScriptComponent>().AddEmptyScript();
            }
            ImGui::EndPopup();
        }
    }
    ImGui::End();
}

void Editor::AssetBrowser()
{
    if (mCurrentDirectory != std::filesystem::path(mBaseDirectory)) {
        if (ImGui::Button("<-")) {
            mCurrentDirectory = mCurrentDirectory.parent_path();
        }
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
        } else if (extension.find("wren") != std::string::npos) {
            icon = ICON_FA_CODE;
        } else if (extension.find("png") != std::string::npos || extension.find("jpg") != std::string::npos || extension.find("jpeg") != std::string::npos) {
            icon = ICON_FA_FILE_IMAGE_O;
        } else if (extension.find("gltf") != std::string::npos || extension.find("obj") != std::string::npos || extension.find("fbx") != std::string::npos) {
            icon = ICON_FA_CUBE;
        }

        ImGui::PushFont(mRHI->GetLargeIconFont());
        ImVec2 buttonPos = ImGui::GetCursorPos();
        ImGui::Button(icon, ImVec2(thumbnailSize, thumbnailSize));
        ImGui::PopFont();
		
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
    ImGui::Begin("Log");
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

void Editor::UpdateShortcuts()
{
    if (Input::IsKeyDown(SDLK_LCTRL)) {
        if (Input::IsKeyPressed(SDLK_Q)) {
            SceneSerializer::SerializeScene(mScene, mCurrentScenePath);
            mWindow->Close();
        }
        if (Input::IsKeyPressed(SDLK_S)) {
            SceneSerializer::SerializeScene(mScene, mCurrentScenePath);
        }
    }
}

void Editor::DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue, float columnWidth)
{
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

void Editor::SetColors()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    // Corners
    style.WindowRounding = 8.0f;
    style.ChildRounding = 8.0f;
    style.FrameRounding = 6.0f;
    style.PopupRounding = 6.0f;
    style.ScrollbarRounding = 6.0f;
    style.GrabRounding = 6.0f;
    style.TabRounding = 6.0f;

    // Colors
    colors[ImGuiCol_Text] =                    ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
    colors[ImGuiCol_TextDisabled] =            ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
    colors[ImGuiCol_WindowBg] =                ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_ChildBg] =                 ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
    colors[ImGuiCol_PopupBg] =                 ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border] =                  ImVec4(0.43f, 0.50f, 0.56f, 0.50f);
    colors[ImGuiCol_BorderShadow] =            ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] =                 ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] =          ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
    colors[ImGuiCol_FrameBgActive] =           ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
    colors[ImGuiCol_TitleBg] =                 ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
    colors[ImGuiCol_TitleBgActive] =           ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] =        ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg] =               ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] =             ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab] =           ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] =    ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] =     ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark] =               ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    colors[ImGuiCol_SliderGrab] =              ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] =        ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
    colors[ImGuiCol_Button] =                  ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_ButtonHovered] =           ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    colors[ImGuiCol_ButtonActive] =            ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
    colors[ImGuiCol_Header] =                  ImVec4(0.20f, 0.25f, 0.29f, 0.55f);
    colors[ImGuiCol_HeaderHovered] =           ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_HeaderActive] =            ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Separator] =               ImVec4(0.43f, 0.50f, 0.56f, 0.50f);
    colors[ImGuiCol_SeparatorHovered] =        ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
    colors[ImGuiCol_SeparatorActive] =         ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_ResizeGrip] =              ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    colors[ImGuiCol_ResizeGripHovered] =       ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] =        ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_Tab] =                     ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_TabHovered] =              ImVec4(0.28f, 0.56f, 1.00f, 0.80f);
    colors[ImGuiCol_TabActive] =               ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_TabUnfocused] =            ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive] =      ImVec4(0.14f, 0.22f, 0.36f, 1.00f);
    colors[ImGuiCol_DockingPreview] =          ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
    colors[ImGuiCol_DockingEmptyBg] =          ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_PlotLines] =               ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] =        ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] =           ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] =    ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] =          ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget] =          ImVec4(1.00f, 0.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] =            ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] =   ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] =       ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] =        ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}
