#include "Game.h"

Game::Game() :m_window(nullptr), m_isRunning(true), mTicksCount(0)
{
	m_BallPos.x = 700.0;
	m_BallPos.y = 350.0;

	m_PaddlePos.x = 10.0;
	m_PaddlePos.y = 300.0;

	m_BallVel.x = -200.0f;
	m_BallVel.y = 235.0f;
}

Game::~Game()
{
}

bool Game::Initialize()
{
	//Initialize the SDL subsystem
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s",SDL_GetError());
		return false;
	}

	//Create the SDL window
	m_window = SDL_CreateWindow(
								"ACH Game", //Window title
								100,		//Top left X-coordinate of window
								100,		//Top left Y-coordiante of window
								1024,		//Width of the window
								768,		//Height of the window
								SDL_WINDOW_RESIZABLE   //Flags (0 - no flags set)
								);
	
	if (!m_window)
	{
		SDL_Log("Failed to create window : %s",SDL_GetError());
		return false;
	}

	//Create the renderer to draw graphics
	m_Renderer = SDL_CreateRenderer(
									m_window,		//Window to create renderer for
									-1,				//Which graphics driver to use, useful for multiple windows, (usually -1)
									SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
									);
	if (!m_Renderer)
	{
		SDL_Log("Failed to create the renderer : %s", SDL_GetError());
		return false;
	}


	return true;
}

void Game::RunLoop()
{
	while (m_isRunning)
	{
		GetInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(m_Renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void Game::GetInput()
{
	SDL_Event event;

	//While there are still events in the queue
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		//Handle different event types here

		//Called if window is closed
		case SDL_QUIT:
			m_isRunning = false;
			break;
		}
	}

	//Get the state of the keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);
	
	//If escape key is pressed, end the loop 
	if (state[SDL_SCANCODE_ESCAPE])
		m_isRunning = false;

	mPaddleDir = 0;
	
	if (state[SDL_SCANCODE_W])
		mPaddleDir -= 1;
	
	if (state[SDL_SCANCODE_S])
		mPaddleDir += 1;
}

void Game::UpdateGame()
{
	//Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	// Delta time is the difference in ticks from the last frame
	// (Converted to seconds)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	//Update the tick counts ( for the next frame )
	mTicksCount = SDL_GetTicks();

	//Clamp maximum delta time value
	if (deltaTime > 0.5f)
		deltaTime = 0.5f;

	//Paddle update based on input
	if (mPaddleDir != 0)
	{
		m_PaddlePos.y += mPaddleDir * 300.0f * deltaTime;

		//Make sure paddle doesn't move off screen
		if (m_PaddlePos.y < (100 / 2.0f + 10))
			m_PaddlePos.y = (100 / 2.0f + 10);
		else if(m_PaddlePos.y > (768.0f - 100 / 2.0f - 10))
			m_PaddlePos.y = 768.0f - 100 / 2.0f - 10;
	}

	//Ball position update - Velocity
	m_BallPos.x -= m_BallVel.x * deltaTime;
	m_BallPos.y -= m_BallVel.y * deltaTime;

	//Ball collision detection with the top wall
	if (m_BallPos.y <= 5 && m_BallVel.y < 0.0f)
	{
		m_BallVel.y *= -1;
	}

	//Ball collision with the paddle

	int diff = m_BallPos.y - m_PaddlePos.y;

	if (//Our y-difference is small enough 
		diff <= 300 / 2.0f &&
		//Ball is at correct x-position
		m_BallPos.x <= 25.0f && m_BallPos.x >= 20.0f &&
		//The ball is moving to the left
		m_BallVel.x < 0.0f)
	{
		m_BallVel.x *= -1.0f;
	}
}

void Game::GenerateOutput()
{
	//Assigns the color to the renderer
	SDL_SetRenderDrawColor(
							m_Renderer,
							128,            //R
							128,            //G
							128,		    //B
							255				//A
							);

	//Fills the back buffer with the assigned color, otherwise known as clearing the back buffer
	SDL_RenderClear(m_Renderer);

	//Draw the entire game scene
	//For top,bottom,left,right borders, should have a different color from the background
	SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 0);

	SDL_DisplayMode cur_dis_mode;
	SDL_GetCurrentDisplayMode(0, &cur_dis_mode);

	//Top border, pos and size
	SDL_Rect wall{
		0,				//Top left x
		0,				//Top left y
		cur_dis_mode.w, //Width
		5				//Height
	};

	//Fills the back buffer with Top border info - game scene
	SDL_RenderFillRect(m_Renderer, &wall);

	//Bottom border, pos and size
	wall.x = 0;
	wall.y = cur_dis_mode.h - 5;
	wall.w = cur_dis_mode.w;
	wall.h = 5;

	//Fills the back buffer with Bottom border info - game scene
	SDL_RenderFillRect(m_Renderer, &wall);

	/*
	//Left border, pos and size
	wall.x = 0;
	wall.y = 0;
	wall.w = 5;
	wall.h = cur_dis_mode.h;

	//Fills the back buffer with left border info - game scene
	SDL_RenderFillRect(m_Renderer, &wall);
	*/

	//Right border, pos and size
	wall.x = cur_dis_mode.w - 5;
	wall.y = 0;
	wall.w = 5;
	wall.h = cur_dis_mode.h;

	//Fills the back buffer with Right border info - game scene
	SDL_RenderFillRect(m_Renderer, &wall);

	SDL_SetRenderDrawColor(m_Renderer, 255, 0, 0, 0);
	SDL_Rect ball{
		static_cast<int>(m_BallPos.x),
		static_cast<int>(m_BallPos.y),
		5,
		5
	};
	//Fills the back buffer with Top border info - game scene
	SDL_RenderFillRect(m_Renderer, &ball);

	SDL_SetRenderDrawColor(m_Renderer, 255, 0, 0, 0);
	SDL_Rect paddle{
		static_cast<int>(m_PaddlePos.x),
		static_cast<int>(m_PaddlePos.y),
		10,
		100
	};
	//Fills the back buffer with Top border info - game scene
	SDL_RenderFillRect(m_Renderer, &paddle);

	//Swap the front and back buffers
	SDL_RenderPresent(m_Renderer);
}
