#pragma once
#include "Viewport.h"
#include <glm/glm.hpp>
#include "Camera/ICamera.h"

class Scene;
class Camera;
class DirectionalLight;
class AmbienteLight;

struct RenderContext
{
	Scene& scene;
	ICamera* renderCamera;
	float aspectRatio;
	Shader* textShader;
	int screenWidth;
	int screenHeight;
};