#pragma once
#include "ScriptComponent.h"

class GameManager;
class RigidbodyComponent;

class BallController : public ScriptComponent
{
public:
	glm::vec3 m_dir = glm::vec3(1, 0, 0);
	float m_speed = 10.0;
	bool m_canMove = false;

	void OnStart() override;
	void OnUpdate(float dt) override;
	void OnCollisionEnter(GameObject* other) override;

private:
	GameManager* m_gameManager = nullptr;
	RigidbodyComponent* m_rigidbody = nullptr;
};