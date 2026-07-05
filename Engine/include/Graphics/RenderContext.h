#pragma once

class Scene;
class Camera;
class DirectionalLight;
class AmbienteLight;

struct RenderContext
{
	Scene& scene;
	Camera& camera;
	float aspectRatio;
	Shader* textShader;
	int screenWidth;
	int screenHeight;
};