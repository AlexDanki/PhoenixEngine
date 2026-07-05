#pragma once
#include <fstream>

class GameObject;
class Transform;
class Scene;

class Component
{
public:

	virtual ~Component() = default;

	// Chamado imediatamente quando o componete é adicionado ao GameObject
	virtual void OnAttach() {};

	// Seta o GameObject que adicionou o componente  
	void SetOwner(GameObject* owner);

	// Retorna o GameObject a quem o componente pertence
	GameObject* GetOwner() const;

	virtual void Serialize(std::ofstream& file) const {};

	virtual void Deserialize(std::ifstream& file) const {};

protected:
	GameObject* m_owner = nullptr;
	Transform* m_transform = nullptr;
	Scene* m_scene;

};