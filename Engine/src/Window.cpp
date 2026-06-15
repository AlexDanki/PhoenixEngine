#include "Window.h"
#include <iostream>

Window::Window(const std::string& title, int width, int height):
	m_title(title), m_width(width), m_height(height)
{
	if(!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
	}

	//glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

	// Centralizar
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	glfwSetWindowPos(m_window, (mode->width - width) / 2, (mode->height - height) / 2);

	if(!m_window)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_window);

	std::cout << "Window Created Title" << "width"<< width << std::endl;
}

Window::~Window()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
	std::cout << "Window Destroyed Title" << m_title << std::endl;
}

void Window::PoolEvents()
{
	glfwPollEvents();
	// Futuramente Glfw / SDL / WinAPI
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(m_window);
}

bool Window::ShouldClose() const
{
	return glfwWindowShouldClose(m_window);
}

int Window::GetWidth() const
{
	return m_width;
}
int Window::GetHeight() const
{
	return m_height;
}