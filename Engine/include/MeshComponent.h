
#include "Component.h"

class Mesh;


class MeshComponent : public Component
{
public:
	explicit MeshComponent(Mesh* mesh);

	Mesh* GetMesh() const;

private:
	Mesh* m_mesh;
};