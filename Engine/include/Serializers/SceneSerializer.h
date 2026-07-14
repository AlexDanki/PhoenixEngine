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
	ObjectType type = ObjectType::Asset;
	std::string name = "GameObject";
	std::string tag = "Untagged";

	// Object Transform
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

	// Object Material
	bool hasMeshComponent = false;
	glm::vec3 color = glm::vec3(1.0f);

	// Object Asset
	std::string assetPath = "Engine";
	std::string texturePath = "Engine";

	// Object BoxCollider
	bool hasBoxCollider = false;
	bool isTrigger = false;
	glm::vec3 boxCenter = glm::vec3(0.0f);
	glm::vec3 boxSize = glm::vec3(1.0f);

	// Object Rigidbodie
	bool hasRigidbody = false;
	glm::vec3 rigidbodyVelocity = glm::vec3(0.0f);
	bool useGravit = false;
	float gravitScale = 1.0f;

	// Object World Text
	bool hasWorldText = false;
	std::string worldText = "World Text";
	float textScale = 1.0f;

	// Object Scripts Components
	std::vector<std::string> scriptsNames;
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

private:

	void ReadTransformData(std::ifstream& file, SceneObjectData& data);
	void ReadComponentData(std::ifstream& file, SceneObjectData& data);
	void ReadMeshComponentData(std::ifstream& file, SceneObjectData& data);
	void ReadBoxColliderComponentData(std::ifstream& file, SceneObjectData& data);
	void ReadRigidbodyComponentData(std::ifstream& file, SceneObjectData& data);
	void ReadWorldTextComponentData(std::ifstream& file, SceneObjectData& data);
	void ReadScriptsComponentData(std::ifstream& file, SceneObjectData& data);
};