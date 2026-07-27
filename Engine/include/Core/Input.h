#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

enum class KeyCode
{
	W = GLFW_KEY_W,
	A = GLFW_KEY_A,
	S = GLFW_KEY_S,
	D = GLFW_KEY_D,
	Q = GLFW_KEY_Q,
	E = GLFW_KEY_E,

	SPACE = GLFW_KEY_SPACE,
	ENTER = GLFW_KEY_ENTER,
	ESC = GLFW_KEY_ESCAPE,

	UP = GLFW_KEY_UP,
	DOWN = GLFW_KEY_DOWN,
	LEFT = GLFW_KEY_LEFT,
	RIGHT = GLFW_KEY_RIGHT,
};

enum class MouseButton
{
	LEFT = 0,
	RIGHT = 1,
	MIDDLE = 2,
};

class Input
{
public:
	static void Init(GLFWwindow* window);
	static bool GetKey(KeyCode key);
	static bool GetMouseButton(MouseButton mouseButton);
	static glm::vec2 GetMousePos();

private:
	static GLFWwindow* s_window;
};