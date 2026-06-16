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
struct RenderContext;

class Renderer
{
public:
	void Init();
	void Clear();
	void DrawMesh(const Mesh& mesh, Shader* shader);
	void DrawGameObject(const GameObject& object, RenderContext& renderContext);
	void DrawScene(RenderContext& renderContext);
	void DrawLine(glm::vec3& start, glm::vec3& end, glm::vec3& color, glm::mat4& viewProjection);

	void SetLineShader(Shader* shader) { m_lineShader = shader; }
	void DrawTransformGizmos(const GameObject& object, const Camera& camera, float aspect);

private:

	void SendModelMatrix(Shader* shader, const GameObject& object);
	void SendViewProjection(Shader* shader, const Camera& camera, const float aspectRatio);
	void SendDirectionalLight(Shader* shader, const DirectionalLight& dirLight);
	void SendAmbineteLight(Shader* shader, const AmbienteLight& ambineteLight);

	

	Shader* m_lineShader;
	unsigned int m_lineVBO;
	unsigned int m_lineVAO;
};