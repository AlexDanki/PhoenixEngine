#include "BallController.h"
#include "RigidbodyComponent.h"

void BallController::OnUpdate(float dt)
{
	RigidbodyComponent* rb = GetOwner()->GetComponent<RigidbodyComponent>();

	if(rb)
	{
		rb->velocity = m_dir * m_speed;
	}
	
	
	
}

void BallController::OnCollisionEnter(GameObject* other)
{
	if(other->GetTag() == "WallBottom" || other->GetTag() == "WallTop")
	{
		m_dir.z *= -1;
	}
}