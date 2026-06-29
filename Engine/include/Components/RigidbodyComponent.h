#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include "Transform.h"

class RigidbodyComponent : public Component
{
public:
	glm::vec3 velocity = glm::vec3(0.0);
	float mass = 1;
	bool useGravity = false;
	float gravityScale = 1;

	void OnAttach() override;

	void PhysicsUpdate(float deltaTime);

private:
	Transform* m_transform = nullptr;
};