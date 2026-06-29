#include "BallController.h"
#include "GameManager.h"
#include "RigidbodyComponent.h"
#include "Transform.h"
#include "Input.h"
#include <iostream>
#include "Random.h"
#include "Scene.h"

void BallController::OnStart()
{
	GameObject* gameManagerObject = m_scene->GetGameObjectByName("GameManager");

	if (gameManagerObject)
	{
		m_gameManager = gameManagerObject->GetComponent<GameManager>();
	}

	m_rigidbody = GetOwner()->GetComponent<RigidbodyComponent>();
}

void BallController::OnUpdate(float dt)
{
	
	if(Input::GetKey(KeyCode::ENTER) && !m_canMove)
	{
		// Random para o indice de direção
		int direction = Random::Range(0, 3);

		switch(direction)
		{
			case 0:
				// RightTop direction 
				m_dir = glm::vec3(-1, 0, -1);
				break;
			case 1:
				// LeftTop direction
				m_dir = glm::vec3(1, 0, -1);
				break;
			case 2:
				//LeftBottom direction
				m_dir = glm::vec3(1, 0, 1);
				break;
			case 3:
				// RightBottom direction
				m_dir = glm::vec3(-1, 0, 1);
				break;
		}
		m_canMove = true;
	}
	
	if(m_rigidbody && !m_canMove)
	{

		m_transform->position.x = 0;
		m_transform->position.y = m_transform->position.y;
		m_transform->position.z = 0;

		m_rigidbody->velocity = glm::vec3(0.0f);
	}

	if(m_rigidbody && m_canMove)
	{
		m_dir = glm::normalize(m_dir);
		m_rigidbody->velocity = m_dir * m_speed;
	}
	
}

void BallController::OnCollisionEnter(GameObject* other)
{
	if(other->GetTag() == "WallBottom" || other->GetTag() == "WallTop")
	{
		m_dir.z *= -1;
	}

	if(other->GetTag() == "Player" || other->GetTag() == "Enemy")
	{
		// Calcula o offset entre a posição da bola e o centro da Paddle
		float offset = m_transform->position.z - other->GetTransform().position.z;
		// Normaliza o offset
		offset = offset / 2.0f;

		// Apenas inverte a direção no eixo X
		m_dir.x *= -1; 
		// Aplica direção no eixo Z dependendo do offset entre a bola e o centro das Paddle's
		m_dir.z = offset; 
	}

	if(other->GetTag() == "WallLeft" || other->GetTag() == "WallRight")
	{
		if (other->GetTag() == "WallLeft")
		{
			if (m_gameManager)
				m_gameManager->EnemyScored();
		}

		if (other->GetTag() == "WallRight")
		{
			if (m_gameManager)
				m_gameManager->PlayerScored();
		}

		m_canMove = false;
		m_transform->position.x = 0;
		m_transform->position.y = m_transform->position.y;
		m_transform->position.z = 0;

		
	}
}