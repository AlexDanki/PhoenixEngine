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

void Renderer::Init()
{
    
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
    glUseProgram(shader->GetProgram());
    glBindVertexArray(mesh.GetVAO());
    glDrawElements(GL_TRIANGLES, mesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr);
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

    glUseProgram(shader->GetProgram());

    SendViewProjection(shader, renderContext.camera, renderContext.aspectRatio);
    SendDirectionalLight(shader, renderContext.dirLight);
    SendAmbineteLight(shader, renderContext.ambienteLight);
    SendModelMatrix(shader, object);

    glUniform3fv(shader->GetMaterialColorLocation(), 1, glm::value_ptr(material->GetMaterialColor()));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetID());

    DrawMesh(*mesh, shader);

}

void Renderer::DrawScene(RenderContext& renderContext)
{

    /*glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture.GetID());*/

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

    //GLint uniformLoc = glGetUniformLocation(m_shader.GetProgram(), "u_ViewProjection");
    glUniformMatrix4fv(shader->GetViewProjectionLocation(), 1, GL_FALSE, glm::value_ptr(viewProjection));
}

void Renderer::SendDirectionalLight(Shader* shader, const DirectionalLight& dirLight)
{
    GLuint directionalLightLoc = glGetUniformLocation(shader->GetProgram(), "u_sun.Direction");
    glUniform3fv(directionalLightLoc, 1, glm::value_ptr(dirLight.Direction));

    GLuint LightColorLoc = glGetUniformLocation(shader->GetProgram(), "u_sun.Color");
    glUniform3fv(LightColorLoc, 1, glm::value_ptr(dirLight.Color));

    GLuint LightIntensityLoc = glGetUniformLocation(shader->GetProgram(), "u_sun.Intensity");
    glUniform1f(LightIntensityLoc, dirLight.Intensity);
}

void Renderer::SendAmbineteLight(Shader* shader, const AmbienteLight& ambineteLight)
{
    GLuint ambinetColorLoc = glGetUniformLocation(shader->GetProgram(), "u_AmbienteColor");
    glUniform3fv(ambinetColorLoc, 1, glm::value_ptr(ambineteLight.Color));
    GLuint ambinetIntensityLoc = glGetUniformLocation(shader->GetProgram(), "u_AmbienteIntensity");
    glUniform1f(ambinetIntensityLoc, ambineteLight.Intensity);
}