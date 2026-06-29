#pragma once
#include <vector>

class Scene;
class GameObject;

struct CollisionPair
{
	GameObject* A;
	GameObject* B;

	bool operator==(const CollisionPair& other) const
	{
		return A == other.A && B == other.B;
	}
};

class PhysicsSystem
{
public:
	void Update(Scene* scene);
	bool CollisionAlreadyExisted(const CollisionPair& pair);
	void NotifyCollisionEnter(GameObject* object, GameObject* other);
	
private:
	std::vector<CollisionPair> m_currentCollisions;
	std::vector<CollisionPair> m_previusCollisions;
};