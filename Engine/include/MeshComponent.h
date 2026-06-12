
#include "Component.h"

class Mesh;
class Material;

class MeshComponent : public Component
{
public:
	explicit MeshComponent(Mesh* mesh, Material* material);

	Mesh* GetMesh() const;
	Material* GetMaterial() const;

private:
	Mesh* m_mesh;
	Material* m_material;
};