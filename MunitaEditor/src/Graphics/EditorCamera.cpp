#include "Graphics/EditorCamera.h"
#include "Core/Input.h"
#include <algorithm>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

void EditorCamera::Update(float deltaTime)
{
	ProcessMouse(deltaTime);
	ProcessKeyboard(deltaTime);
}

void EditorCamera::ProcessMouse(float deltaTime)
{
	glm::vec2 mouse = Input::GetMousePos();

	if(m_firstMouseFrame)
	{
		m_lastMousePosition = mouse;
		m_firstMouseFrame = false;
	}

	glm::vec2 delta = mouse - m_lastMousePosition;
	m_lastMousePosition = mouse;

	if(!Input::GetMouseButton(MouseButton::RIGHT))
	{
		m_lastMousePosition = mouse;
		return;
	}

	m_yaw += delta.x * m_mouseSensitive;
	m_pitch -= delta.y * m_mouseSensitive;

	if(m_pitch < -89.9)
	{
		m_pitch = -89.9;
	}else if(m_pitch > 89.9)
	{
		m_pitch = 89.9;
	}

}
void EditorCamera::ProcessKeyboard(float deltaTime)
{
	//std::cout << GetForward().z << "\n";
	if(!Input::GetMouseButton(MouseButton::RIGHT))
	{
		return;
	}

	Transform& transform = GetTransform();
	float speed =  m_moveSpeed * deltaTime;

	if(Input::GetKey(KeyCode::W))
	{
		transform.SetPosition(GetPosition() + GetForward() * speed);

	}
	if(Input::GetKey(KeyCode::S))
	{
		transform.SetPosition(GetPosition() + GetBackward() * speed);

	}
	if(Input::GetKey(KeyCode::A))
	{
		transform.SetPosition(GetPosition() + GetLeft() * speed);

	}
	if(Input::GetKey(KeyCode::D))
	{
		transform.SetPosition(GetPosition() + GetRight() * speed);

	}
	if(Input::GetKey(KeyCode::Q))
	{
		transform.SetPosition(GetPosition() + GetDown() * speed);

	}
	if(Input::GetKey(KeyCode::E))
	{
		transform.SetPosition(GetPosition() + GetUp() * speed);

	}

}

glm::vec3 EditorCamera::GetPosition() const
{
	return m_transform.GetPosition();
}

Transform& EditorCamera::GetTransform()
{
	return m_transform;
}

const Transform& EditorCamera::GetTransform() const
{
	return m_transform;
}

glm::mat4 EditorCamera::GetViewMatrix() const
{
	const glm::vec3 position = GetPosition();

	return glm::lookAt(
		position,
		position + GetForward(),
		GetUp()
	);
}

glm::mat4 EditorCamera::GetProjection(float aspect) const
{
	return glm::perspective(glm::radians(45.0f), aspect, 0.1f, 500.0f);
}

glm::mat4 EditorCamera::GetViewProjection(float aspect) const
{
	return (GetProjection(aspect) * GetViewMatrix());
}

glm::vec3 EditorCamera::GetForward() const
{
	float  yaw = glm::radians(m_yaw);
	float pitch = glm::radians(m_pitch);

	glm::vec3 foward = glm::vec3(0.0);

	foward.x = cos(yaw) * cos(pitch);
	foward.y = sin(pitch);
	foward.z = sin(yaw) * cos(pitch);

	return glm::normalize(foward);
}

glm::vec3 EditorCamera::GetBackward() const
{
	glm::vec3 back = -GetForward();

	return back;
}

glm::vec3 EditorCamera::GetUp() const
{
	glm::vec3 worldUp = glm::vec3(0.0, 1.0, 0.0);
	glm::vec3 up = glm::cross(GetRight(), GetForward());
	

	return glm::normalize(up);
}

glm::vec3 EditorCamera::GetDown() const
{
	glm::vec3 down = -GetUp();

	return down;
}

glm::vec3 EditorCamera::GetRight() const
{
	glm::vec3 worldUp = glm::vec3(0.0, 1.0, 0.0);
	glm::vec3 right = glm::cross(GetForward(), worldUp);

	return glm::normalize(right);
}

glm::vec3 EditorCamera::GetLeft() const
{
	glm::vec3 left = -GetRight();

	return left;
}
