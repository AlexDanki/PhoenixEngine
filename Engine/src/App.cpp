#include "glad/glad.h"
#include "App.h"
#include <iostream>
#include <chrono>
#include "Mesh.h"
#include "GameObject.h"
#include "MeshComponent.h"
#include "memory"

void App::Run()
{

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return;
	}

	m_renderer.Init();

	Mesh triangleMesh;
	triangleMesh.CreateCube();

	m_camera.GetTransform().position.x = 0.0f;
	m_camera.GetTransform().position.z = 10.0f;
	m_camera.GetTransform().position.y = 3.0f;

	GameObject& gameObject = m_scene.CreateGameObject("GameObject2");
	gameObject.AddComponent(std::make_unique<MeshComponent>(&triangleMesh));
	gameObject.GetTransform().position.z = -3.0f;
	gameObject.GetTransform().scale.x = 1.2f;
	gameObject.GetTransform().scale.y = 1.2f;
	gameObject.GetTransform().rotation.y = 60.0f;

	GameObject& gameObject2 = m_scene.CreateGameObject("GameObject2");
	gameObject2.AddComponent(std::make_unique<MeshComponent>(&triangleMesh));
	gameObject2.GetTransform().position.z = -1.5f;
	gameObject2.GetTransform().position.x = 1.35f;

	GameObject& gameObject3 = m_scene.CreateGameObject("GameObject3");
	gameObject3.AddComponent(std::make_unique<MeshComponent>(&triangleMesh));
	gameObject3.GetTransform().position.z = -1.0f;
	gameObject3.GetTransform().position.x = 2.35f;
	gameObject3.GetTransform().position.y= 1.5f;

	std::cout << "Engine Started" << std::endl;

	using Clock = std::chrono::high_resolution_clock;
	using TimePoint = std::chrono::time_point<Clock>;

	float deltaTime = 0.f; // Simulate a fixed time step (60 FPS)
	float accumulator = 0.f;

	float fixedDeltaTime = 1.0f / 60.f; // 60 FPS

	TimePoint lastTime = Clock::now();
	
	
	float aspect = static_cast<float>(m_window.GetWidth()) / static_cast<float>(m_window.GetHeight());

	while (m_isRunning && !m_window.ShouldClose())
	{
		m_window.PoolEvents();
		TimePoint currentTime = Clock::now();
		std::chrono::duration<float> elapsed = currentTime - lastTime;

		ProcessInput();
		deltaTime = elapsed.count();
		accumulator += deltaTime;
		
		while (accumulator >= fixedDeltaTime)
		{
			FixedUpdate();
			accumulator -= fixedDeltaTime;
		}

		Update(deltaTime);

		m_renderer.Clear();
		m_renderer.DrawScene(m_scene, m_camera, m_directionalLight, m_ambienteLight, aspect);

		m_window.SwapBuffers();

		lastTime = currentTime;
	}
}

void App::ProcessInput()
{
	// Futuramente janela + inputs
	GLFWwindow* window = m_window.GetNativeWindow();

	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		m_camera.GetTransform().position.z -= 0.05f;
	}

	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		m_camera.GetTransform().position.z += 0.05f;
	}

	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		m_camera.GetTransform().position.x -= 0.05f;
	}

	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		m_camera.GetTransform().position.x += 0.05f;
	}

	if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		m_camera.GetTransform().position.y -= 0.05f;
	}

	if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		m_camera.GetTransform().position.y += 0.05f;
	}
}

void App::Update(float dt)
{

}

void App::FixedUpdate()
{

}

void App::Render()
{

}