#include "Game.h"
#include "engine/Application.h"

int main(int argc, char *argv[])
{
	Application *application = CreateApplication();
	application->Run();
	delete application;
	// Game game;
	// bool success = game.Initialize();
	// if (success)
	// 	game.Update();
	// game.Shutdown();
	return 0;
}