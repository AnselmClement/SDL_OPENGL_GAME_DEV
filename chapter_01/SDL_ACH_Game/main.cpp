#include "Game.h"

int main(int argc, char** argv)
{
	Game game;
	bool result = game.Initialize();
	if (result)
	{
		game.RunLoop();
	}
	game.Shutdown();

	return 0;
}