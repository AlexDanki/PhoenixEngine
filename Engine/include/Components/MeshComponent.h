
#include "Component.h"

class Mesh;
class MeshAsset;
class Material;

class MeshComponent : public Component
{
public:
	static constexpr const char* ComponentName = "MeshComponent";

	explicit MeshComponent();
	explicit MeshComponent(Mesh* mesh, Material* material);
	explicit MeshComponent(MeshAsset* meshAsset, Material* material);
	void OnAttach() override;
	Mesh* GetMesh() const;
	Material* GetMaterial() const;

	void Serialize(std::ofstream& file) const override;

private:
	Mesh* m_mesh = nullptr;
	MeshAsset* m_meshAsset = nullptr;
	Material* m_material = nullptr;
};