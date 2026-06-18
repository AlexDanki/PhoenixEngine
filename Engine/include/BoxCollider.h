#pragma once
#include "Component.h"
#include <glm//glm.hpp>
#include "Transform.h"
#include <array>
class BoxCollider : public Component
{
public:
	BoxCollider();
	bool uniformSize = true;
	bool isTrigger = false;
	glm::vec3 center = glm::vec3(0.0);
	glm::vec3 size = glm::vec3(1.0);

	void UpdateBounds();
	void OnAttach() override;
	std::array<glm::vec3, 8> GetCorners();
	bool Intersects(BoxCollider& other);
	float GetTop() { return GetMax().y; };
	float GetBottom() { return GetMin().y; };

	glm::vec3 GetMin();
	glm::vec3 GetMax();

private:

	Transform* m_transform = nullptr;

	glm::vec3 min;
	glm::vec3 max;
	
};