#include "Panels/AssetBrowserPanel.h"
#include <iostream>
#include "Core/EditorApplication.h"
#include "Asset/AssetDatabase.h"
#include "Asset/AssetEntry.h"
#include <string>


void AssetBrowserPanel::Draw()
{
	ui::Begin("AssetBrowser");

	if(m_currentDirectory != m_database->GetRootDiretory())
	{
		std::string path = m_currentDirectory.string();
		ui::Text(path.c_str());
		if(ui::Button(" << Back"))
		{
			m_currentDirectory = m_currentDirectory.parent_path();
		}
	}

	if (m_database)
	{
		for (auto& dataEntry : m_database->GetEntriesInDirectory(m_currentDirectory))
		{

			ui::TreeNodeEx(dataEntry->Name.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen);

			/*if (dataEntry->IsDirectory) 
			{
				ui::TreeNodeEx(dataEntry->Name.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen);
			}
			else 
			{
				ui::TreeNodeEx(dataEntry->Name.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen);
			}*/

			if(ui::IsItemClicked())
			{
				if(dataEntry->IsDirectory)
				{
					m_currentAsset = dataEntry;
					m_editor->SetCurrentAsset(dataEntry);
					m_currentDirectory = dataEntry->Path;
				}
			}
		}
	}

	ui::End();

}

void AssetBrowserPanel::SetAssetDatabase(mse::AssetDatabase* database)
{
	m_database = database;
}

void AssetBrowserPanel::SetEditorApplication(EditorApplication* editor)
{
	m_editor = editor;
}

void AssetBrowserPanel::SetCurrentDiretory(const std::filesystem::path& path)
{
	m_currentDirectory = path;
}