#pragma once
#include <glm/glm.hpp>


struct Fog
{
	bool enabled = true;
	glm::vec3 color{ 0.6, 0.65, 0.7 };
	float density = 0.02f;
};