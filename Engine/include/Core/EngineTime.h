#pragma once
#include <chrono>

namespace Mse
{
	class Time
	{
	public:
		Time();
		float deltaTime; // Simulate a fixed time step (60 FPS)
		float fixedDeltaTime; // 60 FPS
		float accumulator;

		void Initialize();

		void StartFrame();
		void EndFrame();

	private:
		using Clock = std::chrono::high_resolution_clock;
		using TimePoint = std::chrono::time_point<Clock>;

		TimePoint currentTime;
		TimePoint lastTime;
	};
}