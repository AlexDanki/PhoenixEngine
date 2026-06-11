#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


struct Vector3
{
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
};

class Transform
{
public:
	Vector3 position;
	Vector3 rotation;
	Vector3 scale{ 1.0f, 1.0f, 1.0f };
	glm::vec3 position2;

	glm::mat4 GetMatrix() const;

};