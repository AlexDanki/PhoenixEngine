#pragma once
#include <iostream>
#include <string>

class Log
{
public:
	static void Info(std::string string) { std::cout << string.c_str() << std::endl; }
	static void Info(int number) { std::cout << number << std::endl; }
	static void Info(float number) { std::cout << number << std::endl; }
	static void Info(bool state) { std::cout << state << std::endl; }
};