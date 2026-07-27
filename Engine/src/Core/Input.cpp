#include "Input.h"

GLFWwindow* Input::s_window = nullptr;

void Input::Init(GLFWwindow* window)
{
	s_window = window;
}

bool Input::GetKey(KeyCode key)
{
	if(!s_window)
	{
		return false;
	}

	return (glfwGetKey(s_window, static_cast<int>(key)) == GLFW_PRESS);

}

bool Input::GetMouseButton(MouseButton mouseButton)
{
	if(!s_window)
	{
		return false;
	}

	return (glfwGetMouseButton(s_window, static_cast<int>(mouseButton)) == GLFW_PRESS);

}

glm::vec2 Input::GetMousePos()
{
	if (!s_window)
	{
		return glm::vec2(0.0f);
	}

	double x = 0;
	double y = 0;
	glfwGetCursorPos(s_window, &x, &y);

	return glm::vec2(static_cast<float>(x), static_cast<float>(y));
}