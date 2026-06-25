#pragma once

#include <memory>
#include <string>
#include <vector>
#include "GameObject.h"
#include "DirectionalLight.h"
#include "AmbienteLight.h"
#include "CameraComponent.h"

//class GameObject;

class Scene 
{
public:

	void Update(float deltaTime);
	void FixedUpdate(float deltaTime);

	GameObject& CreateGameObject(const std::string& name, ObjectType type);

	void RemoveGameObject(GameObject* gameObject);

	const std::vector<std::unique_ptr<GameObject>>& GetGameObjects() const;

	std::string GenerateUniqueName(const std::string& baseName);

	DirectionalLight& GetDirectionalLight() { return m_directionalLight; }
	const DirectionalLight& GetDirectionalLight() const{ return m_directionalLight; }
	AmbienteLight& GetAmbienteLight()  { return m_ambienteLight; }
	const AmbienteLight& GetAmbienteLight()  const { return m_ambienteLight; }
	CameraComponent* GetPrimaryCamera() const;
	GameObject* GetGameObjectByTag(const std::string& tag);

private:
	std::vector<std::unique_ptr<GameObject>> m_gameObjects;

	DirectionalLight m_directionalLight;
	AmbienteLight m_ambienteLight;
};