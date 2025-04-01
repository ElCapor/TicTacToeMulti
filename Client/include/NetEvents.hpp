/**
 * @file NetEvents.hpp
 * @author ElCapor (104172250+ElCapor@users.noreply.github.com)
 * @brief Network Events
 * @version 0.1
 * @date 2025-02-25
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef NETEVENTS_HPP
#define NETEVENTS_HPP

#include <Events.hpp>
#include <Singleton.hpp>

enum NetEvents {
    ServerPing, // server ping
    ConnectionEstablished, // connection was estblished
    ConnectionLost, // connection was lost
    ServerAssignedRoom, // server assigned a room for client
    ServerPlayerJoinedRoom, // a player joined our room
    ServerPlayerLeftRoom, // a player left our room
    RoomFull, // room is full
    ServerGameStarted, // game started
    ServerGameOtherPlayerPlaced, // other player placed a tile
    ServerGameClientTurn, // client turn
    NetEvents_End // for iteration purposes
};

class ServerPingEvent: public Event<NetEvents>
{
    public:
        ServerPingEvent() : Event(ServerPing) {}
};

class ConnectionEstablishedEvent: public Event<NetEvents>
{
    public:
        ConnectionEstablishedEvent() : Event(ConnectionEstablished) {}
};

class ConnectionLostEvent: public Event<NetEvents>
{
    public:
        ConnectionLostEvent() : Event(ConnectionLost) {}
};

class ServerAssignedRoomEvent: public Event<NetEvents>
{
    
    int roomID;
    int playerCount;
    public:
        ServerAssignedRoomEvent() : Event(ServerAssignedRoom) {}
        ServerAssignedRoomEvent(int roomID, int playerCount) : Event(ServerAssignedRoom), roomID(roomID), playerCount(playerCount) {}

        int GetRoomID() const {
            return roomID;
        }

        int GetPlayerCount() const {
            return playerCount;
        }
};

class ServerPlayerJoinedRoomEvent: public Event<NetEvents>
{
    int playerID;
    public:
        ServerPlayerJoinedRoomEvent(int playerID) : Event(ServerPlayerJoinedRoom), playerID(playerID) {}

        int GetPlayerID() const {
            return playerID;
        }
};

class ServerPlayerLeftRoomEvent: public Event<NetEvents>
{
    int playerID;
    public:
        ServerPlayerLeftRoomEvent(int playerID) : Event(ServerPlayerLeftRoom), playerID(playerID) {}

        int GetPlayerID() const {
            return playerID;
        }
};

class RoomFullEvent: public Event<NetEvents>
{
    int roomID;
    public:
        RoomFullEvent() : Event(RoomFull) {}
};

class ServerGameStartedEvent: public Event<NetEvents>
{
    int turn; // if 0 then client else 1 if other player
    public:
        ServerGameStartedEvent(int turn) : Event(ServerGameStarted), turn(turn) {}
    
    int GetTurn() const {
        return turn;
    }
};

class ServerGameOtherPlayerPlacedEvent: public Event<NetEvents>{
    std::pair<int, int> position;
    public:
        ServerGameOtherPlayerPlacedEvent() : Event(ServerGameOtherPlayerPlaced) {}
};

class ServerGameClientTurnEvent: public Event<NetEvents>{
    public:
        ServerGameClientTurnEvent() : Event(ServerGameClientTurn) {}
};

class NetEventsManager : public EventManager<NetEvents>, public Singleton<NetEventsManager>
{
    public:
        NetEventsManager() : EventManager<NetEvents>() {}
};
#endif // NETEVENTS_HPP