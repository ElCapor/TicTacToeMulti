#include <Game.hpp>
#include <raylib.h>
#include <NetEvents.hpp>


/* Event Listener*/
class GameEventListener: public EventListener<NetEvents>
{
	void OnConnectionEstablished(ConnectionEstablishedEvent *event)
	{
		TraceLog(LOG_INFO, "Connection Established");
	}
public:
	void OnEvent(Event<NetEvents> *received) override
	{
		switch (received->m_type)
		{
			case ConnectionEstablished:
				OnConnectionEstablished(static_cast<ConnectionEstablishedEvent*>(received));
				break;
			case ConnectionLost:
				break;
			case ServerAssignedRoom:
				break;
			case RoomFull:
				break;
			case ServerGameStarted:
				break;
			case ServerGameOtherPlayerPlaced:
				break;
			case ServerGameClientTurn:
				break;
		}
	}
};

/* Draw Functions*/
void DrawWaitingForServer()
{
	DrawText("Waiting for connection with server...", GetScreenWidth() / 2, GetScreenHeight() / 2, 20, RED);
}


/*Game Class*/

Game::Game(int width = 800, int height = 600, const char* windowTitle = "Game") 
: m_Width(width), m_Height(height), m_windowTitle(windowTitle), m_GameState(GameState::WaitingForServer)
{
	m_eventListener = new GameEventListener();
	m_netclient = new NetClient();
}

void Game::OnLoad()
{
	InitWindow(m_Width, m_Height, m_windowTitle);
	SetTargetFPS(60);

	// iterate over all the events in NetEvents enum
	for (int i = NetEvents::ConnectionEstablished; i < NetEvents::NetEvents_End; i++)
	{
		// subscribe to the event
		NetEventsManager::getInstance().Subscribe(static_cast<NetEvents>(i), m_eventListener);
	}

	m_netclient->ConnectToServer();

}

void Game::OnExit()
{
	m_netclient->DisconnectFromServer();
	CloseWindow();
}

void Game::OnUpdate()
{
	m_netclient->Update();
}

void Game::OnDraw()
{
	switch (m_GameState)
	{
		case GameState::WaitingForServer:
			DrawWaitingForServer();
			break;
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
