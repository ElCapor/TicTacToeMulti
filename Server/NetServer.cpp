/**
 * @file NetServer.cpp
 * @author ElCapor (https://github.com/ElCapor)
 * @brief The main server code
 * @version 0.1
 * @date 2025-02-21
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <ErrorMacros.hpp>
#include <Logger.hpp>
#include <NetServer.hpp>


/// CONSTRUCTORS ///

NetServer::NetServer(unsigned short port)
    : net::server_interface<TicMessages>(port) {}

void NetServer::Start()
{
  /**
   * Subscribe to all room events
   */
  for (int i = RoomEvent_PlayerJoined; i < RoomEvent_Max; i++)
  {
    RoomEventManager::getInstance().Subscribe(static_cast<RoomEventEnum>(i), this);
  }

  ServerInterface::Start();
}


/// HELPER FUNCTIONS ///

cpp::result<Player &, int> NetServer::GetPlayerForClient(
    NetworkClient client) {
  if (m_players.find(client->GetID()) == m_players.end()) {
    return cpp::fail(-1);
  }
  return m_players[client->GetID()];
}

cpp::result<NetworkClient&, int> NetServer::GetClientForPlayer(Player player)
{
  std::uint32_t clientID;
  // get key from value in m_players map
  for (auto& keyvaluepair: m_players)
  {
    if (keyvaluepair.second == player)
    {
      clientID = keyvaluepair.first;
    }
  }
  if (clientID)
  {
    for (auto& connection: m_deqConnections)
    {
      if (connection->GetID() == clientID)
      {
        return connection;
      }
    }
  }
  return cpp::fail(-1);
}

cpp::result<Player &, int> NetServer::RegisterPlayer(std::uint32_t id,
                                                     Player player) {
  if (m_players.find(id) != m_players.end()) {
    return cpp::fail(-1);
  }
  m_players[id] = player;
  return m_players[id];
}

cpp::result<int, int> NetServer::UnregisterPlayer(std::uint32_t id) {
  if (m_players.find(id) == m_players.end() || m_players.size() == 0) {
    return cpp::fail(-1);
  }
  return m_players.erase(id);
}

template <typename... Ty>
void NetServer::DisconnectClientWithError(
    NetworkClient client, Ty... args) {
  Logger::Error(args...);
  Send(client, TicMessages_ServerError);
  client->Disconnect();
  OnClientDisconnect(client);
}

template <typename... Args>
void NetServer::Send(NetworkClient client, MessageEnum msgType, Args... data) {
  auto msg = net::message<MessageEnum>();
  msg.header.id = msgType;

  (msg << ... << data);

  client->Send(msg);
}

bool NetServer::Is(ClientOrServerMessage message, MessageEnum msgType) {
  return message.header.id == msgType;
}

ClientMessage NetServer::New(MessageEnum msgType) {
  return net::new_message<MessageEnum>(msgType);
}

void NetServer::Send(NetworkClient client, ClientMessage &message) {
  client->Send(message);
}

/// SERVER FUNCTIONS ///

bool NetServer::OnClientConnect(
    NetworkClient client) {
  Logger::Debug("[SERVER] Client Connected @", client->GetID());
  return true;
}

void NetServer::OnClientDisconnect(
    NetworkClient client) {
  Logger::Info("Client Disconnected @", client->GetID());
  /*
  Remove player from room
  */
  Player player{(int)client->GetID()};
  m_roomManager.RemovePlayerFromRoom(player);
  RESULT_CHECK(UnregisterPlayer(client->GetID()));
}

void NetServer::OnMessage(NetworkClient client,
                          ClientMessage& message) {
  Logger::Info("[SERVER] Message Received @", client->GetID());
  if (Is(message, TicMessages_ClientDisconnect))
  {
    OnClientDisconnect(client);
  }
}

void NetServer::OnClientValidated(NetworkClient client) {
  Logger::Debug("[SERVER] Validated Client @", client->GetID());
  Send(client, TicMessages_ServerAccept);

  Player player{(int)client->GetID()};
  RESULT_CHECK(RegisterPlayer(client->GetID(), player));

  auto room_ret = m_roomManager.AssignNewRoomToPlayer(player);
  if (room_ret.has_error()) {
    DisconnectClientWithError(client, "Failed to assign room to player @",
                              client->GetID());
  }
  auto room = room_ret.value();
  Logger::Info("Assigned room @", room.GetID());
  //Send(client, TicMessages_ServerAssignedRoom, room.GetPlayerCount(), room.GetID());
}


/// EVENTS ///
void NetServer::OnPlayerJoinedRoom(RoomPlayerJoinedEvent *received)
{
  Logger::Info("[SERVER] Player Joined Room @", received->GetRoomID());
  auto ret = m_roomManager.GetRoom(received->GetRoomID());
  if (ret.has_error())
  {
    Logger::Error("Failed to get room @", received->GetRoomID());
    return;
  }
  auto& room = ret.value();
  auto playerClient = GetClientForPlayer({received->GetPlayerID()});
  if (playerClient.has_error())
  {
    Logger::Error("Failed to get client for player @", received->GetPlayerID());
    return;
  }
  Send(playerClient.value(), TicMessages_ServerAssignedRoom, room.GetPlayerCount(), received->GetRoomID());
  auto otherPlayers = room.GetOthersById(received->GetPlayerID());
  for (auto& player : otherPlayers)
  {
    auto client_ret = GetClientForPlayer(player);
    if (client_ret.has_error())
    {
      Logger::Error("Failed to get client for player @", player.id);
      continue;
    }
    auto& client = client_ret.value();
    Send(client, TicMessages_ServerPlayerJoinedRoom, received->GetPlayerID());
  }
}

void NetServer::OnPlayerLeftRoom(RoomPlayerLeftEvent *received)
{
  Logger::Info("[SERVER] Player Left Room @", received->GetRoomID());
  auto ret = m_roomManager.GetRoom(received->GetRoomID());
  if (!ret.has_value())
  {
    Logger::Error("[SERVER] Room not found @", received->GetRoomID());
    return;
  }
  auto& room = ret.value();
  auto otherPlayers = room.GetOthersById(received->GetPlayerID());
  for (auto& otherPlayer : otherPlayers)
  {
    auto client_ret = GetClientForPlayer(otherPlayer);
    if (!client_ret.has_value())
    {
      Logger::Error("[SERVER] Client not found for player @", otherPlayer.id);
      continue;
    }
    auto& client = client_ret.value();
    Send(client, TicMessages_ServerPlayerLeftRoom, received->GetPlayerID());
  }
}

void NetServer::OnRoomFull(RoomRoomFullEvent *received)
{
  Logger::Info("[SERVER] Room Full @", received->GetRoomID());
  auto ret = m_roomManager.GetRoom(received->GetRoomID());
  if (!ret.has_value())
  {
    Logger::Error("[SERVER] Room not found @", received->GetRoomID());
    return;
  }
  auto& room = ret.value();
  room.SetState(RoomState_Locked);
  for (auto& player : room.GetPlayers())
  {
    auto client_ret = GetClientForPlayer(player);
    if (!client_ret.has_value())
    {
      Logger::Error("[SERVER] Client not found for player @", player.id);
      continue;
    }
    auto& client = client_ret.value();
    Send(client, TicMessages_ServerRoomFull);
    asio::io_context ioContext;
    asio::steady_timer timer(ioContext, asio::chrono::seconds(5));
    timer.async_wait([&](const asio::error_code& error) {
      if (!error) {
        Logger::Info("[SERVER] Game Started");
        Send(client, TicMessages_ServerGameStart);
      }
    });
  }
}

void NetServer::OnEvent(RoomEvent *received)
{
  switch (received->m_type)
  {
  case RoomEvent_PlayerJoined:
  OnPlayerJoinedRoom(static_cast<RoomPlayerJoinedEvent*>(received));
    break;
  case RoomEvent_PlayerLeft:
  OnPlayerLeftRoom(static_cast<RoomPlayerLeftEvent*>(received));
    break;
  case RoomEvent_RoomFull:
  OnRoomFull(static_cast<RoomRoomFullEvent*>(received));
    break;
  case RoomEvent_ChangeRoomState:
    break;
  default:
    break;
  }
}
