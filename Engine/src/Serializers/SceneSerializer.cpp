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
#include "WorldTextRenderComponent.h"
#include "App.h"
#include "DirectionalLight.h"
#include "AmbienteLight.h"
#include "Log.h"
#include "SerializeUtils.h"

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
		object->Serialize(file);
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

	if (auto meshComponent = object.GetComponent<MeshComponent>())
	{
		data.hasMeshComponent = true;
		glm::vec3 materialColor = meshComponent->GetMaterial()->GetMaterialColor();
		data.color = materialColor;
	}
	
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

	if(auto worldText = object.GetComponent<WorldTextRenderComponent>())
	{
		data.hasWorldText = true;
		data.worldText = worldText->GetText();
		data.textScale = worldText->GetScale();
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
	//SceneObjectData currentObject = SceneObjectData();

	while(std::getline(file, line))
	{

		if(line == "GAMEOBJECT")
		{
			SceneObjectData data;
			while(std::getline(file, line))
			{
				if(line == "END_GAMEOBJECT")
				{
					sceneData.push_back(data);
					break;
				}
				if (line == "TYPE")
				{
					std::getline(file, line);
					data.type = static_cast<ObjectType>(std::stoi(line));
					continue;
				}

				if (line == "NAME")
				{
					std::getline(file, line);
					data.name = line;
					continue;
				}

				if (line == "TAG")
				{
					std::getline(file, line);
					data.tag = line;
					continue;
				}

				if (line == "TRANSFORM")
				{
					ReadTransformData(file, data);
					continue;
				}

				if(line == "COMPONENT")
				{
					ReadComponentData(file, data);
					continue;
				}
			} // End While

		}
	}

	return sceneData;

}

void SceneSerializer::ReadTransformData(std::ifstream& file, SceneObjectData& data)
{
	std::string line;

	std::getline(file, line);

	std::stringstream ss(line);

	ss >>
		data.position.x >>
		data.position.y >>
		data.position.z;

	//-------------------

	std::getline(file, line);

	std::stringstream sr(line);

	sr >>
		data.rotation.x >>
		data.rotation.y >>
		data.rotation.z;

	//-------------------

	std::getline(file, line);

	std::stringstream sc(line);

	sc >>
		data.scale.x >>
		data.scale.y >>
		data.scale.z;
}

void SceneSerializer::ReadComponentData(std::ifstream& file, SceneObjectData& data)
{
	std::string line;

	std::getline(file, line);
	
	if(line == "MESH_RENDERER")
	{
		ReadMeshComponentData(file, data);
	}
	else if( line == "BOX_COLLIDER")
	{
		ReadBoxColliderComponentData(file, data);
	}
	else if( line == "RIGIDBODY")
	{
		ReadRigidbodyComponentData(file, data);
	}
	else if( line == "WORLD_TEXT")
	{
		ReadWorldTextComponentData(file, data);
	}
	else if(line == "SCRIPT")
	{
		ReadScriptsComponentData(file, data);
	}
	
}

void SceneSerializer::ReadMeshComponentData(std::ifstream& file, SceneObjectData& data)
{
	std::string line;
	data.hasMeshComponent = true;

	while (std::getline(file, line))
	{
		if (line == "END_COMPONENT")
		{
			break;
		}
		else if (line == "MESH")
		{
			std::getline(file, data.assetPath);
		}
		else if(line == "TEXTURE")
		{
			std::getline(file, data.texturePath);
		}
		else if(line == "COLOR")
		{
			std::getline(file, line);
			std::stringstream cs(line);
			cs >> data.color.x >> data.color.y >> data.color.z;
		}
	}
}

void SceneSerializer::ReadBoxColliderComponentData(std::ifstream& file, SceneObjectData& data)
{
	std::string line;

	data.hasBoxCollider = true;

	while(std::getline(file, line))
	{
		if(line == "END_COMPONENT")
		{
			break;
		}
		else if (line == "IS_TRIGGER")
		{
			std::getline(file, line);
			data.isTrigger = std::stoi(line);
		}
		else if(line == "CENTER")
		{
			data.boxCenter =  SerializeUtils::ReadVec3(file);
		}
		else if(line == "SIZE")
		{
			data.boxSize = SerializeUtils::ReadVec3(file);
		}
		
	}
}

void SceneSerializer::ReadRigidbodyComponentData(std::ifstream& file, SceneObjectData& data)
{
	std::string line;
	data.hasRigidbody = true;

	while(std::getline(file, line))
	{
		if(line == "END_COMPONENT")
		{
			break;
		}
		else if(line == "USE_GRAVIT")
		{
			std::getline(file, line);
			data.useGravit = std::stoi(line);
		}
		else if(line == "GRAVIT_SCALE")
		{
			std::getline(file, line);
			data.gravitScale = std::stof(line);
		}
	}
}

void SceneSerializer::ReadWorldTextComponentData(std::ifstream& file, SceneObjectData& data)
{
	std::string line;
	data.hasWorldText = true;

	while(std::getline(file, line))
	{
		if(line == "END_COMPONENT")
		{
			break;
		}
		
		else if(line == "TEXT")
		{
			data.worldText.clear();
			while(std::getline(file, line))
			{
				if(line == "END_TEXT")
				{
					break;
				}

				if(!data.worldText.empty())
				{
					data.worldText += '\n';
				}

				data.worldText += line;
			}
		}
		else if(line == "SCALE")
		{
			std::getline(file, line);
			data.textScale = std::stof(line);
		}

	}
}

void SceneSerializer::ReadScriptsComponentData(std::ifstream& file, SceneObjectData& data)
{
	std::string line;

	while(std::getline(file, line))
	{
		if(line == "END_COMPONENT")
		{
			break;
		}

		if(line == "CLASS")
		{
			std::getline(file, line);
			data.scriptsNames.push_back(line);
			//std::cout << "Script Name: " << line << std::endl;
		}
	}
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