#pragma once
#include "Panels/EditorPanel.h"
#include <filesystem>
#include <iostream>
#include "Asset/AssetDatabase.h"

class mse::AssetDatabase;
class mse::AssetEntry;
class EditorApplication;

class AssetBrowserPanel : public EditorPanel
{
public:
	void Draw() override;
	void SetAssetDatabase(mse::AssetDatabase* datadabse);
	void SetEditorApplication(EditorApplication* editor);
	void SetCurrentDiretory(const std::filesystem::path& path);

private:
	mse::AssetDatabase* m_database = nullptr;
	const mse::AssetEntry* m_currentAsset = nullptr;
	std::filesystem::path m_currentDirectory;
	EditorApplication* m_editor = nullptr;
};