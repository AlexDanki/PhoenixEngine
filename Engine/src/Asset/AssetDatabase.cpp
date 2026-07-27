#include "Asset/AssetDatabase.h"
#include <iostream>
#include <filesystem>
#include "Asset/AssetType.h"

void mse::AssetDatabase::Refresh()
{
	// Limpa o array de dados antes de atualizar
	m_entries.clear();

	for(auto& directoryEntry : std::filesystem::recursive_directory_iterator(GetRootDiretory()))
	{
		// Cria um AssetEntry para registro dos arquivos
		mse::AssetEntry asset;

		// Preenche o AssetEntry
		asset.Type = mse::AssetType::Unknown;
		asset.Name = directoryEntry.path().filename().string();
		asset.Path = directoryEntry.path();
		asset.IsDirectory = directoryEntry.is_directory();

		// Seta o tipo do arquivo
		SetDataType(asset);

		// Adiciona o AssetEntry ao array de dados
		m_entries.push_back(asset);
	}
}


// Retorna todos os AssetEntries guardados
std::vector<mse::AssetEntry>& mse::AssetDatabase::GetEntries()
{
	return m_entries; 
}

// Retorna todos os AssetEntries guardados
const std::vector<mse::AssetEntry>& mse::AssetDatabase::GetEntries() const
{
	return m_entries; 
}

std::vector<const mse::AssetEntry*> mse::AssetDatabase::GetEntriesInDirectory(const std::filesystem::path& path)
{
	std::vector<const mse::AssetEntry*> result;

	for(const auto& asset : m_entries)
	{
		if(asset.Path.parent_path() == path)
		{

			result.push_back(&asset);
		}
	}

	return result;

}

// Seta o tipo do arquivo
void mse::AssetDatabase::SetDataType( mse::AssetEntry& assetEntry)
{
	if(assetEntry.IsDirectory)
	{
		assetEntry.Type = mse::AssetType::Folder;
	}
	else if(assetEntry.Path.extension() == ".fbx")
	{
		assetEntry.Type = mse::AssetType::Model;
	}
	else if(assetEntry.Path.extension() == ".png")
	{
		assetEntry.Type = mse::AssetType::Texture;
	}
	else if(assetEntry.Path.extension() == ".ttf")
	{
		assetEntry.Type = mse::AssetType::Font;
	}
	else if(assetEntry.Path.extension() == ".vert")
	{
		assetEntry.Type = mse::AssetType::Shader;
	}
	else if(assetEntry.Path.extension() == ".frag")
	{
		assetEntry.Type = mse::AssetType::Shader;
	}
}

const std::filesystem::path& mse::AssetDatabase::GetRootDiretory() const 
{
	return m_rootDirectory;
}