#pragma once
#include <string>
#include "GLFW/glfw3.h"

class Window {
public:
	Window(const std::string& title, int width, int height);
	~Window();
	void PoolEvents();
	void SwapBuffers();
	bool ShouldClose() const;

	int GetWidth() const;
	int GetHeight() const;

	GLFWwindow* GetNativeWindow() const { return m_window; }

private:

	GLFWwindow* m_window = nullptr;

	std::string m_title;
	int m_width;
	int m_height;
	bool m_shouldClose = false;

};