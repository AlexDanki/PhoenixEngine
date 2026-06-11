#pragma once
#include <string>
#include <memory>
#include <vector>
#include "Transform.h"
#include <type_traits>
#include "Component.h"

class GameObject
{
public:
	GameObject(const std::string& name);

	const std::string GetName() const;

	Transform& GetTransform();
	const Transform& GetTransform() const;

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


private:
	std::string m_name;
	Transform m_transform;
	std::vector<std::unique_ptr<Component>> m_components;
};