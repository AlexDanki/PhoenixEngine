#pragma once
#include "ScriptComponent.h"

class PaddleController : public ScriptComponent
{
public:
	float speed = 10.0f;
	void OnUpdate(float dt) override;
};