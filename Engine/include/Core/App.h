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
#include "PhysicsSystem.h"
#include "EngineTime.h"
#include "Viewport.h"
#include "RenderContext.h"
#include "Camera/ICamera.h"


class App
{
public:
	~App();
	void Run();
	
	const Scene* GetScene() const { return m_currentScene; }
	AssetManager& GetAssetManager() { return m_assetManager; }
	Mse::Time m_time;

	bool IsRunning();
	void Initialize();
	void StartFrame();
	void Update();
	void FixedStep();
	void Render();
	void EndFrame();
	void ShutDown();

	void SetViewport(unsigned int width, unsigned int height);

	GameObject* CreateCube();
	GameObject* CreatePlane();
	GameObject* CreateCamera();
	GameObject* CreateText();
	GameObject* CreateEmpty();

	GLFWwindow* GetNativeWindow() { return m_window.GetNativeWindow(); }
	void SetRenderCamera(ICamera* renderCamera) { m_renderCamera = renderCamera; };

	float GetDeltaTime() { return m_time.deltaTime; }

private:
	bool m_isRunning = true;
	float m_deltaTime = 0.0f;
	float m_accumulator = 0.0f;

	// Time

	Window m_window{ "Munita Source Engine", 1280, 720 };
	Renderer m_renderer;
	Scene m_defaultScene;
	Scene* m_currentScene;
	ICamera* m_renderCamera;
	AssetManager m_assetManager;
	PhysicsSystem m_physicsSytem;
	Viewport m_viewport = { 1280, 720 };

	GameObject* m_player = nullptr;
	GameObject* m_ground = nullptr;
	GameObject* m_mainCamera = nullptr;
	GameObject* m_paddle = nullptr;
	GameObject* m_ball = nullptr;
	GameObject* m_score = nullptr;

	SceneSerializer m_sceneSerializer;
	//EditorLayer m_editorLayer;

	bool m_SceneSaved = false;
	bool m_uniformTranformScale = true;
	bool m_uniformBoxColliderSize = true;

	// Assets
	Shader* m_litShader;
	Shader* m_textShader;

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
	void ProcessInput();
	void FixedUpdate(float dt);


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