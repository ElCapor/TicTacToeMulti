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
    ConnectionEstablished, // connection was estblished
    ConnectionLost, // connection was lost
    ServerAssignedRoom, // server assigned a room for client
    RoomFull, // room is full
    ServerGameStarted, // game started
    ServerGameOtherPlayerPlaced, // other player placed a tile
    ServerGameClientTurn, // client turn

    NetEvents_End // for iteration purposes
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
        ServerGameStartedEvent() : Event(ServerGameStarted) {}
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