/**
 * @file Game.hpp
 * @author ElCapor (https://github.com/ElCapor)
 * @brief Base Game Class
 * @version 0.1
 * @date 2025-02-22
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef GAME_H
#define GAME_H
#include <memory>
#include <GameState.hpp>
#include <NetClient.hpp>
#include <Events.hpp>
#include <NetEvents.hpp>

class NetClient;

class Game : public EventListener<NetEvents>
{
int m_Width;
int m_Height;
const char* m_windowTitle;

GameState m_GameState;
NetClient* m_netclient;
public:
    Game(int width, int height, const char* windowTitle);
    void OnLoad();
    void OnExit();
    void OnUpdate();
    void OnDraw();
    void Main();
public:
    void OnEvent(Event<NetEvents> *received) override;

private:
    void OnServerPing(ServerPingEvent *event);
    void OnConnectionEstablished(ConnectionEstablishedEvent *event);
    void OnConnectionLost(ConnectionLostEvent *event);
    void OnServerAssignedRoom(ServerAssignedRoomEvent *event);
    void OnServerPlayerJoinedRoom(ServerPlayerJoinedRoomEvent *event);
    void OnServerPlayerLeftRoom(ServerPlayerLeftRoomEvent *event);
    void OnRoomFull(RoomFullEvent *event);
    void OnServerGameStarted(ServerGameStartedEvent *event);
    void OnServerGameOtherPlayerPlaced(ServerGameOtherPlayerPlacedEvent *event);
    void OnServerGameClientTurn(ServerGameClientTurnEvent *event);
};

#endif // GAME_H