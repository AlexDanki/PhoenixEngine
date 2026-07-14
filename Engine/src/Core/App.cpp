#include "glad/glad.h"
#include "App.h"
#include <iostream>
#include <chrono>
#include "Mesh.h"
#include "GameObject.h"
#include "MeshComponent.h"
#include "BoxCollider.h" 
#include "memory"
#include "RenderContext.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glm/gtc/type_ptr.inl>
#include "MeshAsset.h"
#include "RigidbodyComponent.h"
#include "PaddleController.h"
#include "BallController.h"
#include "RotatorScript.h"
#include "EnemyPaddleController.h"
#include "WorldTextRenderComponent.h"
#include "GameManager.h"
#include "Input.h"
#include "Log.h"
#include "ScriptRegistry.h"
#include "FontLibrary.h"
#include "Font.h"

namespace ui = ImGui;

void InterfaceImGui();

App::~App()
{
	delete(m_cubeMaterial);
	delete(m_cameraMaterial);
	delete(m_planeMaterial);
	delete(m_paddleMaterial);
	delete(m_ballMaterial);
}

void App::Initialize()
{
	m_currentScene = &m_defaultScene;
	ScriptRegistry::Register("PaddleController", []() { return std::make_unique<PaddleController>(); });
	ScriptRegistry::Register("TransformRotator", []() { return std::make_unique<RotatorScript>(); });
	ScriptRegistry::Register("BallController", []() { return std::make_unique<BallController>(); });
	ScriptRegistry::Register("GameManager", []() { return std::make_unique<GameManager>(); });
	ScriptRegistry::Register("EnemyPaddleController", []() { return std::make_unique<EnemyPaddleController>(); });
	//OBJLoader::Loader("Models/Et.obj", glm::vec3(0,0,0), glm::vec3(0, 0, 0));

	std::cout << "Engine Started" << std::endl;

	// Inicializa funções do OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return;
	}

	// Inicializa o Render
	m_renderer.Init();

	//m_editorLayer.Inity(m_window.GetNativeWindow());

	Input::Init(m_window.GetNativeWindow());

	FontLibrary::Init();

	// Cria Shader
	m_litShader = m_assetManager.LoadShader("Shaders/Lit.vert", "Shaders/Lit.frag");
	Shader* lineShader = m_assetManager.LoadShader("Shaders/Line.vert", "Shaders/Line.frag");
	m_renderer.SetLineShader(lineShader);

	m_textShader = m_assetManager.LoadShader("Shaders/TextVertex.vert", "Shaders/TextFragment.frag");

	// Configura Material 01
	Texture* groundTex = m_assetManager.LoadTexture("Textures/GroundTex.png");
	if (!groundTex)
	{
		std::cout << "Erro ao carregar textura: GrountTex" << std::endl;
	}

	// Configura Material 02
	m_paddleTexture = m_assetManager.LoadTexture("Textures/PaddleTex.png");
	if (!m_paddleTexture)
	{
		std::cout << "Erro ao carregar textura: AreiaTex" << std::endl;
	}


	// Mesh
	m_planeMesh.CreatePlane();
	m_cubeMesh.CreateCube();


	// AssetMesh 
	m_paddleAsset = m_assetManager.LoadMeshAsset("Models/Paddle.fbx");

	// Material
	//Texture* cubeTexture = m_assetManager.LoadTexture("Textures/GroundTex.png");
	Material* cubeMaterial = new Material(m_litShader);
	cubeMaterial->SetTexture(groundTex);
	cubeMaterial->SetMaterialColor(glm::vec3(1, 1, 1));
	m_cubeMaterial = cubeMaterial;

	Material* planeMaterial = new Material(m_litShader);
	planeMaterial->SetMaterialColor(glm::vec3(1, 1, 1));
	planeMaterial->SetTexture(groundTex);
	m_planeMaterial = planeMaterial;

	Material* cameraMaterial = new Material(m_litShader);
	cameraMaterial->SetTexture(m_cameraTexture);
	cameraMaterial->SetMaterialColor({ 1, 1, 1 });
	m_cameraMaterial = cameraMaterial;

	Material* paddleMaterial = new Material(m_litShader);
	paddleMaterial->SetTexture(m_paddleTexture);
	paddleMaterial->SetMaterialColor({ 1, 1, 1 });
	m_paddleMaterial = paddleMaterial;

	Material* ballMaterial = new Material(m_litShader);
	ballMaterial->SetTexture(m_paddleTexture);
	ballMaterial->SetMaterialColor({ 1, 1, 1 });
	m_ballMaterial = ballMaterial;

	// Verifica se não existe Cena salva.
	if (!m_sceneSerializer.SceneFileExists("Scene.txt"))
	{
		// Cria uma cena padrão da Engine
		CreateDefaultScene();
	}
	else
	{
		// Se existir cena salva carregar todos os recusos dela
		LoadScene();
		LoadDirectionalLightScene();
		LoadAmbinetLightScene();
	}

	m_currentScene->Start();
}

void App::Run()
{

	

}

void App::StartFrame()
{
	m_time.StartFrame();
	m_window.PoolEvents();
}

void App::EndFrame()
{
	

	m_time.EndFrame();
	m_window.SwapBuffers();
}

void InterfaceImGui()
{
	//ImGui::ShowDemoWindow();
}

void App::ProcessInput()
{
	// Futuramente janela + inputs
	GLFWwindow* window = m_window.GetNativeWindow();
}

void App::Update()
{
	m_currentScene->Update(m_time.deltaTime);
	m_physicsSytem.Update(m_currentScene);
}

void App::FixedStep()
{
	// Loop Fixo
	while (m_time.accumulator >= m_time.fixedDeltaTime)
	{
		FixedUpdate(m_time.fixedDeltaTime);
		m_time.accumulator -= m_time.fixedDeltaTime;
	}
}

void App::FixedUpdate(float dt)
{
	m_currentScene->FixedUpdate(dt);
}

void App::Render()
{
	float aspect = static_cast<float>(m_window.GetWidth()) / static_cast<float>(m_window.GetHeight());


	RenderContext renderContext{
		*m_currentScene,
		m_camera,
		aspect,
		m_textShader,
		m_window.GetWidth(),
		m_window.GetHeight()
	};

	m_renderer.Clear();

	m_renderer.DrawScene(renderContext);

	/*		if(m_editorLayer.GetSelectedObject())
			{
				glm::vec3 start = glm::vec3(m_editorLayer.GetSelectedObject()->GetTransform().position.x,
					m_editorLayer.GetSelectedObject()->GetTransform().position.y,
					m_editorLayer.GetSelectedObject()->GetTransform().position.z);

				glm::vec3 end = glm::vec3(m_editorLayer.GetSelectedObject()->GetTransform().position.x,
					m_editorLayer.GetSelectedObject()->GetTransform().position.y + 3,
					m_editorLayer.GetSelectedObject()->GetTransform().position.z);

				//m_renderer.DrawLine(start, end, glm::vec3(0.0, 1.0, 0.0), m_camera.GetViewProjectionMatrix(aspect));
				m_renderer.DrawTransformGizmos(*m_editorLayer.GetSelectedObject(), renderContext, aspect);
				if(m_editorLayer.GetSelectedObject()->GetComponent<BoxCollider>())
				{
					m_renderer.DrawBoxCollider(*m_editorLayer.GetSelectedObject()->GetComponent<BoxCollider>(), m_currentScene->GetPrimaryCamera()->GetViewProjection(aspect));

				}
			}*/

			// Draw World Grid
	const int quantLines = 500;
	m_renderer.DrawWorldGrid(quantLines, m_currentScene->GetPrimaryCamera()->GetViewProjection(aspect));

	
}

void App::ShutDown()
{
	FontLibrary::ShutDown();
}

GameObject* App::CreateGameObject(std::string name, ObjectType type,  const char* assetPath, const char* texturePath, Material& material)
{
	MeshAsset* meshAsset = m_assetManager.LoadMeshAsset(assetPath);

	std::string objectName = m_currentScene->GenerateUniqueName(name);
	GameObject& object = m_currentScene->CreateGameObject(name, type);

	Material& _material = material;

	object.SetTexturePath(texturePath);

	_material.SetTexture(m_assetManager.LoadTexture(texturePath));

	object.AddComponent(std::make_unique<MeshComponent>(meshAsset, &_material));
	

	if(type == ObjectType::Camera)
	{
		object.AddComponent(std::make_unique<CameraComponent>());
		object.GetTransform().position.y = 4.3;
		object.GetTransform().position.z = 23;
	}

	if(object.GetType() == ObjectType::Asset)
	{
		object.SetAssetPath(assetPath);
	}

	return &object;
}

GameObject* App::CreateAssetObject(std::string& assetPath, std::string& texturePath)
{
	MeshAsset* asset = m_assetManager.LoadMeshAsset(assetPath);

	GameObject* object = &m_currentScene->CreateGameObject("Asset", ObjectType::Asset);
	Material* material = m_assetManager.CreateMaterial(texturePath, m_litShader);
	object->AddComponent(std::make_unique<MeshComponent>(asset, material));
	object->SetAssetPath(assetPath.c_str());
	return object;
}

GameObject* App::CreateCube()
{

	std::string name = m_currentScene->GenerateUniqueName("Cube");
	GameObject& cube = m_currentScene->CreateGameObject(name, ObjectType::Cube);
	cube.AddComponent(std::make_unique<MeshComponent>(&m_cubeMesh, m_cubeMaterial));
	m_cubeMaterial->SetTexture(m_assetManager.LoadTexture("Textures/DefaultTex.png"));
	cube.GetTransform().position.z = 0;

	//m_player = &cube;
	return &cube;
}

GameObject* App::CreatePlane()
{
	
	// Cria objeto 02
	std::string name = m_currentScene->GenerateUniqueName("Plane");
	GameObject& plane = m_currentScene->CreateGameObject(name, ObjectType::Plane);
	plane.AddComponent(std::make_unique<MeshComponent>(&m_planeMesh, m_planeMaterial));
	//plane.AddComponent(std::make_unique<BoxCollider>());

	//m_ground = &plane;
	return &plane;
}

GameObject* App::CreateCamera()
{

	std::string name = m_currentScene->GenerateUniqueName("Camera");
	GameObject& camera = m_currentScene->CreateGameObject(name, ObjectType::Camera);
	//camera.AddComponent(std::make_unique<MeshComponent>(&m_cubeMash, m_cameraMaterial));
	camera.AddComponent(std::make_unique<CameraComponent>());
	camera.GetTransform().position.y = 3;
	camera.GetTransform().position.z = 20;

	m_mainCamera = &camera;

	return &camera;
}

GameObject* App::CreateText()
{
	std::string name = m_currentScene->GenerateUniqueName("WorldText");
	GameObject& text = m_currentScene->CreateGameObject(name, ObjectType::Text);
	text.AddComponent(std::make_unique<WorldTextRenderComponent>());
	WorldTextRenderComponent* tr = text.GetComponent<WorldTextRenderComponent>();
	Font* font = m_assetManager.LoadFont("Fonts/TheCat.ttf");
	tr->SetFont(font);
	return &text;
}

GameObject* App::CreateEmpty()
{
	std::string name = m_currentScene->GenerateUniqueName("Empty");
	GameObject& empty = m_currentScene->CreateGameObject(name, ObjectType::Empty);
	return &empty;
}

GameObject* App::CreateObjectFromData(SceneObjectData data)
{
	GameObject* go = nullptr;

	switch(data.type)
	{
		
		case ObjectType::Cube:
			go =  CreateCube();
			break;
		case ObjectType::Plane:
			go = CreatePlane();
			break;
		case ObjectType::Text:
			go = CreateText();
			break;
		case ObjectType::Empty:
			go = CreateEmpty();
			break;
		case ObjectType::Camera:
			go = CreateCamera();
			break;
		case ObjectType::Asset:
			go = CreateAssetObject(data.assetPath, data.texturePath);
			break;
	}

	if (go) return go;

	return nullptr;

}

void App::LoadScene()
{
	if(m_sceneSerializer.SceneFileExists("Scene.txt"))
	{
		std::vector<SceneObjectData> sceneDataObjects;
		sceneDataObjects = m_sceneSerializer.LoadAllGameObjectsData("Scene.txt");

	for(const auto& data : sceneDataObjects)
		{
			GameObject* object = CreateObjectFromData(data);
			object->SetName(data.name);
			std::cout << "Object Name: " << data.name << std::endl;
			object->SetTag(data.tag);
			object->SetTexturePath(data.texturePath.c_str());
			//if (m_editorLayer.GetSelectedObject() == nullptr) { m_editorLayer.SetSelectedObject(object); }

			Transform& transform = object->GetTransform();
			transform.position.x = data.position.x;
			transform.position.y = data.position.y;
			transform.position.z = data.position.z;

			transform.rotation.x = data.rotation.x;
			transform.rotation.y = data.rotation.y;
			transform.rotation.z = data.rotation.z;

			transform.scale.x = data.scale.x;
			transform.scale.y = data.scale.y;
			transform.scale.z = data.scale.z;

			if(data.hasMeshComponent)
			{
				MeshComponent* meshComponent = object->GetComponent<MeshComponent>();
				Material* material = meshComponent->GetMaterial();
				material->GetMaterialColor().x = data.color.x;
				material->GetMaterialColor().y = data.color.y;
				material->GetMaterialColor().z = data.color.z;
			}
			
			// Adiciona BoxCollider
			if(data.hasBoxCollider)
			{
				object->AddComponent(std::make_unique<BoxCollider>());
				BoxCollider* boxCollider = object->GetComponent<BoxCollider>();

				boxCollider->isTrigger = data.isTrigger;

				std::cout << "XSizeBox do :" << object->GetName() << " é  - " << data.boxSize.x << std::endl;
				std::cout << "YSizeBox do :" << object->GetName() << " é  - " << data.boxSize.y << std::endl;
				std::cout << "ZSizeBox do :" << object->GetName() << " é  - " << data.boxSize.z << std::endl;

				boxCollider->SetCenter(data.boxCenter);
				boxCollider->SetSize(data.boxSize);
				

			}

			// Adiciona Rigidbody
			if(data.hasRigidbody)
			{
				object->AddComponent(std::make_unique<RigidbodyComponent>());
				RigidbodyComponent* rigidBody = object->GetComponent<RigidbodyComponent>();
				rigidBody->gravityScale = data.gravitScale;
				std::cout << "GravitScale: " << data.gravitScale << std::endl;
				rigidBody->useGravity = data.useGravit;
			}

			// Adiciona WorldTextRenderComponent
			if (auto worldText = object->GetComponent<WorldTextRenderComponent>())
			{
				
				worldText->SetText(data.worldText);
				worldText->SetScale(data.textScale);
			}

			// Adiciona ScriptComponentes
			for (auto scriptName : data.scriptsNames)
			{
				std::cout << "Script Name: " << scriptName << std::endl;
				object->SetScriptComponentName(scriptName);
				object->AddComponent(ScriptRegistry::Create(scriptName));
			}
		}
	}
}

void App::LoadDirectionalLightScene()
{
	DirectionalLightdata data;
	data = m_sceneSerializer.LoadDirectionalLightData("Scene.txt");

	m_defaultScene.GetDirectionalLight().Direction.x = data.direction.x;
	m_defaultScene.GetDirectionalLight().Direction.y = data.direction.y;
	m_defaultScene.GetDirectionalLight().Direction.z = data.direction.z;

	m_defaultScene.GetDirectionalLight().Color.x = data.color.x;
	m_defaultScene.GetDirectionalLight().Color.y = data.color.y;
	m_defaultScene.GetDirectionalLight().Color.z = data.color.z;

	m_defaultScene.GetDirectionalLight().Intensity = data.intensity;

}
void App::LoadAmbinetLightScene()
{

	AmbineteLightData data;
	data = m_sceneSerializer.LoadAmbienteLightdata("Scene.txt");

	m_defaultScene.GetAmbienteLight().Color.x = data.color.x;
	m_defaultScene.GetAmbienteLight().Color.y = data.color.y;
	m_defaultScene.GetAmbienteLight().Color.z = data.color.z;

	m_defaultScene.GetAmbienteLight().Intensity = data.intensity;

}

void App::CreateDefaultScene()
{
	m_player = CreateEmpty();
	//m_editorLayer.SetSelectedObject(m_player);
	m_mainCamera = CreateCamera();
	//m_mainCamera->AddComponent(std::make_unique<RotatorScript>());
	//m_mainCamera = CreateGameObject("Main_Cam", ObjectType::Camera, "Models/Paddle.fbx" , "Textures/PaddleTex.png", *m_paddleMaterial);
	//m_editorLayer.SetSelectedObject(m_mainCamera);
	//CreateCube();
	//CreateCube();
	//CreateCube();
	//CreateCube();
	CreatePlane();
	CreateGameObject("Paddle", ObjectType::Asset, "Models/Paddle.fbx", "Textures/PaddleTex.png", *m_paddleMaterial);
	//CreateGameObject("EnemyPaddle", ObjectType::Asset, "Models/Paddle.fbx", "Textures/PaddleTex.png", *m_paddleMaterial);
	//CreateGameObject("Ball", ObjectType::Asset, "Models/Ball.fbx", "Textures/BallTex.png", *m_ballMaterial);
}

Material* App::CreateMaterial(std::string& texturePath)
{
	Material* material = new Material(m_litShader);
	material->SetTexture(m_assetManager.LoadTexture(texturePath));
	material->SetMaterialColor(glm::vec3(1.0f));
	return material;
}

bool App::IsRunning()
{
	return (m_isRunning && !m_window.ShouldClose());
}