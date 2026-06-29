#include "Component.h"
#include "GameObject.h"
#include "Transform.h"
#include "Scene.h"

void Component::SetOwner(GameObject* owner)
{
	m_owner = owner;
	m_transform = &owner->GetTransform();
	m_scene = owner->GetScene();
}

GameObject* Component::GetOwner() const
{
	return m_owner;
}