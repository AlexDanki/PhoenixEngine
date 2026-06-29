#pragma once
#include "ScriptComponent.h"
#include "GameObject.h"
class PaddleController : public ScriptComponent
{
public:
	float speed = 10.0f;
	void OnUpdate(float dt) override;
	void OnCollisionEnter(GameObject* other) override;
};