#pragma once
#include <random>

class Random
{
public:
	static int Range(int a, int b);
	static double Range(double a, double b);

private:
	static std::random_device rd;
	static std::mt19937 gen;
};