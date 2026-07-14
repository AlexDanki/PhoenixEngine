#include "ComponentFactory.h"
#include "MeshComponent.h"
#include "WorldTextRenderComponent.h"
#include "Component.h"

std::unique_ptr<Component> ComponentFactory::CreateComponent(const std::string& componentName)
{
	if (componentName == MeshComponent::ComponentName)
	{
		return std::make_unique<MeshComponent>();
	}
	else if(componentName == WorldTextRenderComponent::ComponentName)
	{
		return std::make_unique<WorldTextRenderComponent>();
	}
	// Add more component types here as needed
	return nullptr;
}