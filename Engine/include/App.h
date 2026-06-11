#pragma once
#include "Window.h"
#include "Renderer.h"
#include "Scene.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "AmbienteLight.h"

class App
{
public:
	void Run();

private:
	bool m_isRunning = true;

	Window m_window{ "Phoenix", 1280, 720 };
	Renderer m_renderer;
	Scene m_scene;
	Camera m_camera;
	DirectionalLight m_directionalLight;
	AmbienteLight m_ambienteLight;

	void ProcessInput();
	void Update(float dt);
	void FixedUpdate();
	void Render();
};