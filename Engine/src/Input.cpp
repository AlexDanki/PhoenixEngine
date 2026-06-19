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