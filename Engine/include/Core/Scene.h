#pragma once

#include <memory>
#include <string>
#include <vector>
#include "GameObject.h"
#include "DirectionalLight.h"
#include "AmbienteLight.h"
#include "CameraComponent.h"
#include "Fog.h"

//class GameObject;

class Scene 
{
public:

	// Chamado depois que a cena é carregada  
	void Start();
	// Chamada a cada frame
	void Update(float deltaTime);
	// Chamada a cada frame
	void FixedUpdate(float deltaTime);

	// Cria um objeto
	GameObject& CreateGameObject(const std::string& name, ObjectType type);
	// Remove objeto
	void RemoveGameObject(GameObject* gameObject);
	// Retorna todos os objetos da cena
	const std::vector<std::unique_ptr<GameObject>>& GetGameObjects() const;
	// Um nome único de objeto ainda não existente na cena
	std::string GenerateUniqueName(const std::string& baseName);

	// Pega a luz direcional
	DirectionalLight& GetDirectionalLight() { return m_directionalLight; }
	const DirectionalLight& GetDirectionalLight() const{ return m_directionalLight; }

	// Pega a luz ambinete
	AmbienteLight& GetAmbienteLight()  { return m_ambienteLight; }
	const AmbienteLight& GetAmbienteLight()  const { return m_ambienteLight; }

	// Pega a primera camera da hierarquia ativa no jogo 
	CameraComponent* GetPrimaryCamera() const;

	// retorna o primiero objeto na hierarquia com a tag apontada
	GameObject* GetGameObjectByTag(const std::string& tag);

	// Retorna o primiero objeto da hierarquia com nome apontado
	GameObject* GetGameObjectByName(const std::string& name);

	Fog& GetFog() { return m_fog; };
	const Fog& GetFog() const { return m_fog; }

private:
	std::vector<std::unique_ptr<GameObject>> m_gameObjects;
	DirectionalLight m_directionalLight;
	AmbienteLight m_ambienteLight;
	Fog m_fog;
};