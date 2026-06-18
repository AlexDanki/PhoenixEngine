#pragma once
#include <string>
#include <unordered_map>
#include <memory>

class Texture;
class Shader;
class MeshAsset;

class AssetManager
{
public:
	AssetManager();
	Texture* LoadTexture(const std::string& path);
	Shader* LoadShader(const std::string& vertPath, const std::string& fragPath);
	MeshAsset* LoadMeshAsset(const std::string& path);

private:
	std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures;
	std::unordered_map<std::string, std::unique_ptr<Shader>> m_shaders;
	std::unordered_map<std::string, std::unique_ptr<MeshAsset>> m_meshAssets;

	std::string ReadFile(const std::string& path);
	std::string m_assetRoot;
};