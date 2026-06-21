#include "ScriptRegistry.h"

std::unordered_map<std::string, ScriptFactory> ScriptRegistry::s_registeredScripts;

void ScriptRegistry::Register(const std::string& scriptName, ScriptFactory factory)
{
	s_registeredScripts[scriptName] = factory;
}

std::unique_ptr<ScriptComponent> ScriptRegistry::Create(std::string scriptName)
{
	auto it = s_registeredScripts.find(scriptName);

	if(it != s_registeredScripts.end())
	{
		return it->second();
	}

	return nullptr;
}

std::unordered_map<std::string, ScriptFactory> ScriptRegistry::GetRegisteredScripts()
{
	return s_registeredScripts;
}
