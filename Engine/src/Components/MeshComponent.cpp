#include "MeshComponent.h"
#include "GameObject.h"
#include "MeshAsset.h"

MeshComponent::MeshComponent(Mesh* mesh, Material* material)
	: m_mesh(mesh),
	m_material(material)
{

}

MeshComponent::MeshComponent(MeshAsset* meshAsset, Material* material)
	: m_meshAsset(meshAsset),
	m_material(material)
{

}

void MeshComponent::OnAttach(){}

Mesh* MeshComponent::GetMesh() const
{
	if(m_meshAsset)
	{
		return &m_meshAsset->GetMesh();
	}

	return m_mesh;
}

Material* MeshComponent::GetMaterial() const
{
	return m_material;
}

void MeshComponent::Serialize(std::ofstream& file) const
{
	file << "COMPONENT\n";
	file << "MESH_RENDERER\n";

	if(m_meshAsset)
	{
		file << "MESH\n";
		file << m_meshAsset->GetFilePath() << "\n";
	}
	else
	{
		// Mesh gerada em tempo de execução, não há caminho de arquivo para serializar
	}
	

	file << "END_MESH_RENDERER\n";
}

void MeshComponent::Deserialize(std::ifstream& file) const
{

}