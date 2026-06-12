#include "glad/glad.h"
#include "App.h"
#include <iostream>
#include <chrono>
#include "Mesh.h"
#include "Material.h"
#include "GameObject.h"
#include "MeshComponent.h"
#include "memory"
#include "RenderContext.h"
#include "AssetManager.h"

void App::Run()
{
	std::cout << "Engine Started" << std::endl;

	// Inicializa funções do OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return;
	}

	// Inicializa o Render
	m_renderer.Init();

	
	// Programas de Sahders 
	const char* vertexShaderSrc = R"(
    #version 330 core
    layout(location = 0) in vec3 aPos;
    layout(location = 1) in vec3 aNormal;
    layout(location = 2) in vec2 aUV;

    uniform mat4 u_Model;
    uniform mat4 u_ViewProjection;

    out vec3 Normal;
    out vec2 UV;
    void main()
    {
        gl_Position = u_ViewProjection * u_Model * vec4(aPos, 1.0);
        Normal = vec3(u_Model * vec4(aNormal, 0.0));
        UV = aUV;
    }
    )";

	const char* fragmentShaderSrc = R"(
    #version 330 core
    out vec4 FragColor;
    in vec3 Normal;

    struct DirectionalLight
    {
        vec3 Direction;
        vec3 Color;
        float Intensity;
    }; 

    uniform DirectionalLight u_sun;

    uniform vec3 u_AmbienteColor;
    uniform float u_AmbienteIntensity;
    uniform sampler2D u_Texture;
    uniform vec3 u_MaterialColor;
    in vec2 UV;
    void main()
    {
        vec3 baseColor = texture(u_Texture, UV).rgb * u_MaterialColor;
        vec3 ambiente = u_AmbienteColor * u_AmbienteIntensity;
        vec3 normal = normalize(Normal);
        vec3 lightDir = normalize(u_sun.Direction);
        float diffuse = max(dot(normal, lightDir), 0.0);
        vec3 finalColor =  baseColor * ((diffuse * u_sun.Color * u_sun.Intensity ) + ambiente );
        FragColor = vec4(finalColor, 1.0);
    }
    )";

	// Gerenciador de arquivos
	AssetManager assetManager;

	// Cria Shader
	Shader litShader;
	litShader.Create(vertexShaderSrc, fragmentShaderSrc);
	

	// Configura Material 01
	Texture* planeTex = assetManager.LoadTexture("../../../../Assets/Textures/GroundTex.png");
	if (!planeTex)
	{
		std::cout << "Erro ao carregar textura: GrountTex" << std::endl;
	}
	Material planeMaterial(&litShader);
	planeMaterial.SetMaterialColor(glm::vec3(1, 1, 1));
	planeMaterial.SetTexture(planeTex);
	planeMaterial.SetMaterialColor({0, 0.7, 0});
	Mesh planeMesh;
	planeMesh.CreatePlane();


	// Configura Material 02
	Texture* areiaTex = assetManager.LoadTexture("../../../../Assets/Textures/AreiaTex.png");
	if (!areiaTex)
	{
		std::cout << "Erro ao carregar textura: AreiaTex" << std::endl;
	}
	Material cubeMaterial(&litShader);
	cubeMaterial.SetTexture(areiaTex);
	cubeMaterial.SetMaterialColor({1, 1, 1});
	Mesh cubeMash;
	cubeMash.CreateCube();

	// Posiciona Camera
	m_camera.GetTransform().position.x = 0.0f;
	m_camera.GetTransform().position.z = 10.0f;
	m_camera.GetTransform().position.y = 3.0f;

	// Cria objeto 01
	GameObject& plane = m_scene.CreateGameObject("GameObject1");
	plane.AddComponent(std::make_unique<MeshComponent>(&planeMesh, &planeMaterial));
	plane.GetTransform().position.y = 0;

	// Cria objeto 02
	GameObject& cube = m_scene.CreateGameObject("GameObject2");
	cube.AddComponent(std::make_unique<MeshComponent>(&cubeMash, &cubeMaterial));
	cube.GetTransform().position.y = 1.0f;
	cube.GetTransform().position.z = -3.0f;
	cube.GetTransform().rotation.y = 60.0f;

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
		m_directionalLight,
		m_ambienteLight,
		aspect
	};

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
		m_renderer.DrawScene(renderContext);

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