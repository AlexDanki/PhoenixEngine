#pragma once

#include "App.h"
#include "Panels/HierarchyPanel.h"
#include "Panels/inspectorPanel.h"
#include "Panels/AssetBrowserPanel.h"
#include "Panels/SceneViewPanel.h"
#include "Panels/GameViewPanel.h"
#include "Graphics/Framebuffer.h"
#include "Graphics/EditorCamera.h"
#include "Asset/AssetDatabase.h"
#include "Asset/AssetEntry.h"

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

	void SetCurrentAsset(const mse::AssetEntry* assetEntry);
	const mse::AssetEntry* GetCurrentAsset() const;

private:
	App m_engine;
	std::unique_ptr<Mse::Framebuffer> m_sceneFramebuffer;
	std::unique_ptr<Mse::Framebuffer> m_gameFramebuffer;

	HierarchyPanel m_hierarchyPanel;
	InspectorPanel m_inspectorPanel;
	AssetBrowserPanel m_assetBrowserPanel;
	SceneViewPanel m_sceneViewPanel;
	GameViewPanel m_gameViewPanel;
	EditorCamera m_editorCamera;
	mse::AssetDatabase m_database;
	const mse::AssetEntry* m_currentAsset;

	void Initialize();
	void ShutDown();
};