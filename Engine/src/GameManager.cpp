#include "GameManager.h"
#include <iostream>

void GameManager::PlayerScored()
{
	playerScore++;
	std::cout << "Player Score :" << playerScore << std::endl;
}
void GameManager::EnemyScored()
{
	enemyScore++;
	std::cout << "Enemy Score :" << enemyScore << std::endl;
}