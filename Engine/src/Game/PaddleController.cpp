#include "PaddleController.h"
#include "Input.h"
#include "Log.h"
#include <iostream>

void PaddleController::OnUpdate(float dt)
{
	Transform& transform = GetOwner()->GetTransform();

	if(Input::GetKey(KeyCode::W))
	{
		transform.position.z -= speed * dt;
	}


	if(Input::GetKey(KeyCode::S))
	{
		transform.position.z += speed * dt;
	}

	if (transform.position.z < -5.5f)
	{
		transform.position.z = -5.5f;
	}

	if (transform.position.z > 5.5f)
	{
		transform.position.z = 5.5f;
	}
}

void PaddleController::OnCollisionEnter(GameObject* other)
{
	std::cout << other->GetName() << std::endl;
}