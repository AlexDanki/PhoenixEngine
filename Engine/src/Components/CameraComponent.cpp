#include "CameraComponent.h"
#include "Transform.h"
#include "GameObject.h"

glm::mat4 CameraComponent::GetViewMatrix()
{
	return glm::inverse(GetOwner()->GetTransform().GetMatrix());
}

glm::mat4 CameraComponent::GetProjection(const float aspectRatio)
{
	return glm::perspective(glm::radians(FOV), aspectRatio, nearPlane, farPlane);
}

glm::mat4 CameraComponent::GetViewProjection(const float aspectRatio)
{
	return (GetProjection(aspectRatio) * GetViewMatrix());
}