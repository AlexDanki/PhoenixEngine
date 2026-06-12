#pragma once

class Scene;
class Camera;
class DirectionalLight;
class AmbienteLight;

struct RenderContext
{
	Scene& scene;
	Camera& camera;
	DirectionalLight& dirLight;
	AmbienteLight& ambienteLight;
	float aspectRatio;
};