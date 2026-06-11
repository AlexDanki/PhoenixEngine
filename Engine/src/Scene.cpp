#include "Scene.h"


GameObject& Scene::CreateGameObject(const std::string& name)
{
	auto gameObject = std::make_unique<GameObject>(name);

	GameObject& ref = *gameObject;

	m_gameObjects.push_back(std::move(gameObject));

	return ref;
}

const std::vector<std::unique_ptr<GameObject>>& Scene::GetGameObjects() const
{
	return m_gameObjects;
}