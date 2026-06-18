#pragma once
#include "ScriptComponent.h"

enum RotateAxis
{
	X,
	Y,
	Z
};

class RotatorScript : public ScriptComponent
{
public:
	RotateAxis axis = RotateAxis::Y;
	float rotateSpeed = 20;

	void OnUpdate(float dt) override
	{
		if(axis == RotateAxis::X)
		{
			GetOwner()->GetTransform().rotation.x += rotateSpeed * dt;
		}
		else if (axis == RotateAxis::Y)
		{
			GetOwner()->GetTransform().rotation.y += rotateSpeed * dt;
		}
		else if (axis == RotateAxis::Z)
		{
			GetOwner()->GetTransform().rotation.z += rotateSpeed * dt;
		}
		
	}
};