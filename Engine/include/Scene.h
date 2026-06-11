#pragma once

#include <memory>
#include <string>
#include <vector>
#include "GameObject.h"

//class GameObject;

class Scene 
{
public:

	GameObject& CreateGameObject(const std::string& name);

	const std::vector<std::unique_ptr<GameObject>>& GetGameObjects() const;

private:
	std::vector<std::unique_ptr<GameObject>> m_gameObjects;

};