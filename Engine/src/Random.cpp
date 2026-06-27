#include "Random.h"

std::random_device Random::rd;
std::mt19937 Random::gen(Random::rd());

int Random::Range(int a, int b)
{
	std::uniform_int_distribution<>dist(a, b);
	return dist(gen);
}

double Random::Range(double a, double b)
{
	std::uniform_real_distribution<> dist(a, b);
	return dist(gen);
}