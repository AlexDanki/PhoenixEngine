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

// Permite a serialização do GameObject e de seus componentes para um arquivo
void GameObject::Serialize(std::ofstream& file) const
{
	file << "GAMEOBJECT\n";

	file << "TYPE\n";
	file << static_cast<int>(m_type) << "\n";

	file << "NAME\n";
	file << m_name << "\n";

	file << "TAG\n";
	file << m_tag << "\n";

	// Serializar a posição, rotação e escala do GameObject
	SerializeTransform(file);
	SerializeComponents(file);
	
	file << "END_GAMEOBJECT\n";
}

// Serializa a posição, rotação e escala do GameObject para um arquivo
void GameObject::SerializeTransform(std::ofstream& file) const
{
	file << "TRANSFORM\n";

	file << "POSITION\n";
	file << m_transform.position.x << " " 
		<< m_transform.position.y << " " 
		<< m_transform.position.z << "\n";

	file << "ROTATION\n";
	file << m_transform.rotation.x << " "
		<< m_transform.rotation.y << " "
		<< m_transform.rotation.z << "\n";

	file << "SCALE\n";
	file << m_transform.scale.x << " "
		<< m_transform.scale.y << " "
		<< m_transform.scale.z << "\n";
}

// Serializa os componentes do GameObject para um arquivo
void GameObject::SerializeComponents(std::ofstream& file) const
{
	for (const auto& component : m_components)
	{
		component->Serialize(file);
	}
}