#pragma once
#include <glm/glm.hpp>
#include "Core/Transform.h"

class ICamera
{
public:
	virtual ~ICamera() = default;

	virtual glm::mat4 GetViewMatrix() const = 0;

	virtual glm::mat4 GetViewProjection(const float aspect) const = 0;

	virtual glm::vec3 GetPosition() const = 0;
};