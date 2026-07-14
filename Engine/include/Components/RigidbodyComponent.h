#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include "Transform.h"
#include <fstream>

class RigidbodyComponent : public Component
{
public:
	
	float mass = 1;

	bool useGravity = false;
	float gravityScale = 1;
	glm::vec3 velocity = glm::vec3(0.0);

	void OnAttach() override;
	void Serialize(std::ofstream& file) const override;
	void PhysicsUpdate(float deltaTime);

private:
	Transform* m_transform = nullptr;
};