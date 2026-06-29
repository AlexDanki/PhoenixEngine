#include "EnemyPaddleController.h"
#include "Scene.h"
#include "RigidbodyComponent.h"
#include <iostream>
#include "Random.h"

void EnemyPaddleController::OnStart()
{
	m_ball = m_scene->GetGameObjectByName("Ball");
	m_rb = GetOwner()->GetComponent<RigidbodyComponent>();
	m_defaultPosX = m_transform->position.x;

	// Se não recebe apontar para o rigidbody da bola
	m_ballRb = m_ball->GetComponent<RigidbodyComponent>();

	// Quando apontar para o RigidBody verificar se ele se move em direção a Paddle
	// e informar que pode calcular o target
	m_canCalculateTargetZ = m_ballRb->velocity.x > 0;
}

void EnemyPaddleController::OnUpdate(float dt)
{
	if (!m_ball || !m_rb) return;

	float ballZ = m_ball->GetTransform().position.z;
	float positionZ = m_transform->position.z;
	m_transform->position.x = m_defaultPosX;

	// Verifica a direção da velocidade no eixo X 
	bool moveToForward = m_ballRb->velocity.x < 0.0f;
	// Define um offset de alinhamento com a bola no eixo Z
	float tolerance = 1;

	// Se não estiver se movendo para longe
	if (!moveToForward)
	{
		// Se puder calcular realizar o cálculo
		if (m_canCalculateTargetZ)
		{
			// Calculo para o alvo da posição da EnemyPaddle no eixo Z
			m_targetZ = Random::Range(-m_erro, m_erro);
			// Depois do calculo mudar estado para que não se calcule de novo antes da hora
			m_canCalculateTargetZ = false;

		}

		float targetZ = ballZ + m_targetZ;
		float distanceZ = targetZ - positionZ;

		// Verificar a posição no eixo Z está fora do que é tolerado
		if (distanceZ > tolerance || distanceZ < -tolerance)
		{
			// Alinhando a Paddle no eixo Z
			if (m_targetZ + ballZ > positionZ)
			{
				m_rb->velocity.z = m_speed;
			}
			else if (m_targetZ + ballZ < positionZ)
			{
				m_rb->velocity.z = -m_speed;
			}
		}
		// Se dentro dos lmites de tolerância a Paddle não precisa se mover
		else
		{
			m_rb->velocity.z = 0;
		}
	}
	else
	{
		m_canCalculateTargetZ = true;
		m_rb->velocity.z = 0;
	}
}