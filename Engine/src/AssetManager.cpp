#include "AssetManager.h"
#include "Texture.h"
#include <iostream>

Texture* AssetManager::LoadTexture(const std::string& path)
{

	auto it = m_textures.find(path);

	if(it != m_textures.end())
	{
		return m_textures[path].get();
	}

	auto texture = std::make_unique<Texture>();

	if(!texture->LoadFromFile(path))
	{
		return nullptr;
	}

	Texture* texturePtr = texture.get();

	m_textures[path] = std::move(texture);

	return texturePtr;

}