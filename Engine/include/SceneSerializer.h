#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "GameObject.h"

struct AmbineteLightData
{
	glm::vec3 color;
	float intensity;
};

struct DirectionalLightdata
{
	glm::vec3 direction;
	glm::vec3 color;
	float intensity;
};

struct SceneObjectData
{
	ObjectType type;
	std::string name;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::vec3 color;
};

class Scene;

class SceneSerializer
{
public:
	bool Save( const Scene& scene, const std::string& path);
	bool Load( Scene& scene, const std::string& path);

	std::vector<SceneObjectData> LoadAllGameObjectsData(const std::string& path);
	DirectionalLightdata LoadDirectionalLightData(const std::string& path);
	AmbineteLightData LoadAmbienteLightdata(const std::string& path);

	bool SceneFileExists(const std::string& path);
};