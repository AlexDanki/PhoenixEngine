#pragma once
#include "Core/Transform.h"
#include <glm/glm.hpp>
#include "Camera/ICAmera.h"

class EditorCamera : public ICamera
{
public:

	glm::vec3 GetPosition() const;

	Transform& GetTransform();
	const Transform& GetTransform() const;

	glm::mat4 GetViewMatrix() const override;
	glm::mat4 GetProjection(float aspect) const;
	glm::mat4 GetViewProjection(float aspect) const override;

	glm::vec3 GetForward() const;
	glm::vec3 GetBackward() const;
	glm::vec3 GetUp() const;
	glm::vec3 GetDown() const;
	glm::vec3 GetRight() const;
	glm::vec3 GetLeft() const;

	void Update(float deltaTime);

private:
	float m_yaw = -90;
	float m_pitch = 0.0;
	float m_moveSpeed = 10.0f;
	float m_mouseSensitive = 1.0f;

	glm::vec2 m_lastMousePosition = glm::vec2(0.0);
	bool m_firstMouseFrame = true;

	Transform m_transform;

	void ProcessMouse(float deltaTime);
	void ProcessKeyboard(float deltaTime);
};