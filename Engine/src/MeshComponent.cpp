#include "MeshComponent.h"

MeshComponent::MeshComponent(Mesh* mesh)
	: m_mesh(mesh)
{

}

Mesh* MeshComponent::GetMesh() const
{
	return m_mesh;
}