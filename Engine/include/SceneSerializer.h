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
	// Object Info
	ObjectType type;
	std::string name;

	// Object Position
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	// Object Material
	glm::vec3 color;

	// Object Asset
	std::string assetPath;

	// Object BoxCollider
	bool hasBoxCollider = false;
	bool isTrigger = false;
	glm::vec3 center = glm::vec3(0.0f);
	glm::vec3 size = glm::vec3(1.0f);
};

class Scene;

class SceneSerializer
{
public:
	bool Save( const Scene& scene, const std::string& path);
	SceneObjectData BuildSceneObjectData(GameObject& object);
	//bool Load( Scene& scene, const std::string& path);

	std::vector<SceneObjectData> LoadAllGameObjectsData(const std::string& path);
	DirectionalLightdata LoadDirectionalLightData(const std::string& path);
	AmbineteLightData LoadAmbienteLightdata(const std::string& path);
	

	bool SceneFileExists(const std::string& path);
};