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

void App::Run()
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

	m_editorLayer.Inity(m_window.GetNativeWindow());

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
	m_cameraTexture = m_assetManager.LoadTexture(" ");
	if (!m_cameraTexture)
	{
		std::cout << "Erro ao carregar textura: AreiaTex" << std::endl;
	}

	if (!m_defaultTexture)
	{
		std::cout << "Erro ao carregar textura: AreiaTex" << std::endl;
	}
	m_paddleTexture = m_assetManager.LoadTexture("Textures/PaddleTex.png");
	if (!m_paddleTexture)
	{
		std::cout << "Erro ao carregar textura: AreiaTex" << std::endl;
	}

	
	// Mesh
	m_planeMesh.CreatePlane();
	m_cubeMesh.CreateCube();


	// AssetMesh 
	m_etAsset = m_assetManager.LoadMeshAsset("Models/Et.obj");
	m_paddleAsset = m_assetManager.LoadMeshAsset("Models/Paddle.fbx");

	// Material
	//Texture* cubeTexture = m_assetManager.LoadTexture("Textures/GroundTex.png");
	Material cubeMaterial(m_litShader);
	cubeMaterial.SetTexture(groundTex);
	cubeMaterial.SetMaterialColor(glm::vec3(1, 1, 1));
	m_cubeMaterial = &cubeMaterial;

	Material planeMaterial(m_litShader);
	planeMaterial.SetMaterialColor(glm::vec3(1, 1, 1));
	planeMaterial.SetTexture(groundTex);
	m_planeMaterial = &planeMaterial;

	Material cameraMaterial(m_litShader);
	cameraMaterial.SetTexture(m_cameraTexture);
	cameraMaterial.SetMaterialColor({ 1, 1, 1 });
	m_cameraMaterial = &cameraMaterial;

	Material paddleMaterial(m_litShader);
	paddleMaterial.SetTexture(m_paddleTexture);
	paddleMaterial.SetMaterialColor({ 1, 1, 1 });
	m_paddleMaterial = &paddleMaterial;

	Material ballMaterial(m_litShader);
	ballMaterial.SetTexture(m_paddleTexture);
	ballMaterial.SetMaterialColor({ 1, 1, 1 });
	m_ballMaterial = &ballMaterial;

	// Posiciona Camera
	m_camera.GetTransform().position.x = 0.0f;
	m_camera.GetTransform().position.z = 10.0f;
	m_camera.GetTransform().position.y = 3.0f;

	// Configura Timer
	using Clock = std::chrono::high_resolution_clock;
	using TimePoint = std::chrono::time_point<Clock>;

	float deltaTime = 0.f; // Simulate a fixed time step (60 FPS)
	float accumulator = 0.f;

	float fixedDeltaTime = 1.0f / 60.f; // 60 FPS

	TimePoint lastTime = Clock::now();
	
	// Configura aspect
	float aspect = static_cast<float>(m_window.GetWidth()) / static_cast<float>(m_window.GetHeight());


	RenderContext renderContext{
		*m_currentScene,
		m_camera,
		aspect,
		m_textShader,
		m_window.GetWidth(),
		m_window.GetHeight()
	};

	// Verifica se não existe Cena salva.
	if(!m_sceneSerializer.SceneFileExists("Scene.txt"))
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

	float saveMessageTimer = 0.0f;
	
	m_editorLayer.SetScene(m_currentScene);
	m_editorLayer.SetApp(this);
	m_editorLayer.SetSceneScerializer(&m_sceneSerializer);

	m_currentScene->Start();
	// Loop Princial
	while (m_isRunning && !m_window.ShouldClose())
	{
		if (deltaTime > 0.05f) { deltaTime = 0.05f; }
		m_window.PoolEvents();
		TimePoint currentTime = Clock::now();
		std::chrono::duration<float> elapsed = currentTime - lastTime;

		ProcessInput();
		deltaTime = elapsed.count();
		accumulator += deltaTime;

		// Loop Fixo
		while (accumulator >= fixedDeltaTime)
		{
			FixedUpdate(fixedDeltaTime);
			accumulator -= fixedDeltaTime;
		}

		// Game Update
		Update(deltaTime);

		// Renderiza
		m_renderer.Clear();
		
		// Alterei

		// Começo do frame ImGui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		float dammySpacing = 5.0;
		// Widgets
		//ImGui::ShowDemoWindow();

		m_editorLayer.Draw(deltaTime);

		m_renderer.DrawScene(renderContext);

		if(m_editorLayer.GetSelectedObject())
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
		}
		
		// Final do frame ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(
			ImGui::GetDrawData()
		);

		m_window.SwapBuffers();

		lastTime = currentTime;
	}
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

void App::Update(float dt)
{
	m_currentScene->Update(dt);
	m_physicsSytem.Update(m_currentScene);
}

void App::FixedUpdate(float dt)
{
	m_currentScene->FixedUpdate(dt);
}

void App::Render()
{

}

void App::ShutDown()
{
	FontLibrary::ShutDown();
}

GameObject* App::CreateGameObject(std::string name, ObjectType type,  const char* assetPath, const char* texturePath, Material& material)
{
	Mesh* mesh = &m_assetManager.LoadMeshAsset(assetPath)->GetMesh();

	std::string objectName = m_currentScene->GenerateUniqueName(name);
	GameObject& object = m_currentScene->CreateGameObject(name, type);

	Material& _material = material;

	object.SetTexturePath(texturePath);

	_material.SetTexture(m_assetManager.LoadTexture(texturePath));

	object.AddComponent(std::make_unique<MeshComponent>(mesh, &_material));
	

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
	plane.AddComponent(std::make_unique<BoxCollider>());

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
	std::string name = m_currentScene->GenerateUniqueName("Text");
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
	switch(data.type)
	{
		case ObjectType::Cube:
			return CreateCube();

		case ObjectType::Plane:
			return CreatePlane();

		case ObjectType::Text:
			return CreateText();

		case ObjectType::Empty:
			return CreateEmpty();

		case ObjectType::Camera:
			return CreateGameObject("Camera", ObjectType::Camera,
				"Models/Paddle.fbx", "Textures/PaddleTex.png", *m_paddleMaterial);

		case ObjectType::Asset:
			return CreateAssetObject(data.assetPath, data.texturePath);

		default:
			return nullptr;
	}

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
			object->SetTag(data.tag);
			object->SetTexturePath(data.texturePath.c_str());
			if (m_editorLayer.GetSelectedObject() == nullptr) { m_editorLayer.SetSelectedObject(object); }

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

			if(auto meshComponent = object->GetComponent<MeshComponent>())
			{
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

				boxCollider->SetCenter(data.boxCenter);
				boxCollider->SetSize(data.boxSize);
				

			}

			// Adiciona Rigidbody
			if(data.hasRigidbody)
			{
				object->AddComponent(std::make_unique<RigidbodyComponent>());
				RigidbodyComponent* rigidBody = object->GetComponent<RigidbodyComponent>();
				rigidBody->gravityScale = data.gravitScale;
				rigidBody->useGravity = data.useGravit;
				rigidBody->velocity.x = data.rigidbodyVelocity.x;
				rigidBody->velocity.y = data.rigidbodyVelocity.y;
				rigidBody->velocity.z = data.rigidbodyVelocity.z;
			}

			// Adiciona WorldTextRenderComponent
			if (data.hasWorldText)
			{
				object->AddComponent(std::make_unique<WorldTextRenderComponent>());
				auto worldText = object->GetComponent<WorldTextRenderComponent>();
				worldText->SetText(data.worldText);
				worldText->SetScale(data.textScale);
			}

			// Adiciona ScriptComponentes
			for (auto scriptName : data.scriptsNames)
			{
				Log::Info(scriptName);
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
	m_mainCamera = CreateGameObject("Main_Cam", ObjectType::Camera, "Models/Paddle.fbx" , "Textures/PaddleTex.png", *m_paddleMaterial);
	m_editorLayer.SetSelectedObject(m_mainCamera);
	CreateCube();
	CreateCube();
	CreateCube();
	CreateCube();
	CreatePlane();
	CreateGameObject("Paddle", ObjectType::Asset, "Models/Paddle.fbx", "Textures/PaddleTex.png", *m_paddleMaterial);
	CreateGameObject("EnemyPaddle", ObjectType::Asset, "Models/Paddle.fbx", "Textures/PaddleTex.png", *m_paddleMaterial);
	CreateGameObject("Ball", ObjectType::Asset, "Models/Ball.fbx", "Textures/BallTex.png", *m_ballMaterial);
}

Material* App::CreateMaterial(std::string& texturePath)
{
	Material* material = new Material(m_litShader);
	material->SetTexture(m_assetManager.LoadTexture(texturePath));
	material->SetMaterialColor(glm::vec3(1.0f));
	return material;
}