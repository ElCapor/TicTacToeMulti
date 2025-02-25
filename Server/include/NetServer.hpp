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

class NetServer : public net::server_interface<TicMessages>
{
public:
    NetServer(unsigned short port) : net::server_interface<TicMessages>(port)
    {

    }

protected:
    bool OnClientConnect(std::shared_ptr<net::connection<TicMessages>> client) override
    {
        Logger::Debug(false, "[SERVER] Client Connected @", client->GetID());
        auto msg = net::new_message<TicMessages>(TicMessages::ServerAccept);
        client->Send(msg);
        return true;
    }

    void OnClientDisconnect(std::shared_ptr<net::connection<TicMessages>> client) override
    {

    }

    void OnMessage(std::shared_ptr<net::connection<TicMessages>> client, net::message<TicMessages>& message)
    {
        
    }

    void OnClientValidated(std::shared_ptr<net::connection<TicMessages>> client)
    {
        Logger::Debug(false, "[SERVER] Validated Client @", client->GetID());
    }



};

#endif // NET_SERVER_H