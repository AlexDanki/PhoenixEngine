#include "AssetManager.h"
#include "Texture.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Shader.h"
#include "MeshAsset.h"

AssetManager::AssetManager()
{
	m_assetRoot = "../../../../Assets/";
}

Texture* AssetManager::LoadTexture(const std::string& path)
{

	auto it = m_textures.find(path);

	if(it != m_textures.end())
	{
		return it->second.get();
	}

	auto texture = std::make_unique<Texture>();

	if(!texture->LoadFromFile(m_assetRoot + path))
	{
		return nullptr;
	}

	Texture* texturePtr = texture.get();

	m_textures[path] = std::move(texture);

	return texturePtr;

}

Shader* AssetManager::LoadShader(const std::string& vertPath, const std::string& fragPath)
{
	std::string keyPath = vertPath + "|" + fragPath;
	auto it = m_shaders.find(keyPath);

	if(it != m_shaders.end())
	{
		return it->second.get();
	}

	// Não achou continue
	std::string vertexShaderSrc = ReadFile(vertPath);
	std::string fragShaderSrc = ReadFile(fragPath);

	if(vertexShaderSrc.empty() || fragShaderSrc.empty())
	{
		return nullptr;
	}

	auto shader = std::make_unique<Shader>();
	if(!shader->Create(vertexShaderSrc.c_str(), fragShaderSrc.c_str()))
	{
		std::cout << "Erro ao criar Shader" << std::endl;
		return nullptr;
	}

	Shader* shaderPtr = shader.get();
	m_shaders[keyPath] = std::move(shader);

	return shaderPtr;

}

MeshAsset* AssetManager::LoadMeshAsset(const std::string& path)
{
	auto it = m_meshAssets.find(path);

	if(it != m_meshAssets.end())
	{
		return it->second.get();
	}

	auto meshAsset = std::make_unique<MeshAsset>();

	if (!meshAsset->LoadFromFile(path)) return nullptr;

	MeshAsset* meshAssetPtr = meshAsset.get();
	

	m_meshAssets[path] = std::move(meshAsset);

	return meshAssetPtr;
}

std::string AssetManager::ReadFile(const std::string& path)
{
	std::ifstream file(m_assetRoot + path);

	if(!file.is_open())
	{
		std::cout << "Erro ao abrir arquivo em: " << path << std::endl;
		return "";
	}

	std::stringstream buffer;
	buffer << file.rdbuf();

	return buffer.str();

	file.close();
}