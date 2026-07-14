#include "RigidbodyComponent.h"
#include "GameObject.h"
#include "SerializeUtils.h"

void RigidbodyComponent::OnAttach()
{
	m_transform = &m_owner->GetTransform();
}

void RigidbodyComponent::PhysicsUpdate(float deltaTime)
{
	float gravitForce = 0.0f;
	if(useGravity)
	{
		gravitForce = 9.8f;
		
	}
	else
	{
		velocity.y = 0;
	}

	velocity.y -= gravitForce * gravityScale * deltaTime;

	m_transform->position.x += velocity.x * deltaTime;
	m_transform->position.y += velocity.y * deltaTime;
	m_transform->position.z += velocity.z * deltaTime;

}

void RigidbodyComponent::Serialize(std::ofstream& file) const
{
	//
	file << "COMPONENT\n";

	file << "RIGIDBODY\n";

	// Gravit scale
	file << "USE_GRAVIT\n";
	file << useGravity << "\n";

	// Velocity
	file << "GRAVIT_SCALE\n";
	file << gravityScale << "\n";

	file << "END_COMPONENT\n";
}