#pragma once
#include "Window.h"
#include "Renderer.h"
#include "Scene.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "AmbienteLight.h"
#include "AssetManager.h"
#include "GameObject.h"
#include "SceneSerializer.h"
#include "Material.h"
#include "Mesh.h"

struct RenderContext;

class App
{
public:
	void Run();
	const Scene& GetScene() const { return m_scene; }

private:
	bool m_isRunning = true;

	Window m_window{ "Phoenix", 1280, 720 };
	Renderer m_renderer;
	Scene m_scene;
	Camera m_camera;
	AssetManager m_assetManager;
	GameObject* m_selectedObject = nullptr;
	SceneSerializer m_sceneSerializer;
	bool m_SceneSaved = false;

	// Assets
	Shader* m_litShader;
	Texture* m_defaultTexture;

	Material* m_cubeMaterial = nullptr; // Cube
	Mesh m_cubeMash;					// Cube

	Material* m_planeMaterial = nullptr; // Plane
	Mesh m_planeMash;					// Plane
	//Material

	void Init();
	void ProcessInput();
	void Update(float dt);
	void FixedUpdate();
	void Render();

	GameObject* CreateCube();
	GameObject* CreatePlane();

	GameObject* CreateObjectFromType(ObjectType type);

	void CreateDefaultScene();

	void LoadScene();
	void LoadDirectionalLightScene();
	void LoadAmbinetLightScene();

};