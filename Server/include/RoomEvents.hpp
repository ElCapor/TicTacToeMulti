/**
 * @file RoomEvents.hpp
 * @author ElCapor (https://github.com/ElCapor)
 * @brief Room Events
 * @version 0.1
 * @date 2025-02-21
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef ROOM_EVENTS_HPP
#define ROOM_EVENTS_HPP
#include <Events.hpp>
#include <Singleton.hpp>

enum RoomEventEnum
{
    RoomEvent_PlayerJoined,
    RoomEvent_PlayerLeft,
    RoomEvent_RoomFull,
    RoomEvent_ChangeRoomState,
    RoomEvent_Max
};

using RoomEvent = Event<RoomEventEnum>;

class RoomPlayerJoinedEvent : public RoomEvent
{
    int playerID;
    int roomID;
public:
    RoomPlayerJoinedEvent(int playerID, int roomID) : RoomEvent(RoomEvent_PlayerJoined), playerID(playerID), roomID(roomID) {}

    int GetPlayerID() const
    {
        return playerID;
    }

    int GetRoomID() const
    {
        return roomID;
    }
};

class RoomPlayerLeftEvent : public RoomEvent
{
    int playerID;
    int roomID;
public:
    RoomPlayerLeftEvent(int playerID, int roomID) : RoomEvent(RoomEvent_PlayerLeft), playerID(playerID), roomID(roomID) {}

    int GetPlayerID() const
    {
        return playerID;
    }

    int GetRoomID() const
    {
        return roomID;
    }
};

class RoomRoomFullEvent : public RoomEvent
{
    int roomID;
public:
    RoomRoomFullEvent(int roomID) : RoomEvent(RoomEvent_RoomFull), roomID(roomID) {}

    int GetRoomID() const
    {
        return roomID;
    }
};

class RoomEventManager : public EventManager<RoomEventEnum>, public Singleton<RoomEventManager>
{
public:
    RoomEventManager() : EventManager<RoomEventEnum>()
    {
    }
};

#endif