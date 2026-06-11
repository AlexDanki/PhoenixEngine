#include "Component.h"
#include "GameObject.h"

void Component::SetOwner(GameObject* owner)
{
	m_owner = owner;
}

GameObject* Component::GetOwner() const
{
	return m_owner;
}