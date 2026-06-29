#pragma once
#include <string>
#include <vector>
//#include <functional>
#include "ScriptComponent.h"
#include <unordered_map>

using ScriptFactory = std::function<std::unique_ptr<ScriptComponent>()>;

class ScriptRegistry
{
public:
	static void Register(const std::string& scriptName, ScriptFactory factory);

	static std::unique_ptr<ScriptComponent> Create(std::string scriptName);

	static std::unordered_map<std::string, ScriptFactory> GetRegisteredScripts();

private:
	static std::unordered_map<std::string, ScriptFactory> s_registeredScripts;
};