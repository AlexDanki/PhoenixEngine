#pragma once
#include "ScriptComponent.h"

class BallController : public ScriptComponent
{
public:
	glm::vec3 m_dir = glm::vec3(0, 0, 1);
	float m_speed = 10.0;
	void OnUpdate(float dt) override;
	void OnCollisionEnter(GameObject* other) override;
};