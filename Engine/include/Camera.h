#pragma once
#include "Transform.h"
#include <glm/glm.hpp>

class Camera {
public:

	Transform& GetTransform();
	const Transform& GetTransform() const;

	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix(const float aspectRatio) const;

private:
	Transform m_transform;
};