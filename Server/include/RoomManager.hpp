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
#include <Room.hpp>
#include <Player.hpp>
#include <vector>
#include <Logger.hpp>

class RoomManager
{
    std::vector<Room<Player>> m_rooms;
    int maxRoomIndex = -1;
    int maxRoomSize;
public:
    RoomManager(int maxRoomSize = 2) : maxRoomSize(maxRoomSize)
    {
        CreateRoom();
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
            CreateRoom();
            AssignNewRoomToPlayer(player); // retry assigning a room in case there was not any available
        } else{
            m_rooms[idx].AddPlayer(player);
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
        room.RemovePlayer(player);

        // Garbage Collector lol
        if (room.IsEmpty())
        {
            m_rooms.erase(std::remove_if(m_rooms.begin(), m_rooms.end(), [&room](Room<Player>& r){ return &r == &room; }), m_rooms.end());
            maxRoomIndex = m_rooms.size() - 1;
        }
    }
};

#endif // ROOM_MANAGER_HPP