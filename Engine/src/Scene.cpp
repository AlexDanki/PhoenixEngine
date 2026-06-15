#include "Scene.h"


GameObject& Scene::CreateGameObject(const std::string& name, ObjectType type)
{
	auto gameObject = std::make_unique<GameObject>(name, type);

	GameObject& ref = *gameObject;

	m_gameObjects.push_back(std::move(gameObject));

	return ref;
}

void Scene::RemoveGameObject(GameObject* gameObject)
{
	for(auto& it = m_gameObjects.begin();it != m_gameObjects.end(); it++)
	{
		if(it->get() == gameObject)
		{
			m_gameObjects.erase(it);
			return;
		}
	}
}

const std::vector<std::unique_ptr<GameObject>>& Scene::GetGameObjects() const
{
	return m_gameObjects;
}

std::string Scene::GenerateUniqueName(const std::string& baseName)
{
	std::string targetName = baseName;
	int index = 0;
	bool found = true;

	while(true)
	{
		bool found = false;
		for (auto& object : m_gameObjects)
		{
			if (object->GetName() == targetName)
			{
				found = true;
				break;
			}
		}
		
		if(!found)
		{
			return targetName;
		}

		index++;
		targetName = baseName + "(" + std::to_string(index) + ")";
	}

	
}