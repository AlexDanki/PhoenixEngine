#pragma once
#include <glm/glm.hpp>
#include "Texture.h"
#include "Shader.h"

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

private:

	void SendModelMatrix(Shader* shader, const GameObject& object);
	void SendViewProjection(Shader* shader, const Camera& camera, const float aspectRatio);
	void SendDirectionalLight(Shader* shader, const DirectionalLight& dirLight);
	void SendAmbineteLight(Shader* shader, const AmbienteLight& ambineteLight);

};