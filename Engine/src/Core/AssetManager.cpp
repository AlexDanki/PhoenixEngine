#include "AssetManager.h"
#include "Texture.h"
#include "Font.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Shader.h"
#include "MeshAsset.h"
#include "Material.h"

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

Font* AssetManager::LoadFont(const std::string& path)
{
	auto it = m_fonts.find(path);

	if(it != m_fonts.end())
	{
		return it->second.get();
	}

	auto font = std::make_unique<Font>();
	if(!font->Load(path))
	{
		std::cout << "Erro ao carregat Font: " << path << std::endl;
		return nullptr;
	}

	Font* fontPtr = font.get();
	m_fonts[path] = std::move(font);

	return fontPtr;
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
	meshAssetPtr->SetFilePath(path);

	m_meshAssets[path] = std::move(meshAsset);

	return meshAssetPtr;
}

Material* AssetManager::CreateMaterial(const std::string& TexturePath, Shader* shader)
{
	auto material = std::make_unique<Material>(shader);
	material->SetTexture(LoadTexture(TexturePath));
	Material* materialPtr = material.get();
	m_materiais.push_back(std::move(material));

	return materialPtr;
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