#pragma once

#include <string>
#include "Mesh.h"

class MeshAsset
{
public:
	bool LoadFromFile(const std::string& path);
	Mesh& GetMesh();

private:
	Mesh m_mesh;
};