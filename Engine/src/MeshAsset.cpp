
#include "Vertex.h"
#include <iostream>
#include "assimp/importer.hpp"
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include "MeshAsset.h"

bool MeshAsset::LoadFromFile(const std::string& path)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile("../../../../Assets/" + path, aiProcess_Triangulate | aiProcess_FlipUVs);
	aiMesh* mesh = scene->mMeshes[0];

	m_mesh.m_vertices.clear();
	m_mesh.m_indices.clear();

	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.Position.x = mesh->mVertices[i].x;
		vertex.Position.y = mesh->mVertices[i].y;
		vertex.Position.z = mesh->mVertices[i].z;

		vertex.Normal.x = mesh->mNormals[i].x;
		vertex.Normal.y = mesh->mNormals[i].y;
		vertex.Normal.z = mesh->mNormals[i].z;

		if (mesh->HasTextureCoords(0))
		{
			aiVector3D uv = mesh->mTextureCoords[0][i];

			vertex.UV = { uv.x, uv.y };
		}
		else
		{
			vertex.UV = { 0.0f,0.0f };
		}

		m_mesh.m_vertices.push_back(vertex);
	}

	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		unsigned int a = mesh->mFaces[i].mIndices[0];
		unsigned int b = mesh->mFaces[i].mIndices[1];
		unsigned int c = mesh->mFaces[i].mIndices[2];

		m_mesh.m_indices.push_back(a);
		m_mesh.m_indices.push_back(b);
		m_mesh.m_indices.push_back(c);
	}

	m_mesh.UploadToGPU();

	return true;
}

Mesh& MeshAsset::GetMesh()
{
	return m_mesh;
}