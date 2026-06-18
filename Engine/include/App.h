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
#include "MeshAsset.h"
#include "BoxCollider.h"
#include "EditorLayer.h"

struct RenderContext;

class App
{
public:
	void Run();
	const Scene& GetScene() const { return m_scene; }
	
	GameObject* CreateCube();
	GameObject* CreatePlane();

private:
	bool m_isRunning = true;

	Window m_window{ "Phoenix", 1280, 720 };
	Renderer m_renderer;
	Scene m_scene;
	Camera m_camera;
	AssetManager m_assetManager;
	GameObject* m_player = nullptr;
	GameObject* m_ground= nullptr;
	SceneSerializer m_sceneSerializer;
	EditorLayer m_editorLayer;

	bool m_SceneSaved = false;
	bool m_uniformTranformScale = true;
	bool m_uniformBoxColliderSize = true;

	// Assets
	Shader* m_litShader;
	Texture* m_defaultTexture;

	Material* m_cubeMaterial = nullptr; // Cube
	MeshAsset* m_etAsset = nullptr;

	Material* m_planeMaterial = nullptr; // Plane
	Mesh m_planeMash;	
	Mesh m_cubeMash;	
	//Material

	void Init();
	void ProcessInput();
	void Update(float dt);
	void FixedUpdate(float dt);
	void Render();

	GameObject* CreateObjectFromType(ObjectType type);

	void CreateDefaultScene();

	void LoadScene();
	void LoadDirectionalLightScene();
	void LoadAmbinetLightScene();

};