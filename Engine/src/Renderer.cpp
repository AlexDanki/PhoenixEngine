#include "Mesh.h"
#include "Material.h"
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
#include "RenderContext.h"
#include "Shader.h"
#include "BoxCollider.h"

void Renderer::Init()
{

    glGenVertexArrays(1, &m_lineVAO);
    glGenBuffers(1, &m_lineVBO);

    glBindVertexArray(m_lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);

    glBufferData(GL_ARRAY_BUFFER, 6*sizeof(float), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(float) * 3,
        (void*)0
    );

    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

}

void Renderer::Clear()
{
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void Renderer::DrawMesh(const Mesh& mesh, Shader* shader)
{
    shader->Use();
    glBindVertexArray(mesh.GetVAO());
    glDrawElements(GL_TRIANGLES, mesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawLine(glm::vec3& start, glm::vec3& end, glm::vec3& color, glm::mat4& viewProjectionMatrix)
{
    const float vertices[] =
    {
        start.x, start.y, start.z,
        end.x, end.y, end.z
    };

    glBindVertexArray(m_lineVAO);
    glBindBuffer(
        GL_ARRAY_BUFFER,
        m_lineVBO
    );
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    m_lineShader->Use();
    glUniformMatrix4fv(m_lineShader->GetViewProjectionLocation(), 1, GL_FALSE, glm::value_ptr(viewProjectionMatrix));
    glUniform3f(m_lineShader->GetColorLocation(), color.x, color.y, color.z);
    glBindVertexArray(m_lineVAO);
    glDrawArrays(GL_LINES, 0, 2);

}

void Renderer::DrawGameObject(const GameObject& object, RenderContext& renderContext)
{
    const MeshComponent* meshComponent = object.GetComponent<MeshComponent>();

    if (!meshComponent) { return; }

    const Mesh* mesh = meshComponent->GetMesh();
    const Material* material = meshComponent->GetMaterial();

    if (!mesh || !material) { return; }

    Texture* texture = material->GetTexture();

    if (texture == nullptr)
    {
        return;
    }

    Shader* shader = material->GetShader();

    shader->Use();

    SendViewProjection(shader, renderContext.camera, renderContext.aspectRatio);
    SendDirectionalLight(shader, renderContext.scene.GetDirectionalLight());
    SendAmbineteLight(shader, renderContext.scene.GetAmbienteLight());
    SendModelMatrix(shader, object);

    glUniform3fv(shader->GetMaterialColorLocation(), 1, glm::value_ptr(material->GetMaterialColor()));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetID());

    DrawMesh(*mesh, shader);

}

void Renderer::DrawScene(RenderContext& renderContext)
{
    for (const auto& gameObject : renderContext.scene.GetGameObjects())
    {
        DrawGameObject(*gameObject, renderContext);
    }
}

void Renderer::DrawTransformGizmos(const GameObject& object, const Camera& camera, float aspect)
{
    glm::vec3 position;
    float size = 2.0;
    glm::mat4 viewProjection = camera.GetViewProjectionMatrix(aspect);

    position.x = object.GetTransform().position.x;
    position.y = object.GetTransform().position.y;
    position.z = object.GetTransform().position.z;
    
    // X axis
    glm::vec3 xEnd = glm::vec3(size, 0.0, 0.0) + position;
    glm::vec3 redColor(1.0, 0.0, 0.0);
    DrawLine(position, xEnd, redColor, viewProjection);

    // Y axis
    glm::vec3 yEnd = glm::vec3(0.0, size, 0.0) + position;
    glm::vec3 greenColor(0.0, 1.0, 0.0);
    DrawLine(position, yEnd, greenColor, viewProjection);

    // Z Axis
    glm::vec3 zEnd = glm::vec3(0.0, 0.0, size) + position;
    glm::vec3 blueColor(0.0, 0.0, 1.0);
    DrawLine(position, zEnd, blueColor, viewProjection);
}

void Renderer::DrawBoxCollider(BoxCollider& boxCollider, glm::mat4 viewProjection)
{
    glm::vec3 color(0, 1, 0);
    auto borders = boxCollider.GetCorners();

    // Edges de baixo
    DrawLine(borders[0], borders[1], color, viewProjection);
    DrawLine(borders[1], borders[2], color, viewProjection);
    DrawLine(borders[2], borders[3], color, viewProjection);
    DrawLine(borders[3], borders[0], color, viewProjection);

    //Edges de cima
    DrawLine(borders[4], borders[5], color, viewProjection);
    DrawLine(borders[5], borders[6], color, viewProjection);
    DrawLine(borders[6], borders[7], color, viewProjection);
    DrawLine(borders[7], borders[4], color, viewProjection);

    //Edges Verticais
    DrawLine(borders[0], borders[4], color, viewProjection);
    DrawLine(borders[1], borders[5], color, viewProjection);
    DrawLine(borders[2], borders[6], color, viewProjection);
    DrawLine(borders[3], borders[7], color, viewProjection);
}

void Renderer::SendModelMatrix(Shader* shader, const GameObject& object)
{
    glm::mat4 model = object.GetTransform().GetMatrix();

    glUniformMatrix4fv(shader->GetModelLocation(), 1, GL_FALSE, glm::value_ptr(model));
}

void Renderer::SendViewProjection(Shader* shader, const Camera& camera, const float aspectRatio)
{
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = camera.GetProjectionMatrix(aspectRatio);
    glm::mat4 viewProjection = projection * view;

    glUniformMatrix4fv(shader->GetViewProjectionLocation(), 1, GL_FALSE, glm::value_ptr(viewProjection));
}

void Renderer::SendDirectionalLight(Shader* shader, const DirectionalLight& dirLight)
{
    glUniform3fv(shader->GetDirectionalLightLocation(), 1, glm::value_ptr(dirLight.Direction));
    glUniform3fv(shader->GetDirectionalLightColorLocation(), 1, glm::value_ptr(dirLight.Color));
    glUniform1f(shader->GetDirectionalLightIntensityLocation(), dirLight.Intensity);
}

void Renderer::SendAmbineteLight(Shader* shader, const AmbienteLight& ambineteLight)
{
    glUniform3fv(shader->GetAmbineteColorLocation(), 1, glm::value_ptr(ambineteLight.Color));
    glUniform1f(shader->GetAmbineteIntensityLocation(), ambineteLight.Intensity);
}