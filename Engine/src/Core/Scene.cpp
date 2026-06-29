#include "Scene.h"
#include "RigidbodyComponent.h"
#include "BoxCollider.h"
#include <iostream>
#include <algorithm>
#include "ScriptComponent.h"


// Chamada quando a cena é carregada
void Scene::Start()
{
	// Start de Scripts dos Objetos
	for (auto& object : m_gameObjects)
	{
		object->StartScripts();
	}
}

// Chamada uma vez a cada frame
void Scene::Update(float deltaTime)
{
	// Update de Scripts dos Objetos
	for (auto& object : m_gameObjects)
	{
		object->UpdateScripts(deltaTime);
	}
}

// Chamada uma vez a cada frame com deltaTime fixo
void Scene::FixedUpdate(float deltaTime)
{
	// Update de Rigidbodyes dos Objetos
	for(auto& object : m_gameObjects)
	{
		if(auto rigidbody = object->GetComponent<RigidbodyComponent>())
		{
			rigidbody->PhysicsUpdate(deltaTime);
		}
	}


	// Gerencia colisões entre os Objetos
	for(auto& object : m_gameObjects)
	{
		if (!object->dinamic) continue;
		auto collider = object->GetComponent<BoxCollider>();
		auto rigidbody = object->GetComponent<RigidbodyComponent>();

		if(collider && rigidbody)
		{
			for(auto& otherObject : m_gameObjects)
			{
				if (otherObject == object) continue;
				auto otherCollider = otherObject->GetComponent<BoxCollider>();


				if(otherCollider)
				{
					// Checa se algum dos collisores possuem BoxCollider como trigger
					bool isTriggerCollision = collider->isTrigger || otherCollider->isTrigger;
					if (isTriggerCollision) continue;

					if(collider->Intersects(*otherCollider))
					{
						

						float minAB_X = collider->GetMax().x - otherCollider->GetMin().x;
						float minBA_X = otherCollider->GetMax().x - collider->GetMin().x;
						float penetrationX = std::min(minAB_X, minBA_X);

						float minAB_Y = collider->GetMax().y - otherCollider->GetMin().y;
						float minBA_Y = otherCollider->GetMax().y - collider->GetMin().y;
						float penetrationY = std::min(minAB_Y, minBA_Y);

						float minAB_Z = collider->GetMax().z - otherCollider->GetMin().z;
						float minBA_Z = otherCollider->GetMax().z - collider->GetMin().z;
						float penetrationZ = std::min(minAB_Z, minBA_Z);

						float penetration = std::min(penetrationX, std::min(penetrationY, penetrationZ));

						Transform* transform = &object->GetTransform();
						Transform* otherTrans = &otherObject->GetTransform();
						auto otherRb = otherObject->GetComponent<RigidbodyComponent>();
						if(penetration == penetrationX)
						{
							float centerA = (collider->GetMin().x + collider->GetMax().x) * 0.5;
							float centerB = (otherCollider->GetMin().x + otherCollider->GetMax().x) * 0.5;

							if (centerA < centerB )
								transform->position.x -= penetrationX;
							
							if (centerA > centerB)
								transform->position.x += penetrationX;

							rigidbody->velocity.x = 0.0f;
						}

						if(penetration == penetrationY)
						{
							float centerA = (collider->GetMin().y + collider->GetMax().y) * 0.5;
							float centerB = (otherCollider->GetMin().y + otherCollider->GetMax().y) * 0.5;

							if(centerA > centerB)
							{
								
								transform->position.y += penetrationY;
								rigidbody->velocity.y = 0.0f;
							}
							if(centerA < centerB)
							{
								
								if(otherObject->dinamic)
								{
									transform->position.y -= penetrationY * 0.5;

									otherTrans->position.y += penetrationY * 0.5;
									if (otherRb)
									{
										otherRb->velocity.y = 0;
									}
								}
								else
								{
									transform->position.y -= penetrationY;
								}
							}
							//rigidbody->velocity.y = 0.0f;
							
						}

						if(penetration == penetrationZ)
						{
							float centerA = (collider->GetMin().z + collider->GetMax().z) * 0.5;
							float centerB = (otherCollider->GetMin().z + otherCollider->GetMax().z) * 0.5;

							if(centerA < centerB)
								transform->position.z -= penetrationZ;
							if (centerA > centerB)
								transform->position.z += penetrationZ;
							rigidbody->velocity.z = 0.0f;
						}

					}
				}
			}
		}
	}
}

// Cria um gameObject com nome e tipo especificados
GameObject& Scene::CreateGameObject(const std::string& name, ObjectType type)
{
	auto gameObject = std::make_unique<GameObject>(name, type);
	gameObject->SetScene(this);
	GameObject& ref = *gameObject;
	
	m_gameObjects.push_back(std::move(gameObject));

	return ref;
}

// Remove um GameObject
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

// Retorna todos os GameObjects da Cena 
const std::vector<std::unique_ptr<GameObject>>& Scene::GetGameObjects() const
{
	return m_gameObjects;
}

// Gera um nome diferente dos objetos que já estão na cena
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

// Pega a primiera câmera Primary do jogo -  Primary -> Indica que a câmera deve ser tratada como principal
// Se a cena tiver mais de uma câmera marcada como Primary a função retorna a primira na Hierarquia 
CameraComponent* Scene::GetPrimaryCamera() const
{
	for(auto& object : m_gameObjects)
	{
		if(auto camera = object->GetComponent<CameraComponent>())
		{
			if(camera->primary)
			{
				return camera;
			}
		}
	}

	return nullptr;
}

// Retorna o primiero objeto na Hierarquia com a Tag indicada
GameObject* Scene::GetGameObjectByTag(const std::string& tag)
{
	for(auto& object : m_gameObjects)
	{
		if(object->GetTag() == tag)
		{
			return object.get();
		}
	}

	return nullptr;
}

// Pega o primiero objeto na hierarquia com o nome indicado
GameObject* Scene::GetGameObjectByName(const std::string& name)
{
	for(auto& object : m_gameObjects)
	{
		if(object->GetName() == name)
		{
			return object.get();
		}
	}

	return nullptr;
}