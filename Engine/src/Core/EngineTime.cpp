#include "EngineTime.h"


Mse::Time::Time()
	:deltaTime(0.0f),
	accumulator(0.0f),
	fixedDeltaTime(1.0f / 60.0f)
{
	Initialize();
}

void Mse::Time::Initialize()
{
	lastTime = Clock::now();
}

void Mse::Time::StartFrame()
{
	if (deltaTime > 0.05f) { deltaTime = 0.05f; }
	currentTime = Clock::now();
	std::chrono::duration<float> elapsed = currentTime - lastTime;

	deltaTime = elapsed.count();
	accumulator += deltaTime;
}

void Mse::Time::EndFrame()
{
	lastTime = currentTime;
}