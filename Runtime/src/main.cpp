#include "App.h"

int main()
{
	App engine;
	engine.Run();
	engine.Initialize();
	while(engine.IsRunning())
	{
		engine.StartFrame();

		engine.FixedStep();
		engine.Update();
		engine.Render();

		engine.EndFrame();
	}
	engine.ShutDown();
}