//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 12:26:03
//

#include "Editor.hpp"

Editor::Editor(ApplicationSpecs specs)
    : Application(specs)
{
    mCameraEntity = mScene->AddEntity("Editor Camera");
    mCameraEntity->Private = true;
    
    auto& cam = mCameraEntity->AddComponent<CameraComponent>();
    cam.Primary = true;
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

    auto ldr = RendererTools::Get("LDRColorBuffer");

    Profiler::OnUI();
    mRenderer->UI(frame);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
    ImGui::Begin("Viewport");

    mViewportFocused = ImGui::IsWindowFocused();

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

    frame.CommandBuffer->ClearRenderTarget(frame.BackbufferView, 0.0f, 0.0f, 0.0f);
    frame.CommandBuffer->Barrier(ldr->Texture, ResourceLayout::Shader);
    ImGui::Image((ImTextureID)ldr->GetView(ViewType::ShaderResource)->GetDescriptor().GPU.ptr, mViewportSize);
    
    ImGui::End();
    ImGui::PopStyleVar();

    ImGui::End();
}
