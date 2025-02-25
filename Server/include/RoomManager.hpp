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

class RoomManager
{
    std::vector<Room<Player>> m_rooms;
    int maxRoomIndex = -1;
    int maxRoomSize;
public:
    RoomManager(int maxRoomSize = 2) : maxRoomSize(maxRoomSize)
    {

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
            if (room.GetPlayer().has_value() && room.GetPlayer().value() == player)
        }
    }
};

#endif // ROOM_MANAGER_HPP