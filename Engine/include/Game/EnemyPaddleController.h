#pragma once
#include "ScriptComponent.h"

class GameObject;
class RigidbodyComponent;

class EnemyPaddleController : public ScriptComponent
{
public:
	void OnStart() override;
	void OnUpdate(float dt) override;

private:
	bool m_canCalculateTargetZ = true;
	float m_erro = 2.8f;
	float m_targetZ = 0.f;
	float m_speed = 6.f;
	float m_defaultPosX = 0.f;
	GameObject* m_ball;
	RigidbodyComponent* m_rb = nullptr;
	RigidbodyComponent* m_ballRb = nullptr;
};