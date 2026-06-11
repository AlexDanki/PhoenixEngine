#pragma once
#include <vector>
struct Vertex;
class Mesh
{
public:
	Mesh();
	~Mesh();

	void CreateTriangle();
	void CreateQuad();
	void CreateCube();

	void UploadToGPU();

	unsigned int GetVAO() const { return m_VAO; }

	unsigned int GetVertexCount() const; 
	unsigned int GetIndexCount() const; 

private:
	unsigned int m_VAO = 0;
	unsigned int m_VBO = 0;
	unsigned int m_EBO = 0;

	unsigned int m_vertexCount = 0;

	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
};
