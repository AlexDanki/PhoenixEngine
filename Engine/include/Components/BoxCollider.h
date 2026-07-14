#pragma once
#include "Component.h"
#include <glm//glm.hpp>
#include "Transform.h"
#include <array>
class BoxCollider : public Component
{
public:
	BoxCollider();

	glm::vec3 m_center = glm::vec3(0.0);
	glm::vec3 m_size = glm::vec3(1.0);

	bool uniformSize = true;
	bool isTrigger = false;
	

	void UpdateBounds();
	void OnAttach() override;
	void Serialize(std::ofstream& file) const override;
	std::array<glm::vec3, 8> GetCorners();
	bool Intersects(BoxCollider& other);

	float GetTop() { return GetMax().y; };
	float GetBottom() { return GetMin().y; };

	glm::vec3 GetMin();
	glm::vec3 GetMax();

	glm::vec3 GetCenter() { return m_center; }
	glm::vec3 GetSize() { return m_size; }

	void SetCenter(glm::vec3 center);
	void SetSize(glm::vec3 size);

private:
	

	Transform* m_transform = nullptr;

	glm::vec3 min;
	glm::vec3 max;
	
};