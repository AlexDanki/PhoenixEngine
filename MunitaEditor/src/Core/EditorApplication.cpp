#include <Core/EditorApplication.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <iostream>

void EditorApplication::Initialize()
{
	
	m_engine.Initialize();

	m_editorCamera.GetTransform().position.y = 3;
	m_editorCamera.GetTransform().position.z = 20;

	// Scene View panel Config
	m_sceneFramebuffer = std::make_unique<Mse::Framebuffer>(1280, 720);
	m_sceneViewPanel.SetFramebuffer(m_sceneFramebuffer.get());
	m_sceneViewPanel.SetApp(&m_engine);
	m_sceneViewPanel.SetRenderCamera(&m_editorCamera);

	// Game View panel Config
	m_gameFramebuffer = std::make_unique<Mse::Framebuffer>(1280, 720);
	m_gameViewPanel.SetFramebuffer(m_gameFramebuffer.get());
	m_gameViewPanel.SetApp(&m_engine);
	m_gameViewPanel.SetRenderCamera(m_engine.GetScene()->GetPrimaryCamera());

	m_database.Refresh();
	m_assetBrowserPanel.SetAssetDatabase(&m_database);
	m_assetBrowserPanel.SetEditorApplication(this);
	m_assetBrowserPanel.SetCurrentDiretory(m_database.GetRootDiretory());

	InitializeImGui();
	
}

void EditorApplication::Run()
{
	Initialize();

	while(m_engine.IsRunning())
	{
		// Inicia o frame
		m_engine.StartFrame();

		// Controla FizedUpdate
		m_engine.FixedStep();

		// Game Update
		m_engine.Update();

		m_sceneFramebuffer->Bind();
		// Renderiza
		m_engine.Render();
		m_sceneFramebuffer->Unbind();

		StartImGuiFrame();

		// Desenha o Docksapce
		DrawDockSpace();

		DrawPanels();
		
		
		EndImGuiFrame();

		// Finaliza o Frame
		m_engine.EndFrame();
	}

	ShutDown();

}

void EditorApplication::DrawDockSpace()
{
	//  Definindo Dockspace flags 
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

	// Pegando a Viewport
	ImGuiViewport* viewport = ImGui::GetMainViewport();

	// configurando proxima janela
	ImGui::SetNextWindowPos(viewport->Pos);

	ImGui::SetNextWindowSize(viewport->Size);

	ImGui::SetNextWindowViewport(viewport->ID);

	// REmovendo toda a decoração da janela
	windowFlags |= ImGuiWindowFlags_NoTitleBar;
	windowFlags |= ImGuiWindowFlags_NoCollapse;
	windowFlags |= ImGuiWindowFlags_NoResize;
	windowFlags |= ImGuiWindowFlags_NoMove;
	windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	windowFlags |= ImGuiWindowFlags_NoNavFocus;

	// Removendo arredondamentos
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	// Criando a janela
	ImGui::Begin("Docspace", nullptr, windowFlags);

	// Definindo um Id para o Docspace 
	ImGuiID dockspaceID = ImGui::GetID("MunitaEditor");

	ImGui::DockSpace(dockspaceID,ImVec2(0.0f, 0.0f));

	ImGui::End();

	ImGui::PopStyleVar(2);
}

void EditorApplication::DrawPanels()
{
	m_hierarchyPanel.Draw();
	m_inspectorPanel.Draw();
	m_assetBrowserPanel.Draw();
	m_editorCamera.Update(m_engine.GetDeltaTime());
	m_gameViewPanel.Draw();
	m_sceneViewPanel.Draw();
	
}

void EditorApplication::ShutDown()
{
	ShutDownImGui();
	m_engine.ShutDown();
}

// IM_GUI
void EditorApplication::InitializeImGui()
{

	GLFWwindow* window = m_engine.GetNativeWindow();

	if (!window)
	{
		std::cout << "WINDOW NÃO APARECE : ( \n";
	}


	IMGUI_CHECKVERSION();

	ImGui::CreateContext();

	ImGui_ImplGlfw_InitForOpenGL(window, true);

	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui_ImplOpenGL3_Init("#version 330");
}

void EditorApplication::StartImGuiFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void EditorApplication::EndImGuiFrame()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EditorApplication::ShutDownImGui()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyPlatformWindows();
	ImGui::DestroyContext();
}

void EditorApplication::SetCurrentAsset(const mse::AssetEntry* assetEntry)
{
	m_currentAsset = assetEntry;
}

const mse::AssetEntry* EditorApplication::GetCurrentAsset() const
{
	return m_currentAsset;
}