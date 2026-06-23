#pragma once
#include <string>
#include <memory>
#include <vector>
#include "Transform.h"
#include <type_traits>
#include "Component.h"
#include <glm/glm.hpp>

enum class ObjectType
{
	Plane,
	Cube,
	Camera,
	Asset
};

class GameObject
{
public:

	bool dinamic = true;
	GameObject(const std::string& name, ObjectType type);

	glm::vec3 velocity = glm::vec3(0.0);

	const std::string GetName() const;
	const std::string GetTag() const { return m_tag; };

	void SetName(const std::string& name) { m_name = name; }
	void SetTag(const std::string& name) { m_tag = name; }

	Transform& GetTransform();
	const Transform& GetTransform() const;

	void UpdateScripts(float dt);

	ObjectType GetType() const;

	void AddComponent(std::unique_ptr<Component> component);

	template<typename T>
	T& AddComponent()
	{
		static_assert(
			std::is_base_of_v<Component, T>,
			"T must inherit from Component"
			);

		auto component = std::make_unique<T>();

		component->SetOwner(this);
		component->OnAttach();

		T& ref = *component;

		m_components.push_back(std::move(compoenent));

		return ref;
	}

	template<typename T>
	T* GetComponent()
	{
		for(auto& c : m_components)
		{
			T* casted = dynamic_cast<T*>(c.get());

			if(casted)
			{
				return casted;
			}
		}

		return nullptr;

	}

	template<typename T>
	const T* GetComponent() const
	{
		for (auto& c : m_components)
		{
			T* casted = dynamic_cast<T*>(c.get());

			if (casted)
			{
				return casted;
			}
		}

		return nullptr;

	}

	void SetAssetPath(const char* path) { m_assetPath = path; }
	void SetTexturePath(const char* path) { m_texturePath = path; }
	void SetType(ObjectType type) { m_type = type; }

	std::string GetAssetPath() { return m_assetPath; }
	std::string GetTexturePath() { return m_texturePath; }

	void SetScriptComponentName(std::string& scriptName);
	std::vector<std::string> GetScriptsComponentsNames(){return m_scriptsComponentsNames; }
	bool FindInScriptComponentsNames(std::string scriptName);

private:
	std::string m_name = "GameObject";
	std::string m_tag = "Untagged";
	std::string m_assetPath = "Engine";
	std::string m_texturePath = "Engine";
	Transform m_transform;
	ObjectType m_type;
	std::vector<std::unique_ptr<Component>> m_components;
	std::vector<std::string> m_scriptsComponentsNames;
};