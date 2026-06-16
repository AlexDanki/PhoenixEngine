#include "glad/glad.h"
#include "App.h"
#include <iostream>
#include <chrono>
#include "Mesh.h"
#include "GameObject.h"
#include "MeshComponent.h"
#include "memory"
#include "RenderContext.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glm/gtc/type_ptr.inl>
#include "OBJLoader.h"

namespace ui = ImGui;

void InterfaceImGui();

void App::Run()
{

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

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	GLFWwindow* window = m_window.GetNativeWindow();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// Cria Shader
	m_litShader = m_assetManager.LoadShader("Shaders/Lit.vert", "Shaders/Lit.frag");
	Shader* lineShader = m_assetManager.LoadShader("Shaders/Line.vert", "Shaders/Line.frag");
	m_renderer.SetLineShader(lineShader);

	// Configura Material 01
	Texture* groundTex = m_assetManager.LoadTexture("Textures/GroundTex.png");
	if (!groundTex)
	{
		std::cout << "Erro ao carregar textura: GrountTex" << std::endl;
	}
	

	// Configura Material 02
	m_defaultTexture = m_assetManager.LoadTexture("Textures/ETImg.png");
	if (!m_defaultTexture)
	{
		std::cout << "Erro ao carregar textura: AreiaTex" << std::endl;
	}

	
	// Mesh
	m_planeMash.CreatePlane();
	// Material
	Material planeMaterial(m_litShader);
	planeMaterial.SetMaterialColor(glm::vec3(1, 1, 1));
	planeMaterial.SetTexture(groundTex);
	m_planeMaterial = &planeMaterial;

	
	// Mesh
	//m_cubeMash.CreateCube();
	m_cubeMash.CreateObjectFromFile("Models/Et.obj");
	// Material
	Material cubeMaterial(m_litShader);
	cubeMaterial.SetTexture(m_defaultTexture);
	cubeMaterial.SetMaterialColor({ 1, 1, 1 });
	m_cubeMaterial = &cubeMaterial;

	// Posiciona Camera
	m_camera.GetTransform().position.x = 0.0f;
	m_camera.GetTransform().position.z = 20.0f;
	m_camera.GetTransform().position.y = 6.0f;

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
		m_scene,
		m_camera,
		aspect
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
	//m_sceneSerializer.Load(m_scene, "Scene.txt");
	// Loop Princial
	while (m_isRunning && !m_window.ShouldClose())
	{
		m_window.PoolEvents();
		TimePoint currentTime = Clock::now();
		std::chrono::duration<float> elapsed = currentTime - lastTime;

		ProcessInput();
		deltaTime = elapsed.count();
		accumulator += deltaTime;

		// Loop Fixo
		while (accumulator >= fixedDeltaTime)
		{
			FixedUpdate();
			accumulator -= fixedDeltaTime;
		}

		// Game Update
		Update(deltaTime);

		// Renderiza
		m_renderer.Clear();
		
		

		// Começo do frame ImGui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		float dammySpacing = 5.0;
		// Widgets
		//ImGui::ShowDemoWindow();
		if (m_selectedObject)
		{
			ImGui::Begin("Inspector");
				MeshComponent* meshComponent = m_selectedObject->GetComponent<MeshComponent>();
				if(meshComponent)
				{
					Material* material = meshComponent->GetMaterial();
					Transform& transform = m_selectedObject->GetTransform();
					if(material)
					{
						ImGui::Text(("Object Name: " + m_selectedObject->GetName()).c_str());
						ImGui::Dummy(ImVec2(0.0f, dammySpacing)); 
						ImGui::Separator();//----------------------------------------
						ImGui::Dummy(ImVec2(0.0f, dammySpacing)); 
						
						ImGui::Text("Material Color");
						ImGui::ColorEdit3(" ", glm::value_ptr(material->GetMaterialColor()));
						ImGui::Dummy(ImVec2(0.0f, dammySpacing)); 

					}

					ImGui::Separator(); //-------------------------------------------
					ImGui::Dummy(ImVec2(0.0f, dammySpacing)); 
					ImGui::Text("Transform");
					ImGui::DragFloat3("Position", &transform.position.x, 0.1f);
					ImGui::DragFloat3("Rotation", &transform.rotation.x, 0.1f);
					ImGui::DragFloat3("Scale", &transform.scale.x, 0.1f);
					ImGui::Dummy(ImVec2(0.0f, dammySpacing)); 
					ImGui::Separator(); // ----------------------------------------

					ImGui::Dummy(ImVec2(0.0f, dammySpacing)); 
					if (ImGui::Button("SaveScene", ImVec2(100, 20)))
					{
						m_sceneSerializer.Save(m_scene, "Scene.txt");
						m_SceneSaved = true;
						saveMessageTimer = 2.0f;
					}

					if (m_SceneSaved)
					{
						ImGui::Text("Scene Saved!");
					}

					if (saveMessageTimer > 0.0)
					{
						saveMessageTimer -= deltaTime;
					}

					if (saveMessageTimer <= 0.0)
					{
						m_SceneSaved = false;
					}
				}
			ImGui::End();

			ImGui::Begin("Hieranchy");
				for(auto& object : m_scene.GetGameObjects())
				{
					bool selected = object.get() == m_selectedObject;
				
					if(ImGui::Selectable(object->GetName().c_str(), selected)){
						m_selectedObject = object.get();
					}
				
				}
				ImGui::Separator();
				ImGui::Dummy(ImVec2(0.0f, dammySpacing));
			ImGui::End();
			
			ImGui::Begin("Lighting");
			ImGui::Text("Directional Light");
				ImGui::DragFloat3("Direction", glm::value_ptr(m_scene.GetDirectionalLight().Direction));
				ImGui::ColorEdit3("D_Color", glm::value_ptr(m_scene.GetDirectionalLight().Color));
				ImGui::DragFloat("D_Intensity", &m_scene.GetDirectionalLight().Intensity, 0.0f, 1.0f, 10.0);
				ImGui::Dummy(ImVec2(0.0f, dammySpacing));
				ImGui::Separator();
				ImGui::Dummy(ImVec2(0.0f, dammySpacing));
				ImGui::Text("Ambiente");
				ImGui::ColorEdit3("A_Color", glm::value_ptr(m_scene.GetAmbienteLight().Color));
				ImGui::DragFloat("A_Intensity", &m_scene.GetAmbienteLight().Intensity, 1.0);
			ImGui::End();

			ImGui::Begin("Create");
				if(ImGui::Button("Create Cube"))
				{
					GameObject* cube = CreateCube();
					m_selectedObject = cube;
				}
				if(ImGui::Button("Create Plane"))
				{
					GameObject* plane = CreatePlane();
					m_selectedObject = plane;
				}
				ImGui::Dummy(ImVec2(0.0f, dammySpacing));
				ui::Separator();
				ImGui::Dummy(ImVec2(0.0f, dammySpacing));
				if(ui::Button("Delete Selected"))
				{
					m_scene.RemoveGameObject(m_selectedObject);
					if(m_scene.GetGameObjects().size() > 0)
					{
						m_selectedObject = m_scene.GetGameObjects()[0].get();
					}
					else
					{
						m_selectedObject = nullptr;
					}
				}
			ImGui::End();
		}
		m_renderer.DrawScene(renderContext);
		glm::vec3 start = glm::vec3(m_selectedObject->GetTransform().position.x, 
			m_selectedObject->GetTransform().position.y, 
			m_selectedObject->GetTransform().position.z);

		glm::vec3 end = glm::vec3(m_selectedObject->GetTransform().position.x, 
			m_selectedObject->GetTransform().position.y + 3,
			m_selectedObject->GetTransform().position.z);

		//m_renderer.DrawLine(start, end, glm::vec3(0.0, 1.0, 0.0), m_camera.GetViewProjectionMatrix(aspect));
		m_renderer.DrawTransformGizmos(*m_selectedObject, m_camera, aspect);

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


GameObject* App::CreateCube()
{
	
	// Cria objeto 02
	std::string name = m_scene.GenerateUniqueName("Cube");
	GameObject& cube = m_scene.CreateGameObject(name, ObjectType::Cube);
	cube.AddComponent(std::make_unique<MeshComponent>(&m_cubeMash, m_cubeMaterial));
	cube.GetTransform().position.x = 0.0f;
	cube.GetTransform().position.y = 1.0f;
	cube.GetTransform().position.z = 0.0f;

	return &cube;
}

GameObject* App::CreatePlane()
{
	
	// Cria objeto 02
	std::string name = m_scene.GenerateUniqueName("Plane");
	GameObject& plane = m_scene.CreateGameObject(name, ObjectType::Plane);
	plane.AddComponent(std::make_unique<MeshComponent>(&m_planeMash, m_planeMaterial));

	return &plane;
}

GameObject* App::CreateObjectFromType(ObjectType type)
{
	switch(type)
	{
		case ObjectType::Cube:
			return CreateCube();
			break;
		case ObjectType::Plane:
			return CreatePlane();
			break;
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
			GameObject* object = CreateObjectFromType(data.type);
			object->SetName(data.name);
			if (m_selectedObject == nullptr) { m_selectedObject = object; }
			Transform& transform = object->GetTransform();
			Material* material = object->GetComponent<MeshComponent>()->GetMaterial();

			transform.position.x = data.position.x;
			transform.position.y = data.position.y;
			transform.position.z = data.position.z;

			transform.rotation.x = data.rotation.x;
			transform.rotation.y = data.rotation.y;
			transform.rotation.z = data.rotation.z;

			transform.scale.x = data.scale.x;
			transform.scale.y = data.scale.y;
			transform.scale.z = data.scale.z;

			material->GetMaterialColor().x = data.color.x;
			material->GetMaterialColor().y = data.color.y;
			material->GetMaterialColor().z = data.color.z;

		}
	}
}

void App::LoadDirectionalLightScene()
{
	DirectionalLightdata data;
	data = m_sceneSerializer.LoadDirectionalLightData("Scene.txt");

	m_scene.GetDirectionalLight().Direction.x = data.direction.x;
	m_scene.GetDirectionalLight().Direction.y = data.direction.y;
	m_scene.GetDirectionalLight().Direction.z = data.direction.z;

	m_scene.GetDirectionalLight().Color.x = data.color.x;
	m_scene.GetDirectionalLight().Color.y = data.color.y;
	m_scene.GetDirectionalLight().Color.z = data.color.z;

	m_scene.GetDirectionalLight().Intensity = data.intensity;

}
void App::LoadAmbinetLightScene()
{

	AmbineteLightData data;
	data = m_sceneSerializer.LoadAmbienteLightdata("Scene.txt");

	m_scene.GetAmbienteLight().Color.x = data.color.x;
	m_scene.GetAmbienteLight().Color.y = data.color.y;
	m_scene.GetAmbienteLight().Color.z = data.color.z;

	m_scene.GetAmbienteLight().Intensity = data.intensity;

}

void App::CreateDefaultScene()
{
	m_selectedObject = CreateCube();
	CreatePlane();
}