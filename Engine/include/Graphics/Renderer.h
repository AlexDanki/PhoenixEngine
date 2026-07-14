#pragma once
#include <glm/glm.hpp>
#include "Texture.h"
#include "Shader.h"
#include "AssetManager.h"

class Mesh;
class Scene;
class Camera;
class GameObject;

class DirectionalLight;
class AmbienteLight;
class Shader;
class BoxCollider;
struct RenderContext;

constexpr glm::vec3 GRID_COLOR{ 0.35f, 0.35f, 0.35f };
constexpr glm::vec3 X_AXIS_COLOR{ 0.6f, 0.0f, 0.0f };
constexpr glm::vec3 Z_AXIS_COLOR{ 0.0f, 0.0f, 1.0f };

class Renderer
{
public:

	void Init();
	void Clear();
	void DrawMesh(const Mesh& mesh, Shader* shader);
	void DrawGameObject(const GameObject& object, RenderContext& renderContext);
	void DrawScene(RenderContext& renderContext);
	void DrawLine(glm::vec3& start, glm::vec3& end, glm::vec3& color, glm::mat4& viewProjection);
	void DrawBoxCollider(BoxCollider& boxCollider, glm::mat4 viewProjection);

	// auxilaires para DrawGameObject
	void DrawMeshComponent(const GameObject& object, RenderContext& renderContext);
	void DrawTextComponent(const GameObject& object, RenderContext& renderContext);

	void SetLineShader(Shader* shader) { m_lineShader = shader; }
	void DrawTransformGizmos(const GameObject& object, RenderContext& renderContext, float aspect);

	void DrawWorldGrid(int quantLines, glm::mat4 viewProjectionMatrix);

private:

	void SendModelMatrix(Shader* shader, const GameObject& object);
	void SendViewProjection(Shader* shader, RenderContext& renderContext, const float aspectRatio);
	void SendOrthoProjection(RenderContext& renderContext);
	void SendDirectionalLight(Shader* shader, const DirectionalLight& dirLight);
	void SendAmbineteLight(Shader* shader, const AmbienteLight& ambineteLight);
	void SendFog(Shader* shader, RenderContext& renderContext);
	void SendCameraPosition(Shader* shader, RenderContext& renderContext);

	Shader* m_lineShader;
	unsigned int m_lineVBO = -1;
	unsigned int m_lineVAO = -1;
	unsigned int m_textVBO = -1;
	unsigned int m_textVAO = -1;

};