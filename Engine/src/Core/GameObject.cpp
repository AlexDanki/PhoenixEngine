//#include "Component.h"
#include "GameObject.h"
#include "ScriptComponent.h"


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

void GameObject::StartScripts()
{
	for(auto& component : m_components)
	{
		if(auto script = dynamic_cast<ScriptComponent*>(component.get()))
		{
			script->OnStart();
		}
	}
}

void GameObject::UpdateScripts(float dt)
{
	for(auto& component : m_components)
	{
		if(auto script = dynamic_cast<ScriptComponent*>(component.get()))
		{
			script->OnUpdate(dt);
		}
	}
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

void GameObject::SetScriptComponentName(std::string& scriptName)
{
	m_scriptsComponentsNames.push_back(scriptName);
}

bool GameObject::FindInScriptComponentsNames(std::string scriptName)
{
	for(auto curScriptName : m_scriptsComponentsNames)
	{
		if(curScriptName == scriptName)
		{
			return true;
		}
	}

	return false;
}

void GameObject::SetScene(Scene* scene)
{
	m_scene = scene;
}