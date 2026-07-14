#pragma once
#include <string>
#include <memory>

class Component;

class ComponentFactory
{
public:
	std::unique_ptr<Component> CreateComponent(const std::string& componentName);

private:

};