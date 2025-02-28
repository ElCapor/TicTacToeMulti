/**
 * @file TicMessages.hpp
 * @author ElCapor (https://github.com/ElCapor)
 * @brief Tic Tac Toe Network Messages definition
 * @version 0.1
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef TIC_MESSAGES_H
#define TIC_MESSAGES_


enum TicMessages {
	TicMessages_ServerAccept, // server accepted your connection
	TicMessages_ServerError, // server encountered an error

	TicMessages_ServerAssignedRoom, // server assigned a room for client

	TicMessages_ServerPing, // server is pinging you
	TicMessages_ClientPing, // Client replying back


	TicMessages_ClientDisconnect, // client disconnected
	TicMessages_ServerDisconnect, // server is going offline

	TicMessages_ServerPlayerJoined, // Sent to other players when a player joins a rooms

};

#endif // TIC_MESSAGES_