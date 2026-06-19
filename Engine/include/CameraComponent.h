#pragma once
#include "Component.h"
#include <glm/glm.hpp>

class CameraComponent : public Component
{
public:

	float FOV = 45.0f;
	float nearPlane = 0.1f;
	float farPlane = 500.0f;
	bool primary = true;

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjection(const float aspectRatio);
	glm::mat4 GetViewProjection(const float aspectRatio);
};