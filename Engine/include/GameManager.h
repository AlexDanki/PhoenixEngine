#pragma once
#include "ScriptComponent.h"


class GameManager : public ScriptComponent
{

public:
	void PlayerScored();
	void EnemyScored();

private:
	int playerScore = 0;
	int enemyScore = 0;
};