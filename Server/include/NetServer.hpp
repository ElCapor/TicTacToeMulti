/**
 * @file NetServer.hpp
 * @author ElCapor (https://github.com/ElCapor)
 * @brief The main server class
 * @version 0.1
 * @date 2025-02-21
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef NET_SERVER_H
#define NET_SERVER_H
#include <Network.h>
#include <TicMessages.hpp>
#include <Logger.hpp>
#include <RoomManager.hpp>
#include <Player.hpp>
#include <unordered_map>

class NetServer : public net::server_interface<TicMessages>
{
    RoomManager m_roomManager;

    std::unordered_map<std::uint32_t, Player> m_players;
public:
    NetServer(unsigned short port) : net::server_interface<TicMessages>(port)
    {
    }

protected:
    cpp::result<Player&, int> GetPlayerForClient(std::shared_ptr<net::connection<TicMessages>> client)
    {
        if (m_players.find(client->GetID()) == m_players.end())
        {
            return cpp::fail(-1);
        }
        return m_players[client->GetID()];
    }

    cpp::result<Player&, int> RegisterPlayer(std::uint32_t id, Player player)
    {
        if (m_players.find(id) != m_players.end())
        {
            return cpp::fail(-1);
        }
        m_players[id] = player;
        return m_players[id];
    }

    cpp::result<void, int> UnregisterPlayer(std::uint32_t id)
    {
        if (m_players.find(id) == m_players.end())
        {
            return cpp::fail(-1);
        }
        m_players.erase(id);
    }


    template <typename... Ty>
    void DisconnectClientWithError(std::shared_ptr<net::connection<TicMessages>> client , Ty... args)
    {
        Logger::Error(args...);
        auto msg = net::new_message<TicMessages>(TicMessages::TicMessages_ServerError);
        client->Send(msg);
        client->Disconnect();
        OnClientDisconnect(client);
    }

    bool OnClientConnect(std::shared_ptr<net::connection<TicMessages>> client) override
    {
        Logger::Debug(false, "[SERVER] Client Connected @", client->GetID());
        return true;
    }

    void OnClientDisconnect(std::shared_ptr<net::connection<TicMessages>> client) override
    {
        Logger::Info("Client Disconnected @", client->GetID());
        /*
        Remove player from room
        */
        Player player{(int)client->GetID()};
        auto room_ret = m_roomManager.GetRoomByPlayer(player);
        if (room_ret.has_error())
        {
            Logger::Error("Failed to get room by player @", client->GetID());
            return;
        }
        Logger::Info("Removing player from room @", room_ret.value().GetID());
        m_roomManager.RemovePlayerFromRoom(player);

    }

    void OnMessage(std::shared_ptr<net::connection<TicMessages>> client, net::message<TicMessages> &message)
    {
        if (message.header.id == TicMessages::TicMessages_ClientDisconnect)
        {
            OnClientDisconnect(client);
        }
    }

    void OnClientValidated(std::shared_ptr<net::connection<TicMessages>> client)
    {
        Logger::Debug(false, "[SERVER] Validated Client @", client->GetID());
        auto msg = net::new_message<TicMessages>(TicMessages::TicMessages_ServerAccept);
        client->Send(msg);

        Player player{(int)client->GetID()};
        RegisterPlayer(client->GetID(), player);    

        auto room_ret = m_roomManager.AssignNewRoomToPlayer(player);
        if (room_ret.has_error())
        {
            DisconnectClientWithError(client, "Failed to assign room to player @", client->GetID());
           
        }
        auto room = room_ret.value();
        Logger::Info("Assigned room @", room.GetID());
        msg = net::new_message<TicMessages>(TicMessages::TicMessages_ServerAssignedRoom);
        msg << room.GetPlayerCount();
        msg << room.GetID();
        client->Send(msg);
    }
};

#endif // NET_SERVER_H