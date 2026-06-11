#pragma once
#include <glm/glm.hpp>
#include "Texture.h"

class Mesh;
class Scene;
class Camera;
class GameObject;

class DirectionalLight;
class AmbienteLight;

class Renderer
{
public:
	void Init();
	void Clear();
	void DrawMesh(const Mesh& mesh);
	void DrawGameObject(const GameObject& object);
	void DrawScene(const Scene& scene, const Camera& camera, const DirectionalLight& dirLight, const AmbienteLight& ambiente, const float aspectRatio);

private:
	
	unsigned int m_shaderProgram = 0;
	unsigned int m_textureID;
	int m_modelLocation = -1;
	Texture m_texture;

	void SendModelMatrix(const GameObject& object);
	void SendViewProjection(const Camera& camera, const float aspectRatio);
	void SendDirectionalLight(const DirectionalLight& dirLight);
	void SendAmbienteLight();

};