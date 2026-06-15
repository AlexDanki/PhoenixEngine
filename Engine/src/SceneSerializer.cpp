#include "SceneSerializer.h"
#include "fstream"
#include "Scene.h"
#include "Transform.h"
#include <iostream>
#include <sstream>
#include <glm/glm.hpp>
#include "Material.h"
#include "MeshComponent.h"
#include "App.h"
#include "DirectionalLight.h"
#include "AmbienteLight.h"

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

	file<<"LIGHTS"
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
		Transform& transform = object->GetTransform();
		glm::vec3 materialColor = object->GetComponent<MeshComponent>()->GetMaterial()->GetMaterialColor();

		std::string objectType = " ";

		if(object->GetType() == ObjectType::Cube)
		{
			objectType = "Cube";
		}
		else if(object->GetType() == ObjectType::Plane)
		{
			objectType = "Plane";
		}

		file<< objectType
			<< " "
			<< object->GetName() 
			<< " "
			<< transform.position.x 
			<< " "
			<< transform.position.y 
			<< " "
			<< transform.position.z 

			<< " "
			<< transform.rotation.x
			<< " "
			<< transform.rotation.y
			<< " "
			<< transform.rotation.z

			<< " "
			<< transform.scale.x
			<< " "
			<< transform.scale.y
			<< " "
			<< transform.scale.z

			<<" "
			<< materialColor.x
			<<" "
			<< materialColor.y
			<<" "
			<< materialColor.z
			<< "\n";

	}

	file.close();

	return true;
}

bool SceneSerializer::Load(Scene& scene, const std::string& path)
{
	std::ifstream file(path);

	if(!file.is_open())
	{
		std::cout << "Erro ao abrir Scene.txt: Não foi possível carregar a cena.";
		return false;
	}

	std::string line;
	while(std::getline(file, line))
	{
		std::stringstream ss(line);

		std::string objectType;
		std::string objectName;
		// Position
		float px; 
		float py;
		float pz;

		// Rotation
		float rx;
		float ry;
		float rz;

		// Scale
		float sx;
		float sy;
		float sz;

		// Material Color
		float cx;
		float cy;
		float cz;

		ss >> objectType;
		ss >> objectName;
		ss >> px;
		ss >> py;
		ss >> pz;

		ss >> rx;
		ss >> ry;
		ss >> rz;

		ss >> sx;
		ss >> sy;
		ss >> sz;

		ss >> cx;
		ss >> cy;
		ss >> cz;

		for (auto& object : scene.GetGameObjects())
		{
			if (object->GetName() == objectName)
			{
				object->GetTransform().position.x = px;
				object->GetTransform().position.y = py;
				object->GetTransform().position.z = pz;

				object->GetTransform().rotation.x = rx;
				object->GetTransform().rotation.y = ry;
				object->GetTransform().rotation.z = rz;

				object->GetTransform().scale.x = sx;
				object->GetTransform().scale.y = sy;
				object->GetTransform().scale.z = sz;

				Material* material = object->GetComponent<MeshComponent>()->GetMaterial();
				material->SetMaterialColor(glm::vec3(cx, cy, cz));

			}
		}
		
	}

	return true;
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
	SceneObjectData data;

	bool startLoad = false;
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

		std::stringstream ss(line);
		std::string typeString;
		
		ss >> typeString;
		ss >> data.name;

		ss >> data.position.x;
		ss >> data.position.y;
		ss >> data.position.z;

		ss >> data.rotation.x;
		ss >> data.rotation.y;
		ss >> data.rotation.z;

		ss >> data.scale.x;
		ss >> data.scale.y;
		ss >> data.scale.z;

		ss >> data.color.x;
		ss >> data.color.y;
		ss >> data.color.z;

		ObjectType objectType;

		if(typeString == "Cube")
		{
			objectType = ObjectType::Cube;
		}
		else if (typeString == "Plane")
		{
			objectType = ObjectType::Plane;
		}

		data.type = objectType;

		sceneData.push_back(data);

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
			std::cout << "Não foi possível abrir arquivo Scene.txt. DirectionalLight default carregado" << std::endl;
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