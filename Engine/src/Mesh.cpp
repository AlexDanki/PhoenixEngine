#include "Mesh.h"
#include "glad/glad.h"
#include "Vertex.h"
#include "OBJLoader.h"
#include <iostream>
#include "assimp/importer.hpp"
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Mesh::Mesh(){}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
	glDeleteVertexArrays(1, &m_VAO);
}

void Mesh::CreateTriangle()
{
	m_vertices = {
		{ glm::vec3(-0.5f, -0.5f, 0.0f) },
		{ glm::vec3(0.5f, -0.5f, 0.0f) },
		{ glm::vec3(0.0f,  0.5f, 0.0f) }
	};

	m_indices = {0, 1, 2};

	UploadToGPU();

}

void Mesh::CreateQuad()
{
	m_vertices =
	{
		{
			glm::vec3(-0.5f,  0.5f, 0.0f),
			glm::vec3(0.0f,  0.0f, 1.0f),
			glm::vec2(0,1),
		},

		{
			glm::vec3(0.5f,  0.5f, 0.0f),
			glm::vec3(0.0f,  0.0f, 1.0f),
			glm::vec2(1,1),
		},

		{
			glm::vec3(0.5f, -0.5f, 0.0f),
			glm::vec3(0.0f,  0.0f, 1.0f),
			glm::vec2(1,0),
		},

		{
			glm::vec3(-0.5f, -0.5f, 0.0f),
			glm::vec3(0.0f,  0.0f, 1.0f),
			glm::vec2(0,0),
		}
	};

	m_indices =
	{
		0, 2, 1,
		0, 3, 2
	};

	UploadToGPU();
}

void Mesh::CreateCube()
{
	m_vertices =
	{
		// Frente (normal +Z)
	// Frente (normal +Z)
{glm::vec3{-0.5f, -0.5f,  0.5f}, glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec2(0,1)},
{glm::vec3{ 0.5f, -0.5f,  0.5f}, glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec2(1,1)},
{glm::vec3{ 0.5f,  0.5f,  0.5f}, glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec2(1,0)},
{glm::vec3{-0.5f,  0.5f,  0.5f}, glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec2(0,0)},

// Trás (normal -Z)
{glm::vec3{-0.5f, -0.5f, -0.5f}, glm::vec3{0.0f, 0.0f, -1.0f}, glm::vec2(0,1)},
{glm::vec3{ 0.5f, -0.5f, -0.5f}, glm::vec3{0.0f, 0.0f, -1.0f}, glm::vec2(1,1)},
{glm::vec3{ 0.5f,  0.5f, -0.5f}, glm::vec3{0.0f, 0.0f, -1.0f}, glm::vec2(1,0)},
{glm::vec3{-0.5f,  0.5f, -0.5f}, glm::vec3{0.0f, 0.0f, -1.0f}, glm::vec2(0,0)},

// Esquerda (normal -X)
{glm::vec3{-0.5f, -0.5f, -0.5f}, glm::vec3{-1.0f, 0.0f, 0.0f}, glm::vec2(0,1)},
{glm::vec3{-0.5f, -0.5f,  0.5f}, glm::vec3{-1.0f, 0.0f, 0.0f}, glm::vec2(1,1)},
{glm::vec3{-0.5f,  0.5f,  0.5f}, glm::vec3{-1.0f, 0.0f, 0.0f}, glm::vec2(1,0)},
{glm::vec3{-0.5f,  0.5f, -0.5f}, glm::vec3{-1.0f, 0.0f, 0.0f}, glm::vec2(0,0)},

// Direita (normal +X)
{glm::vec3{ 0.5f, -0.5f, -0.5f}, glm::vec3{1.0f, 0.0f, 0.0f}, glm::vec2(0,1)},
{glm::vec3{ 0.5f, -0.5f,  0.5f}, glm::vec3{1.0f, 0.0f, 0.0f}, glm::vec2(1,1)},
{glm::vec3{ 0.5f,  0.5f,  0.5f}, glm::vec3{1.0f, 0.0f, 0.0f}, glm::vec2(1,0)},
{glm::vec3{ 0.5f,  0.5f, -0.5f}, glm::vec3{1.0f, 0.0f, 0.0f}, glm::vec2(0,0)},

// Topo (normal +Y)
{glm::vec3{-0.5f,  0.5f, -0.5f}, glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec2(0,1)},
{glm::vec3{-0.5f,  0.5f,  0.5f}, glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec2(1,1)},
{glm::vec3{ 0.5f,  0.5f,  0.5f}, glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec2(1,0)},
{glm::vec3{ 0.5f,  0.5f, -0.5f}, glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec2(0,0)},

// Base (normal -Y)
{glm::vec3{-0.5f, -0.5f, -0.5f}, glm::vec3{0.0f, -1.0f, 0.0f}, glm::vec2(0,1)},
{glm::vec3{-0.5f, -0.5f,  0.5f}, glm::vec3{0.0f, -1.0f, 0.0f}, glm::vec2(1,1)},
{glm::vec3{ 0.5f, -0.5f,  0.5f}, glm::vec3{0.0f, -1.0f, 0.0f}, glm::vec2(1,0)},
{glm::vec3{ 0.5f, -0.5f, -0.5f}, glm::vec3{0.0f, -1.0f, 0.0f}, glm::vec2(0,0)},


	};

	m_indices =
	{
		0, 1, 2,
		0, 2, 3,
		4, 6, 5,
		4, 7, 6,
		8, 9, 10,
		8, 10, 11,
		12, 14, 13,
		12, 15, 14,
		16, 17, 18,
		16, 18, 19,
		20, 22, 21,
		20, 23, 22
	};

	UploadToGPU();
}

void Mesh::CreatePlane()
{
	m_vertices =
	{
		{glm::vec3{-10.0f,  0.0f, -10.0f}, glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec2(0,1)},
		{glm::vec3{-10.0f,  0.0f,  10.0f}, glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec2(1,1)},
		{glm::vec3{ 10.0f,  0.0f,  10.0f}, glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec2(1,0)},
		{glm::vec3{ 10.0f,  0.0f, -10.0f}, glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec2(0,0)},
	};

	m_indices =
	{
		0, 1, 2,
		0, 2, 3
	};

	UploadToGPU();
}

void Mesh::CreateObjectFromFile(const std::string& path)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile("../../../../Assets/" + path, aiProcess_Triangulate | aiProcess_FlipUVs);
	aiMesh* mesh = scene->mMeshes[0];

	m_vertices.clear();
	m_indices.clear();

	for(int i = 0; i < mesh->mNumVertices; i++)
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
			vertex.UV ={ 0.0f,0.0f };
		}

		m_vertices.push_back(vertex);
	}
	
	for(int i = 0; i < mesh->mNumFaces; i++)
	{
		unsigned int a = mesh->mFaces[i].mIndices[0];
		unsigned int b = mesh->mFaces[i].mIndices[1];
		unsigned int c = mesh->mFaces[i].mIndices[2];

		m_indices.push_back(a);
		m_indices.push_back(b);
		m_indices.push_back(c);
	}

	UploadToGPU();

}

void Mesh::UploadToGPU()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV));
	glEnableVertexAttribArray(2);
}

unsigned int Mesh::GetVertexCount() const
{

	return static_cast<unsigned int>(m_vertices.size()); // Each vertex has 3 components (x, y, z)
}

unsigned int Mesh::GetIndexCount() const
{

	return static_cast<unsigned int>(m_indices.size()); // Each vertex has 3 components (x, y, z)
}