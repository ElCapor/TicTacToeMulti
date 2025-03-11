#include <Game.hpp>
#include <raylib.h>
#include <NetEvents.hpp>

void Game::OnConnectionEstablished(ConnectionEstablishedEvent *event)
{
	TraceLog(LOG_INFO, "Connection Established");
}
void Game::OnServerAssignedRoom(ServerAssignedRoomEvent *event)
{
	TraceLog(LOG_INFO, "Server Assigned Room");
	TraceLog(LOG_INFO, "Room ID: %d", event->GetRoomID());
	TraceLog(LOG_INFO, "Player Count: %d", event->GetPlayerCount());

	m_GameState = GameState::WaitingForPlayers;
}

void Game::OnServerPlayerJoinedRoom(ServerPlayerJoinedRoomEvent *event)
{
	TraceLog(LOG_INFO, "Player Joined Room");
	TraceLog(LOG_INFO, "Player ID: %d", event->GetPlayerID());
}

void Game::OnServerPlayerLeftRoom(ServerPlayerLeftRoomEvent *event)
{
	TraceLog(LOG_INFO, "Player Left Room");
	TraceLog(LOG_INFO, "Player ID: %d", event->GetPlayerID());
}

void Game::OnRoomFull(RoomFullEvent *event)
{
	TraceLog(LOG_INFO, "Room Full");
}

void Game::OnServerGameStarted(ServerGameStartedEvent *event)
{
}

void Game::OnServerGameOtherPlayerPlaced(ServerGameOtherPlayerPlacedEvent *event)
{
}

void Game::OnServerGameClientTurn(ServerGameClientTurnEvent *event)
{
}

void Game::OnConnectionLost(ConnectionLostEvent *event)
{
	TraceLog(LOG_INFO, "Connection Lost");
	m_GameState = GameState::ConnectionLost;
}

void Game::OnEvent(Event<NetEvents> *received)
{
	switch (received->m_type)
	{
	case ConnectionEstablished:
		OnConnectionEstablished(static_cast<ConnectionEstablishedEvent *>(received));
		break;
	case ConnectionLost:
		OnConnectionLost(static_cast<ConnectionLostEvent *>(received));
		break;
	case ServerAssignedRoom:
		OnServerAssignedRoom((ServerAssignedRoomEvent *)(received));
		break;
	case ServerPlayerJoinedRoom:
		OnServerPlayerJoinedRoom((ServerPlayerJoinedRoomEvent *)(received));
		break;
	case ServerPlayerLeftRoom:
		OnServerPlayerLeftRoom((ServerPlayerLeftRoomEvent *)(received));
		break;
	case RoomFull:
		OnRoomFull((RoomFullEvent *)(received));
		break;
	case ServerGameStarted:
	OnServerGameStarted((ServerGameStartedEvent *)(received));
		break;
	case ServerGameOtherPlayerPlaced:
	OnServerGameOtherPlayerPlaced((ServerGameOtherPlayerPlacedEvent *)(received));
		break;
	case ServerGameClientTurn:
	OnServerGameClientTurn((ServerGameClientTurnEvent *)(received));
		break;
	}
}

/* Draw Functions*/
void DrawWaitingForServer()
{
	DrawText("Waiting for connection with server...", GetScreenWidth() / 2, GetScreenHeight() / 2, 20, RED);
}

void DrawWaitingForPlayers()
{
	DrawText("Waiting for players...", GetScreenWidth() / 2, GetScreenHeight() / 2, 20, RED);
}

void DrawConnectionLost()
{
	DrawText("Connection Lost", GetScreenWidth() / 2, GetScreenHeight() / 2, 20, RED);
}

/*Game Class*/

Game::Game(int width = 800, int height = 600, const char *windowTitle = "Game")
	: m_Width(width), m_Height(height), m_windowTitle(windowTitle), m_GameState(GameState::WaitingForServer)
{
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
		NetEventsManager::getInstance().Subscribe(static_cast<NetEvents>(i), this);
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
	case GameState::WaitingForPlayers:
		DrawWaitingForPlayers();
		break;
	case GameState::ConnectionLost:
		DrawConnectionLost();
		break;
	}
}

void Game::Main()
{
	OnLoad();
	while (!WindowShouldClose())
	{
		OnUpdate();
		BeginDrawing();
		ClearBackground(RAYWHITE);
		OnDraw();
		EndDrawing();
	}
	OnExit();
}
