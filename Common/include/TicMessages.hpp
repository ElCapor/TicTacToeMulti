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
	ServerAccept, // server accepted your connection
	
	ServerPing, // server is pinging you
	ClientPing, // Client replying back


};

#endif // TIC_MESSAGES_