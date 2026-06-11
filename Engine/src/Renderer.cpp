#include "Mesh.h"
#include "GameObject.h"
#include "Renderer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Scene.h"
#include "Camera.h"
#include "MeshComponent.h"
#include <glm/gtc/type_ptr.hpp>
#include "DirectionalLight.h"
#include "AmbienteLight.h"
#include "iostream";


void Renderer::Init()
{
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
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
    in vec2 UV;
    void main()
    {
        vec3 baseColor = texture(u_Texture, UV).rgb;
        vec3 ambiente = u_AmbienteColor * u_AmbienteIntensity;
        vec3 normal = normalize(Normal);
        vec3 lightDir = normalize(u_sun.Direction);
        float diffuse = max(dot(normal, lightDir), 0.0);
        vec3 finalColor =  baseColor * ((diffuse * u_sun.Color * u_sun.Intensity ) + ambiente );
        FragColor = vec4(finalColor, 1.0);
    }
    )";

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSrc, nullptr);
    glCompileShader(fragmentShader);

    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vertexShader);
    glAttachShader(m_shaderProgram, fragmentShader);
    glLinkProgram(m_shaderProgram);

    m_modelLocation = glGetUniformLocation(m_shaderProgram, "u_Model");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    
    if(!m_texture.LoadFromFile("../../../../Assets/Textures/AreiaTex.png"))
    {
        std::cout << "Erro ao carregar AreaTex.png" << std::endl;
    }

    glUseProgram(m_shaderProgram);
    GLuint textureLoc = glGetUniformLocation(m_shaderProgram, "u_Texture");
    glUniform1i(textureLoc, 0);
    
}

void Renderer::Clear()
{
	glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::DrawMesh(const Mesh& mesh)
{
	glUseProgram(m_shaderProgram);
	glBindVertexArray(mesh.GetVAO());
    glDrawElements(GL_TRIANGLES, mesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawGameObject(const GameObject& object)
{
    const MeshComponent* meshComponent = object.GetComponent<MeshComponent>();

    if (!meshComponent) { return; }

    const Mesh* mesh = meshComponent->GetMesh();

    if (!mesh) { return; }
    
    SendModelMatrix(object);

    DrawMesh(*mesh);

}

void Renderer::DrawScene(const Scene& scene, const Camera& camera, 
    const DirectionalLight& dirLight, const AmbienteLight& ambiente, const float aspectRatio)
{
    
    glUseProgram(m_shaderProgram);

    SendViewProjection(camera, aspectRatio);
    SendDirectionalLight(dirLight);

    GLuint ambinetColorLoc = glGetUniformLocation(m_shaderProgram, "u_AmbienteColor");
    glUniform3fv(ambinetColorLoc, 1, glm::value_ptr(ambiente.Color));
    GLuint ambinetIntensityLoc = glGetUniformLocation(m_shaderProgram, "u_AmbienteIntensity");
    glUniform1f(ambinetIntensityLoc, ambiente.Intensity);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture.GetID());

    for (const auto& gameObject : scene.GetGameObjects())
    {
        DrawGameObject(*gameObject);
	}
}

void Renderer::SendModelMatrix(const GameObject& object)
{
    glm::mat4 model = object.GetTransform().GetMatrix();

    glUseProgram(m_shaderProgram);
    glUniformMatrix4fv(m_modelLocation, 1, GL_FALSE, glm::value_ptr(model));
}

void Renderer::SendViewProjection(const Camera& camera, const float aspectRatio)
{
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = camera.GetProjectionMatrix(aspectRatio);
    glm::mat4 viewProjection = projection * view;

    GLint uniformLoc = glGetUniformLocation(m_shaderProgram, "u_ViewProjection");
    glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(viewProjection));
}

void Renderer::SendDirectionalLight(const DirectionalLight& dirLight)
{
    GLuint directionalLightLoc = glGetUniformLocation(m_shaderProgram, "u_sun.Direction");
    glUniform3fv(directionalLightLoc, 1, glm::value_ptr(dirLight.Direction));

    GLuint LightColorLoc = glGetUniformLocation(m_shaderProgram, "u_sun.Color");
    glUniform3fv(LightColorLoc, 1, glm::value_ptr(dirLight.Color));

    GLuint LightIntensityLoc = glGetUniformLocation(m_shaderProgram, "u_sun.Intensity");
    glUniform1f(LightIntensityLoc, dirLight.Intensity);
}