#include "Transform.h"

glm::mat4 Transform::GetMatrix() const
{
	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(position.x, position.y, position.z));
	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(scale.x, scale.y, scale.z));

	return model;
}