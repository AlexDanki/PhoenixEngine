#pragma once
#include "Component.h"
#include "GameObject.h"

class ScriptComponent:public Component
{
public:
	virtual void OnCreate() {}

	// Chamada uma única vez quando toda a cena já foi criada
	virtual void OnStart() {}

	// Chamada a cada frame 
	virtual void OnUpdate(float dt) {};

	// Chamado sempre que o colisor encontra outro
	virtual void OnCollisionEnter(GameObject* other) {};
};