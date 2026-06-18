#pragma once

class GameObject;

class Component
{
public:

	virtual ~Component() = default;
	virtual void OnAttach() {};
	void SetOwner(GameObject* owner);
	GameObject* GetOwner() const;

protected:
	GameObject* m_owner = nullptr;

};