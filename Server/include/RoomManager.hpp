/**
 * @file RoomManager.hpp
 * @author ElCapor (104172250+ElCapor@users.noreply.github.com)
 * @brief System to manage matchmaking rooms serversided
 * @version 0.1
 * @date 2025-02-25
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef ROOM_MANAGER_HPP
#define ROOM_MANAGER_HPP
#include <RoomGame.hpp>
#include <Player.hpp>
#include <vector>
#include <Logger.hpp>
#include <RoomEvents.hpp>
#include <ErrorMacros.hpp>

class RoomManager
{
    std::vector<RoomGame<Player>> m_rooms;
    int maxRoomIndex = -1;
    int maxRoomSize;
public:
    RoomManager(int maxRoomSize = 2) : maxRoomSize(maxRoomSize)
    {
        RESULT_CHECK(CreateRoom());
    }

    int GetAvailableRoomIndex()
    {
        for (int i = 0; i < m_rooms.size(); i++)
        {
            if (!m_rooms[i].IsFull() && m_rooms[i].IsPublic())
            {
                return i;
            }
        }
        return -1;
    }

    cpp::result<Room<Player>&, int> CreateRoom()
    {
        maxRoomIndex++;
        m_rooms.emplace_back(maxRoomIndex, maxRoomSize);
        return GetRoom(maxRoomIndex);
    }

    cpp::result<Room<Player>&, int> GetRoom(int roomID)
    {
        for (auto& room : m_rooms)
        {
            if (room.GetID() == roomID)
            {
                return room;
            }
        }
        return cpp::fail(-1);
    }

    cpp::result<Room<Player>&, int> GetRoomByPlayer(Player player)
    {
        for (auto& room : m_rooms)
        {
            if (room.GetPlayer(player.id).has_value())
            {
                return room;
            }
        }
        return cpp::fail(-1);
    }

    cpp::result<Room<Player>&, int> AssignNewRoomToPlayer(Player player)
    {
        int idx = GetAvailableRoomIndex();
        if (idx == -1)
        {
            RESULT_CHECK(CreateRoom());
            return AssignNewRoomToPlayer(player); // retry assigning a room in case there was not any available
        } else{
            m_rooms[idx].AddPlayer(player);
            RoomEventManager::getInstance().SendEvent(new RoomPlayerJoinedEvent(player.id, m_rooms[idx].GetID()));
            if (m_rooms[idx].IsFull())
            {
                RoomEventManager::getInstance().SendEvent(new RoomRoomFullEvent(m_rooms[idx].GetID()));
            }
            return m_rooms[idx];
        }
        return cpp::fail(-1);
    }

    void RemovePlayerFromRoom(Player player)
    {
        auto ret = GetRoomByPlayer(player);
        if (!ret.has_value())
        {
            Logger::Error("Attempt to remove player ", player.id, " But he was not found...");
            return;
        }

        Room<Player>& room = ret.value();
        int oldSize = room.GetPlayerCount();
        room.RemovePlayer(player);
        if (oldSize == room.GetPlayerCount())
        {
            Logger::Error("Remove Player Failed for : ", player.id);
            return;
        }
        RoomEventManager::getInstance().SendEvent(new RoomPlayerLeftEvent(player.id, room.GetID()));

        // Garbage Collector lol
        if (room.IsEmpty())
        {
            m_rooms.erase(std::remove_if(m_rooms.begin(), m_rooms.end(), [&room](Room<Player>& r){ return &r == &room; }), m_rooms.end());
            maxRoomIndex = m_rooms.size() - 1;
        }
    }
};

#endif // ROOM_MANAGER_HPP