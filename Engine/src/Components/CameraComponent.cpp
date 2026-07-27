#include "CameraComponent.h"
#include "Transform.h"
#include "GameObject.h"

glm::mat4 CameraComponent::GetViewMatrix() const 
{
	return glm::inverse(GetOwner()->GetTransform().GetMatrix());
}

glm::mat4 CameraComponent::GetProjection(const float aspectRatio) const
{
	return glm::perspective(glm::radians(FOV), aspectRatio, nearPlane, farPlane);
}

glm::mat4 CameraComponent::GetViewProjection(const float aspectRatio) const
{
	return (GetProjection(aspectRatio) * GetViewMatrix());
}

glm::vec3 CameraComponent::GetPosition() const
{
	return GetOwner()->GetTransform().GetPosition();
}