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