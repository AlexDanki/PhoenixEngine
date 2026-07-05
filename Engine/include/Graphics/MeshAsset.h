#pragma once

#include <string>
#include "Mesh.h"

class MeshAsset
{
public:
	bool LoadFromFile(const std::string& path);
	Mesh& GetMesh();
	const Mesh& GetMesh() const;

	const std::string& GetFilePath() const { return m_filePath; }
	void SetFilePath(const std::string& path) { m_filePath = path; }

private:
	Mesh m_mesh;
	std::string m_filePath;
};