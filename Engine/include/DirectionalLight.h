#pragma once
#include <glm/glm.hpp>


class DirectionalLight
{
public:
	glm::vec3 Direction = glm::vec3(-0.55, 1.0, 0.2); // -0.55, 1.0, 0.2
	glm::vec3 Color = glm::vec3(1.0f, 1.0f, 1.0f);
	float Intensity = 0.6f;
};