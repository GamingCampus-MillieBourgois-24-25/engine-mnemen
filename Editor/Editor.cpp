//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 12:26:03
//

#include "Editor.hpp"

#include <FontAwesome/FontAwesome.hpp>

Editor::Editor(ApplicationSpecs specs)
    : Application(specs)
{
    mCameraEntity = mScene->AddEntity("Editor Camera");
    mCameraEntity->Private = true;
    
    auto& cam = mCameraEntity->AddComponent<CameraComponent>();
    cam.Primary = true;

    SetColors();
}

Editor::~Editor()
{

}

void Editor::OnUpdate(float dt)
{
    int width, height;
    mWindow->PollSize(width, height);

    mCamera.UpdateMatrices(std::max(mViewportSize.x, 1.0f), std::max(mViewportSize.y, 1.0f));
    if (mViewportFocused)
        mCamera.Input(dt);

    auto& cam = mCameraEntity->GetComponent<CameraComponent>();
    cam.Primary = true;
    cam.Projection = mCamera.Projection();
    cam.View = mCamera.View();
}

void Editor::OnPhysicsTick()
{

}

void Editor::OnImGui(const Frame& frame)
{
    BeginDockSpace();

    Profiler::OnUI();

    mRenderer->UI(frame);

    // Viewport
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin(ICON_FA_GAMEPAD " Viewport");

        auto ldr = RendererTools::Get("LDRColorBuffer");
        mViewportFocused = ImGui::IsWindowFocused();

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	    mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

        frame.CommandBuffer->ClearRenderTarget(frame.BackbufferView, 0.0f, 0.0f, 0.0f);
        frame.CommandBuffer->Barrier(ldr->Texture, ResourceLayout::Shader);
        ImGui::Image((ImTextureID)ldr->GetView(ViewType::ShaderResource)->GetDescriptor().GPU.ptr, mViewportSize);

        ImGui::End();
        ImGui::PopStyleVar();
    }

    // Entity Tree
    HierarchyPanel();

    // Asset Panel
    AssetPanel();

    // Entity editor
    EntityEditor();

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
            if (ImGui::MenuItem("Exit")) {
                mWindow->Close();
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
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::End();
}

void Editor::EntityEditor()
{
    ImGui::Begin(ICON_FA_WRENCH " Entity Editor");
    ImGui::End();
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
