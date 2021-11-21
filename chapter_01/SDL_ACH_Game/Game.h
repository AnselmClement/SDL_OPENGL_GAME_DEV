#pragma once

#include "SDL.h"
//class SDL_Window;

struct Vector2
{
	float x;
	float y;
};

class Game
{
public:
	Game();
	~Game();

	//
	bool Initialize();
	void RunLoop();
	void Shutdown();

private:
	//Helper functions for RunLoop function
	void GetInput();
	void UpdateGame();
	void GenerateOutput();

	bool m_isRunning;
	SDL_Window* m_window;
	SDL_Renderer* m_Renderer;

	Vector2 m_PaddlePos;
	Vector2 m_BallPos;
	Vector2 m_BallVel;

	Uint32 mTicksCount;
	int8_t mPaddleDir;
};

