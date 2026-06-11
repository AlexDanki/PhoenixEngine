#pragma once

class GameObject;

class Component
{
public:
	virtual ~Component() = default;

	void SetOwner(GameObject* owner);
	GameObject* GetOwner() const;

private:
	GameObject* m_owner = nullptr;

};