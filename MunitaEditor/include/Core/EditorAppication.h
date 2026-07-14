#pragma once
#include "App.h"
#include "Panels/HierarchyPanel.h"
#include "Panels/inspectorPanel.h"
#include "Panels/AssetBrowserPanel.h"
#include "Panels/SceneViewPanel.h"
#include "Panels/GameViewPanel.h"
#include "Graphics/Framebuffer.h"

class EditorApplication
{
public:

	void DrawDockSpace();
	void DrawPanels();
	void Run();

	void InitializeImGui();
	void ShutDownImGui();

	void StartImGuiFrame();
	void EndImGuiFrame();

private:
	App m_engine;
	std::unique_ptr<Mse::Framebuffer> m_sceneFramebuffer;

	HierarchyPanel m_hierarchyPanel;
	InspectorPanel m_inspectorPanel;
	AssetBrowserPanel m_assetBrowserPanel;
	SceneViewPanel m_sceneViewPanel;
	GameViewPanel m_gameViewPanel;

	void Initialize();
	void ShutDown();
};