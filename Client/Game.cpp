#include <Game.hpp>
#include <raylib.h>



Game::Game(int width = 800, int height = 600, const char* windowTitle = "Game") : m_Width(width), m_Height(height), m_windowTitle(windowTitle)
{
}

void Game::OnLoad()
{
	InitWindow(m_Width, m_Height, m_windowTitle);
	SetTargetFPS(60);


}

void Game::OnExit()
{
	CloseWindow();
}

void Game::OnUpdate()
{
}

void Game::OnDraw()
{
	switch (m_GameState)
	{
		case GameState::WaitingForServer:

	}
}

void Game::Main()
{
	OnLoad();
	while(!WindowShouldClose())
	{
		OnUpdate();
		BeginDrawing();
		ClearBackground(RAYWHITE);
		OnDraw();
		EndDrawing();
	}
	OnExit();
}
