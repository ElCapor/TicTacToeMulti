#include <NetClient.hpp>
#include <NetEvents.hpp>
#include <Network.h>

/** Server -> Client Messages **/
using ServerMessage = net::message<TicMessages>;

/** Client -> Server Message **/
using ClientMessage = net::message<TicMessages>;

/** Client <-> Server Message **/
using ClientOrServerMessage = net::message<TicMessages>;


bool Is(ClientOrServerMessage message, TicMessages msgType)
{
  return message.header.id == msgType;
}


class internal_client : public net::client_interface<TicMessages> {
  friend class NetClient;
  const char *host;
  unsigned short port;

public:
  internal_client(const char *host = "127.0.0.1", unsigned short port = 62323)
      : host(host), port(port) {}
  ~internal_client() = default;
};

NetClient::NetClient() { this->client = new internal_client(); }

void NetClient::ConnectToServer() { client->Connect("127.0.0.1", 62323); }

void NetClient::DisconnectFromServer() {
  Send(TicMessages_ClientDisconnect);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  client->Disconnect();
}

void NetClient::Update() {
  if (client->IsConnected()) {
    if (!client->Incoming().empty()) {
      auto msg = client->Incoming().pop_front();
      if (Is(msg.msg, TicMessages_ServerAccept)) {
        ConnectionEstablishedEvent event;
        NetEventsManager::getInstance().SendEvent(&event);
      } else if (Is(msg.msg, TicMessages_ServerError)) {
        ConnectionLostEvent event;
        NetEventsManager::getInstance().SendEvent(&event);
      } else if (Is(msg.msg, TicMessages_ServerAssignedRoom)) {
        auto data = msg.msg;
        int playerCount;
        int roomID;
        data >> roomID;
        data >> playerCount;
        ServerAssignedRoomEvent *event =
            new ServerAssignedRoomEvent(roomID, playerCount);
        std::cout << "ROOM ID " << event->GetRoomID() << std::endl;
        // This works fine, so i have 0 idea why the event listener doesnt work
        NetEventsManager::getInstance().SendEvent(event);
      } else if (Is(msg.msg, TicMessages_ServerPlayerJoinedRoom)) {
        auto data = msg.msg;
        int playerID;
        data >> playerID;
        ServerPlayerJoinedRoomEvent *event =
            new ServerPlayerJoinedRoomEvent(playerID);
        NetEventsManager::getInstance().SendEvent(event);
      } else if (Is(msg.msg, TicMessages_ServerPlayerLeftRoom)) {
        auto data = msg.msg;
        int playerID;
        data >> playerID;
        ServerPlayerLeftRoomEvent *event =
            new ServerPlayerLeftRoomEvent(playerID);
        NetEventsManager::getInstance().SendEvent(event);
      } else if (Is(msg.msg, TicMessages_ServerRoomFull)) {
        auto data = msg.msg;
        RoomFullEvent *event = new RoomFullEvent();
        NetEventsManager::getInstance().SendEvent(event);
      } else if (Is(msg.msg, TicMessages_ServerGameStart)) {
        auto data = msg.msg;
        int turn;
        data >> turn;
        ServerGameStartedEvent *event = new ServerGameStartedEvent(turn);
        NetEventsManager::getInstance().SendEvent(event);
      }
    }
  }
}

template <typename... Args>
inline void NetClient::Send(TicMessages msgType, Args... data) {
  if (!client->IsConnected())
    std::cout << "Client not connected" << std::endl;
  auto msg = net::message<TicMessages>();
  msg.header.id = msgType;

  (msg << ... << data);

  client->Send(msg);
}
