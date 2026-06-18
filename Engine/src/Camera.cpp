#include "Camera.h"
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

Transform& Camera::GetTransform()
{
	return m_transform;
}

const Transform& Camera::GetTransform() const
{
	return m_transform;
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::inverse(m_transform.GetMatrix());
}

glm::mat4 Camera::GetProjectionMatrix(const float aspectRatio) const
{
	return glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 1000.0f);
}