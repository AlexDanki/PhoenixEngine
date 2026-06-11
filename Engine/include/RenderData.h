#pragma once

class Scene;
class Camera;
class DirectionalLight;
class AmbienteLight;

struct RenderData
{
	Scene& Scene;
	Camera& Camera;
	DirectionalLight& DirectionalLight;
	AmbienteLight& AmbienteLight;
	float AspectRatio;
};