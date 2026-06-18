#include "RigidbodyComponent.h"
#include "GameObject.h"

void RigidbodyComponent::OnAttach()
{
	m_transform = &m_owner->GetTransform();
}

void RigidbodyComponent::PhysicsUpdate(float deltaTime)
{
	if(useGravity)
	{
		velocity.y -= 9.8f * gravityScale * deltaTime;
	}
	else
	{
		velocity.y = 0;
	}

	m_transform->position.y += velocity.y * deltaTime;

}