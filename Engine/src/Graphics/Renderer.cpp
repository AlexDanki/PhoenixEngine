#include "Mesh.h"
#include "Material.h"
#include "GameObject.h"
#include "Renderer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Scene.h"
#include "Camera.h"
#include "MeshComponent.h"
#include "WorldTextRenderComponent.h"
#include <glm/gtc/type_ptr.hpp>
#include "DirectionalLight.h"
#include "AmbienteLight.h"
#include "iostream";
#include "RenderContext.h"
#include "Shader.h"
#include "BoxCollider.h"
#include "Font.h"
#include "Fog.h"
#include "ft2build.h"
#include FT_FREETYPE_H

void Renderer::Init()
{

	glGenVertexArrays(1, &m_textVAO);
	glGenBuffers(1, &m_textVBO);

	glBindVertexArray(m_textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_textVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(
		0,
		4,
		GL_FLOAT,
		GL_FALSE,
		sizeof(float) * 4,
		(void*)0
	);

	glEnableVertexAttribArray(0);

	// Inicializa o shader de linhas
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

void Renderer::DrawScene(RenderContext& renderContext )
{
    Fog& fog = renderContext.scene.GetFog();
    for (const auto& gameObject : renderContext.scene.GetGameObjects())
    {
        DrawGameObject(*gameObject, renderContext);
    }

}

void Renderer::DrawGameObject(const GameObject& object, RenderContext& renderContext)
{

    DrawMeshComponent(object, renderContext);
    DrawTextComponent(object, renderContext);
}

void Renderer::DrawMeshComponent(const GameObject& object, RenderContext& renderContext)
{

    const MeshComponent* meshComponent = object.GetComponent<MeshComponent>();

    if (!meshComponent) { return; }

    const Mesh* mesh = meshComponent->GetMesh();
    const Material* material = meshComponent->GetMaterial();

    if (!mesh || !material) { return; }

    Texture* texture = material->GetTexture();

    if (texture == nullptr) { return; }

    Shader* shader = material->GetShader();

    shader->Use();

    SendViewProjection(shader, renderContext, renderContext.aspectRatio);
    SendDirectionalLight(shader, renderContext.scene.GetDirectionalLight());
    SendAmbineteLight(shader, renderContext.scene.GetAmbienteLight());
    SendModelMatrix(shader, object);
    SendFog(shader, renderContext);
    SendCameraPosition(shader, renderContext);

    glUniform3fv(shader->GetMaterialColorLocation(), 1, glm::value_ptr(material->GetMaterialColor()));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetID());
    DrawMesh(*mesh, shader);
}

void Renderer::DrawTextComponent(const GameObject& object, RenderContext& renderContext)
{
    const WorldTextRenderComponent* textRender = object.GetComponent<WorldTextRenderComponent>();

    if (!textRender) { return; }

    const Font* font = textRender->GetFont();

    if(!font) { return; }

    const std::string& text = textRender->GetText();

    if (text.empty()) return;

    float cursorX = 0.0f;
    float cursorY = 0.0f;

    for(auto c : text)
    {
        Font::Character character = font->GetCharacter(c);
        
		const float scale = textRender->GetScale();
		float w = character.size.x * scale;
		float h = character.size.y * scale;

        float vertices[] =
        {
            cursorX ,       cursorY + h,    0.0f, 0.0f,
            cursorX ,       cursorY ,       0.0f, 1.0f,
            cursorX + w,    cursorY ,       1.0f, 1.0f,

            cursorX ,       cursorY + h,    0.0f, 0.0f,
            cursorX + w,    cursorY ,       1.0f, 1.0f,
            cursorX + w,    cursorY + h,    1.0f, 0.0f
        };

        renderContext.textShader->Use();
		glBindVertexArray(m_textVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_textVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        
        glBindTexture(GL_TEXTURE_2D, character.textureID);
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(
            renderContext.textShader->GetUniformLocation("textTexture"),
            0
        );

        glEnable(GL_BLEND);

        glBlendFunc(
            GL_SRC_ALPHA,
            GL_ONE_MINUS_SRC_ALPHA
        );
        //SendOrthoProjection(renderContext);
        SendModelMatrix(renderContext.textShader, object);
		SendViewProjection(renderContext.textShader, renderContext, renderContext.aspectRatio);
        glDrawArrays(GL_TRIANGLES, 0, 6);
		cursorX += (character.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
        
    }
    glDisable(GL_BLEND);
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

void Renderer::DrawTransformGizmos(const GameObject& object, RenderContext& renderContext, float aspect)
{
    glm::vec3 position;
    float size = 2.0;
    glm::mat4 viewProjection = renderContext.renderCamera->GetViewProjection(aspect);

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

void Renderer::DrawWorldGrid(int quantLines, glm::mat4 viewProjectionMatrix)
{
    glm::vec3 start{ 0.0 };
    glm::vec3 end{ 0.0 };
    
    for(int x = -quantLines; x < quantLines + 1; x++)
    {
        if (x == 0)
        {
            // Down to Up im X Axis 
            start = glm::vec3{ x, 0, quantLines };
            end = glm::vec3{ x, 0, -quantLines };
            DrawLine(start, end, glm::vec3{ Z_AXIS_COLOR }, viewProjectionMatrix);

            // // Right to Left im Z axis 
            start = glm::vec3{ quantLines, 0, x };
            end = glm::vec3{ -quantLines, 0, x };
            DrawLine(start, end, glm::vec3{X_AXIS_COLOR}, viewProjectionMatrix);
        }
        else
        {
            // Down to Up | | |
            start = glm::vec3{ x, 0, quantLines };
            end = glm::vec3{ x, 0, -quantLines };
            DrawLine(start, end, glm::vec3{ GRID_COLOR }, viewProjectionMatrix);

            // Right to Left <---
            start = glm::vec3{ quantLines, 0, x };
            end = glm::vec3{ -quantLines, 0, x };
            DrawLine(start, end, glm::vec3{ GRID_COLOR }, viewProjectionMatrix);
        }
    }
}

void Renderer::SendModelMatrix(Shader* shader, const GameObject& object)
{
    glm::mat4 model = object.GetTransform().GetMatrix();

    glUniformMatrix4fv(shader->GetModelLocation(), 1, GL_FALSE, glm::value_ptr(model));
}

void Renderer::SendViewProjection(Shader* shader, RenderContext& renderContext, const float aspectRatio)
{
    if(auto camera = renderContext.renderCamera)
    {
        glUniformMatrix4fv(shader->GetViewProjectionLocation(), 1, GL_FALSE,
            glm::value_ptr(camera->GetViewProjection(aspectRatio)));
    }
}

void Renderer::SendOrthoProjection(RenderContext& renderContext)
{
	glm::mat4 projection = glm::ortho(0.0f, (float) renderContext.screenWidth,
		(float)renderContext.screenHeight, 0.0f);
    GLint orthlocation = glGetUniformLocation(renderContext.textShader->GetProgram(), "projection");
	glUniformMatrix4fv(orthlocation, 1, GL_FALSE, glm::value_ptr(projection));
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

void Renderer::SendFog(Shader* shader, RenderContext& renderContext)
{
    // Enviar Fog para shaders
    glUniform1i(shader->GetFogEnabledLocation(), renderContext.scene.GetFog().enabled);
    glUniform3fv(shader->GetFogColorLocation(), 1, glm::value_ptr(renderContext.scene.GetFog().color));
    glUniform1f(shader->GetFogDensityLocation(), renderContext.scene.GetFog().density);
}

void Renderer::SendCameraPosition(Shader* shader, RenderContext& renderContext)
{
    glUniform3fv(shader->GetCameraPositionLocation(), 1, glm::value_ptr(renderContext.renderCamera->GetPosition()));
}