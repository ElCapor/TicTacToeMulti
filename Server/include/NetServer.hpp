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
#include <Player.hpp>
#include <RoomManager.hpp>
#include <TicMessages.hpp>
#include <unordered_map>
#include <Events.hpp>

using MessageEnum = TicMessages;
using Connection = net::connection<MessageEnum>;
using client_type = std::shared_ptr<Connection>;

using NetworkClient = client_type;
using ServerInterface = net::server_interface<MessageEnum>;

/** Server -> Client Messages **/
using ServerMessage = net::message<MessageEnum>;

/** Client -> Server Message **/
using ClientMessage = net::message<MessageEnum>;

/** Client <-> Server Message **/
using ClientOrServerMessage = net::message<MessageEnum>;

using PlayerRoom = Room<Player>;

class NetServer : public ServerInterface, public EventListener<RoomEventEnum>
{
/// MEMBERS ///
  RoomManager m_roomManager;
  std::unordered_map<std::uint32_t, Player> m_players;

/// CONSTRUCTORS ///
public:
  NetServer(unsigned short port);

/// HELPER FUNCTIONS ///
protected:
  cpp::result<Player &, int> GetPlayerForClient(NetworkClient client);
  cpp::result<NetworkClient&, int> GetClientForPlayer(Player player);
  
  cpp::result<Player &, int> RegisterPlayer(std::uint32_t id, Player player);
  cpp::result<int, int> UnregisterPlayer(std::uint32_t id);
  template <typename... Ty>
  void DisconnectClientWithError(NetworkClient client, Ty... args);

  template <typename... Args>
  void Send(NetworkClient client, MessageEnum msgType, Args...);

  ClientMessage New(MessageEnum msgType);
  void Send(NetworkClient client, ClientMessage &message);


  bool Is(ClientOrServerMessage message, MessageEnum msgType);
/// EVENTS ///
protected:
  void OnPlayerJoinedRoom(RoomPlayerJoinedEvent *received);
  void OnPlayerLeftRoom(RoomPlayerLeftEvent *received);
  void OnRoomFull(RoomRoomFullEvent *received);

/// SERVER FUNCTIONS ///
public:
  bool OnClientConnect(NetworkClient client) override;
  void OnClientDisconnect(NetworkClient client) override;
  void OnMessage(NetworkClient client, ClientMessage &message) override;
  void OnClientValidated(NetworkClient client) override;
  void OnEvent(RoomEvent *received) override;
  void Start();

};

#endif // NET_SERVER_H
