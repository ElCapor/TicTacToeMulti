/**
 * @file NetClient.hpp
 * @author ElCapor (https://github.com/ElCapor)
 * @brief Client network interface
 * @version 0.1
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef NET_CLIENT_H
#define NET_CLIENT_H
#include <TicMessages.hpp>

class internal_client;

class NetClient
{
	internal_client* client;
public:
	NetClient();

	void ConnectToServer();

	void DisconnectFromServer();

	void Update();

	template <typename ...Args>
	void Send(TicMessages msgType, Args...);
};

#endif // NET_CLIENT_H

