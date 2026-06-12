#include "MeshComponent.h"

MeshComponent::MeshComponent(Mesh* mesh, Material* material)
	: m_mesh(mesh),
	m_material(material)
{

}

Mesh* MeshComponent::GetMesh() const
{
	return m_mesh;
}

Material* MeshComponent::GetMaterial() const
{
	return m_material;
}