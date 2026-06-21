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
	GameObject* CreateCamera();

private:
	bool m_isRunning = true;

	Window m_window{ "Phoenix", 1280, 720 };
	Renderer m_renderer;
	Scene m_scene;
	Camera m_camera;
	AssetManager m_assetManager;

	GameObject* m_player = nullptr;
	GameObject* m_ground= nullptr;
	GameObject* m_mainCamera= nullptr;
	GameObject* m_paddle= nullptr;

	SceneSerializer m_sceneSerializer;
	EditorLayer m_editorLayer;

	bool m_SceneSaved = false;
	bool m_uniformTranformScale = true;
	bool m_uniformBoxColliderSize = true;

	// Assets
	Shader* m_litShader;

	Texture* m_defaultTexture;
	Texture* m_cameraTexture;
	Texture* m_paddleTexture;
	Texture* m_ballTexture;

	Material* m_cubeMaterial = nullptr; // Cube
	Material* m_cameraMaterial = nullptr; // Cube
	Material* m_planeMaterial = nullptr; // Plane
	Material* m_paddleMaterial = nullptr; // Paddle
	Material* m_ballMaterial = nullptr; // Ball

	MeshAsset* m_etAsset = nullptr;
	MeshAsset* m_paddleAsset = nullptr;
	Mesh m_planeMesh;	
	Mesh m_cubeMesh;	
	
	//Material

	void Init();
	void ProcessInput();
	void Update(float dt);
	void FixedUpdate(float dt);
	void Render();

	//GameObject* CreateObjectFromType(ObjectType type);
	GameObject* CreateObjectFromData(SceneObjectData data);
	GameObject* CreateGameObject(std::string name, ObjectType type, const char* assetPath, const char* texturePath, Material& material);
	GameObject* CreateAssetObject(std::string& assetPath, std::string& texturePath);
	Material* CreateMaterial(std::string& texturePath);
	void CreateDefaultScene();

	void LoadScene();
	void LoadDirectionalLightScene();
	void LoadAmbinetLightScene();

};