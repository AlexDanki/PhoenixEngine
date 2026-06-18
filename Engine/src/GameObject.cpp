//#include "Component.h"
#include "GameObject.h"


GameObject::GameObject(const std::string& name, ObjectType type)
	:m_name(name),
	m_type(type)
{

}

const std::string GameObject::GetName() const
{
	return m_name;
}

Transform& GameObject::GetTransform()
{
	return m_transform;
}

const Transform& GameObject::GetTransform() const
{
	return m_transform;
}

void GameObject::AddComponent(std::unique_ptr<Component> component)
{
	component->SetOwner(this);
	component->OnAttach();
	m_components.push_back(std::move(component));
}

ObjectType GameObject::GetType() const
{
	return m_type;
}