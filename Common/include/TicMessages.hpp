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
#define TIC_MESSAGES_H

enum TicMessages {
  TicMessages_ServerAccept, // server accepted your connection
  TicMessages_ServerError,  // server encountered an error

  TicMessages_ServerAssignedRoom, // server assigned a room for client

  TicMessages_ServerPing, // server is pinging you
  TicMessages_ClientPing, // Client replying back

  TicMessages_ClientDisconnect, // client disconnected
  TicMessages_ServerDisconnect, // server is going offline

  TicMessages_ServerPlayerJoinedRoom, // Sent to other players when a player
                                      // joins their room
  TicMessages_ServerPlayerLeftRoom,   // Sent to other players when a player
                                      // leaves their room
  TicMessages_ServerRoomFull, // Sent to all players when their room is full

  TicMessages_ClientRequestRoomInfo, // Client requests information about his room
  TicMessages_ServerRoomInfo,    // Server responds with information about room
  
  TicMessages_ServerGameStart, // Server decided that we should start
  // After game start , game is handled by RoomGame class

  // RoomGame
  TicMessages_ClientMove, // Client requests to make a move

  TicMessages_ServerMove, // Server replies with the move and replicate to other players in room

  TicMessages_MaxRoomGameMessages
  // End of RoomGame messages

};

#endif // TIC_MESSAGES_
