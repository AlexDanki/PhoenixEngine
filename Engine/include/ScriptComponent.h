#pragma once
#include "Component.h"
#include "GameObject.h"

class ScriptComponent:public Component
{
public:
	virtual void OnCreate() {}
	virtual void OnUpdate(float dt) {};
};