#pragma once
#include <glm/glm.hpp>
class AmbienteLight
{
public:
	glm::vec3 Color = glm::vec3{ 0.4, 0.3, 0.6 }; // 0.4, 0.3, 0.6
	float Intensity = 0.2f;
};