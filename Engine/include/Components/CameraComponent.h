#pragma once
#include "Component.h"
#include "Camera/ICamera.h"
#include <glm/glm.hpp>

class CameraComponent : public Component, public ICamera
{
public:

	float FOV = 60.0f;
	float nearPlane = 0.1f;
	float farPlane = 100.0f;
	bool primary = true;

	glm::mat4 GetViewMatrix() const override;
	glm::mat4 GetProjection(const float aspectRatio) const;
	glm::mat4 GetViewProjection(const float aspectRatio) const override;
	glm::vec3 GetPosition() const override;
};