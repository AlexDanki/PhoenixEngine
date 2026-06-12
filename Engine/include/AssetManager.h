#pragma once
#include <string>
#include <unordered_map>
#include <memory>

class Texture;

class AssetManager
{
public:
	Texture* LoadTexture(const std::string& path);

private:
	std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures;
};