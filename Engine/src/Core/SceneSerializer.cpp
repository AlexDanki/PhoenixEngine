#include "SceneSerializer.h"
#include "fstream"
#include "Scene.h"
#include "Transform.h"
#include <iostream>
#include <sstream>
#include <glm/glm.hpp>
#include "Material.h"
#include "MeshComponent.h"
#include "RigidbodyComponent.h"
#include "App.h"
#include "DirectionalLight.h"
#include "AmbienteLight.h"
#include "Log.h"

bool SceneSerializer::Save(const Scene& scene, const std::string& path)
{
	std::ofstream file(path);

	if(!file.is_open())
	{
		std::cout << "Erro ao abrir Scene.txt: Não foi possível salvar a cena.";
		return false;
	}

	const DirectionalLight& dirLight = scene.GetDirectionalLight();
	const AmbienteLight& ambineteLight = scene.GetAmbienteLight();

	file<< "----------------------------------------------\n"
		<< " PHOENIX_ENGINE V0.1 \n"
		<< "---------------------------------------------- \n"
		<<"LIGHTS"
		<<"\n"
		<< "DirectionalLight"
		<< " "
		<< dirLight.Direction.x
		<< " "
		<< dirLight.Direction.y
		<< " "
		<< dirLight.Direction.z

		<< " "
		<< dirLight.Color.x
		<< " "
		<< dirLight.Color.y
		<< " "
		<< dirLight.Color.z
		<< " "
		<< dirLight.Intensity
		<< "\n"
		<< "AmbienteLight"
		<< " "
		<< ambineteLight.Color.x
		<< " "
		<< ambineteLight.Color.y
		<< " "
		<< ambineteLight.Color.z
		<< " "
		<< ambineteLight.Intensity
		<< "\n"
		<< "\n"
		<<"GAMEOBJECTS"
		<< "\n";

	for(auto& object : scene.GetGameObjects())
	{
		/*Transform& transform = object->GetTransform();
		glm::vec3 materialColor = object->GetComponent<MeshComponent>()->GetMaterial()->GetMaterialColor();*/

		SceneObjectData data = BuildSceneObjectData(*object);

		std::string objectType = "Asset";

		if(data.type == ObjectType::Cube)
		{
			objectType = "Cube";
		}
		else if(data.type == ObjectType::Plane)
		{
			objectType = "Plane";
		}
		else if(data.type == ObjectType::Camera)
		{
			objectType = "Camera";
		}
		else if(data.type == ObjectType::Asset)
		{
			objectType = "Asset";
		}


		file << objectType
			<< " "
			// Object Info
			<< data.name
			<< " "
			<< data.tag
			<< " "
			// Object Transform
			<< data.position.x
			<< " "
			<< data.position.y
			<< " "
			<< data.position.z

			<< " "
			<< data.rotation.x
			<< " "
			<< data.rotation.y
			<< " "
			<< data.rotation.z

			<< " "
			<< data.scale.x
			<< " "
			<< data.scale.y
			<< " "
			<< data.scale.z

			// Object Material
			<< " "
			<< data.color.x
			<< " "
			<< data.color.y
			<< " "
			<< data.color.z

			// Object Asset
			<< " "
			<< data.assetPath

			// Object Texture
			<< " "
			<< data.texturePath

			// Object BoxCollider
			<< " "
			<< data.hasBoxCollider
			<< " "
			<< data.isTrigger

			<< " "
			<< data.boxCenter.x
			<< " "
			<< data.boxCenter.y
			<< " "
			<< data.boxCenter.z

			<< " "
			<< data.boxSize.x
			<< " "
			<< data.boxSize.y
			<< " "
			<< data.boxSize.z

			// Object Rigidbody
			<< " "
			<< data.hasRigidbody
			<< " "
			<< data.rigidbodyVelocity.x
			<< " "
			<< data.rigidbodyVelocity.y
			<< " "
			<< data.rigidbodyVelocity.z
			<< " "
			<< data.useGravit
			<< " "
			<< data.gravitScale
			<< "\n"
			<< "SCRIPTS\n";

		for(auto scriptName : data.scriptsNames)
		{
			file << scriptName << "\n";
			
		}
		file << "END SCRIPTS\n";
	}

	file.close();

	return true;
}

SceneObjectData SceneSerializer::BuildSceneObjectData(GameObject& object)
{
	

	SceneObjectData data;

	// Object Info
	data.type = object.GetType();
	data.name = object.GetName();
	data.tag = object.GetTag();


	Transform& transform = object.GetTransform();

	data.position.x = transform.position.x;
	data.position.y = transform.position.y;
	data.position.z = transform.position.z;

	data.rotation.x = transform.rotation.x;
	data.rotation.y = transform.rotation.y;
	data.rotation.z = transform.rotation.z;

	data.scale.x = transform.scale.x;
	data.scale.y = transform.scale.y;
	data.scale.z = transform.scale.z;

	glm::vec3 materialColor = object.GetComponent<MeshComponent>()->GetMaterial()->GetMaterialColor();

	data.color = materialColor;
	data.assetPath = object.GetAssetPath();
	data.texturePath = object.GetTexturePath();

	if(auto boxCollider = object.GetComponent<BoxCollider>())
	{
		data.hasBoxCollider = true;
		data.isTrigger = boxCollider->isTrigger;
		data.boxCenter = boxCollider->GetCenter();
		data.boxSize = boxCollider->GetSize();

	}
	else
	{
		data.hasBoxCollider = false;
	}

	if(auto rigidbody = object.GetComponent<RigidbodyComponent>())
	{
		data.hasRigidbody = true;
		data.rigidbodyVelocity.x = rigidbody->velocity.x;
		data.rigidbodyVelocity.y = rigidbody->velocity.y;
		data.rigidbodyVelocity.z = rigidbody->velocity.z;
		data.useGravit = rigidbody->useGravity;
		data.gravitScale = rigidbody->gravityScale;
	}
	else
	{
		data.hasRigidbody = false;
	}

	data.scriptsNames = object.GetScriptsComponentsNames();

	return data;

}

std::vector<SceneObjectData> SceneSerializer::LoadAllGameObjectsData(const std::string& path)
{
	std::vector<SceneObjectData> sceneData;

	std::ifstream file(path);
	if(!file.is_open())
	{
		return sceneData;
	}
	

	std::string line;
	

	bool startLoad = false;
	bool readingScripts = false;
	SceneObjectData currentObject = SceneObjectData();

	while(std::getline(file, line))
	{
		

		if(line == "GAMEOBJECTS")
		{
			startLoad = true;
			continue;
		}

		if(!startLoad)
		{
			continue;
		}

		if(line == "SCRIPTS")
		{
			readingScripts = true;
			continue;
		}

		if(line == "END SCRIPTS")
		{
			readingScripts = false;
			sceneData.push_back(currentObject);
			currentObject = SceneObjectData();
			continue;
		}

		if(readingScripts)
		{
			currentObject.scriptsNames.push_back(line);
			continue;
		}

		std::stringstream ss(line);
		std::string typeString;
		
		ss >> typeString;
		ss >> currentObject.name;
		ss >> currentObject.tag;

		ss >> currentObject.position.x;
		ss >> currentObject.position.y;
		ss >> currentObject.position.z;

		ss >> currentObject.rotation.x;
		ss >> currentObject.rotation.y;
		ss >> currentObject.rotation.z;

		ss >> currentObject.scale.x;
		ss >> currentObject.scale.y;
		ss >> currentObject.scale.z;

		ss >> currentObject.color.x;
		ss >> currentObject.color.y;
		ss >> currentObject.color.z;
		ss >> currentObject.assetPath;
		ss >> currentObject.texturePath;

		ss >> currentObject.hasBoxCollider;
		ss >> currentObject.isTrigger;

		ss >> currentObject.boxCenter.x;
		ss >> currentObject.boxCenter.y;
		ss >> currentObject.boxCenter.z;

		ss >> currentObject.boxSize.x;
		ss >> currentObject.boxSize.y;
		ss >> currentObject.boxSize.z;

		ss >> currentObject.hasRigidbody;

		ss >> currentObject.rigidbodyVelocity.x;
		ss >> currentObject.rigidbodyVelocity.y;
		ss >> currentObject.rigidbodyVelocity.z;

		ss >> currentObject.useGravit;
		ss >> currentObject.gravitScale;

		ObjectType objectType = ObjectType::Cube;

		if(typeString == "Cube")
		{
			objectType = ObjectType::Cube;
		}
		else if (typeString == "Plane")
		{
			objectType = ObjectType::Plane;
		}else if (typeString == "Camera")
		{
			objectType = ObjectType::Camera;
		}else if (typeString == "Asset")
		{
			objectType = ObjectType::Asset;
		}

		// Preencher Scripts

		currentObject.type = objectType;

	}

	return sceneData;

}

DirectionalLightdata SceneSerializer::LoadDirectionalLightData(const std::string& path)
{
	DirectionalLightdata sceneData;
	std::ifstream file(path);

	if(!file.is_open())
	{
		glm::vec3 direction(-0.55, 1.0, 0.2);
		glm::vec3 color(1.0, 1.0, 1.0);
		
		sceneData.direction = direction;
		sceneData.color = color;
		sceneData.intensity = 1.0f;
		std::cout << "Não foi possível abrir arquivo Scene.txt. DirectionalLight default carregado" << std::endl;
		return sceneData;
	}

	std::string line;
	while(std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string token = " ";

		ss >> token;

		DirectionalLightdata data;
		if(token == "DirectionalLight")
		{
			ss >> data.direction.x;
			ss >> data.direction.y;
			ss >> data.direction.z;

			ss >> data.color.x;
			ss >> data.color.y;
			ss >> data.color.z;

			ss >> data.intensity;
			sceneData = data;
			return sceneData;
		}

	}

	//return sceneData;
}

AmbineteLightData SceneSerializer::LoadAmbienteLightdata(const std::string& path)
{
	std::ifstream file(path);
	AmbineteLightData ambienteData;

	if (!file.is_open())
	{
		
		glm::vec3 color = glm::vec3(1.0, 1.0, 1.0);
		ambienteData.color = color;
		ambienteData.intensity = 0.2f;
		std::cout << "Não foi possível abrir arquivo Scene.txt. AmbienteLight default carregado" << std::endl;
		return ambienteData;
	}

	std::string line;
	while(std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string token;
		ss >> token;

		AmbineteLightData data;
		if(token == "AmbienteLight")
		{

			ss >> data.color.x;
			ss >> data.color.y;
			ss >> data.color.z;

			ss >> data.intensity;

			return data;
		}
	}

}

bool SceneSerializer::SceneFileExists(const std::string& path)
{
	std::ifstream file(path);
	bool exists = file.is_open();
	file.close();
	return exists;
}