#include "PhysicsSystem.h"
#include "Scene.h"
#include "GameObject.h"
#include "BoxCollider.h"
#include "ScriptComponent.h"
#include "Log.h"

void PhysicsSystem::Update(Scene* scene)
{
	m_previusCollisions = m_currentCollisions;
	m_currentCollisions.clear();
	auto& objects = scene->GetGameObjects();

	for(int i = 0; i < objects.size(); i++)
	{
		for(int j = i + 1; j < objects.size(); j++)
		{
			// Procurar collisões aqui
			GameObject* A = objects[i].get();
			GameObject* B = objects[j].get();

			BoxCollider* boxColliderA = A->GetComponent<BoxCollider>();
			BoxCollider* boxColliderB = B->GetComponent<BoxCollider>();

			if(boxColliderA && boxColliderB)
			{

				if(boxColliderA->Intersects(*boxColliderB))
				{
					CollisionPair pair;
					pair.A = boxColliderA->GetOwner();
					pair.B = boxColliderB->GetOwner();

					m_currentCollisions.push_back(pair);

					if(!CollisionAlreadyExisted(pair))
					{
						NotifyCollisionEnter(pair.A, pair.B);
						NotifyCollisionEnter(pair.B, pair.A);
					}

				}
			}
		}
	}
}

bool PhysicsSystem::CollisionAlreadyExisted(const CollisionPair& pair)
{
	for(auto& oldPair : m_previusCollisions)
	{
		if (oldPair == pair)
			return true;
	}

	return false;
}

void PhysicsSystem::NotifyCollisionEnter(GameObject* object, GameObject* other)
{
	for(auto& component : object->GetComponents())
	{ 
		ScriptComponent* scriptComponent = dynamic_cast<ScriptComponent*>(component.get());
		if(scriptComponent)
		{
			scriptComponent->OnCollisionEnter(other);
		}
	}
}