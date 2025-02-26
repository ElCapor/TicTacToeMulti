#include <NetClient.hpp>
#include <Network.h>
#include <NetEvents.hpp>

class internal_client : public net::client_interface<TicMessages>
{
	friend class NetClient;
	const char* host;
	unsigned short port;
public:
	internal_client(const char* host = "127.0.0.1", unsigned short port = 62323) :host(host), port(port) {

	}
	~internal_client() = default;
};

NetClient::NetClient()
{
	this->client = new internal_client();
}

void NetClient::ConnectToServer()
{
	client->Connect("127.0.0.1", 62323);
}

void NetClient::DisconnectFromServer()
{
	auto msg = net::new_message<TicMessages>(TicMessages::TicMessages_ClientDisconnect);
	client->Send(msg);
	client->Disconnect();
}

void NetClient::Update()
{
	if (client->IsConnected())
	{
		if (!client->Incoming().empty())
		{
			auto msg = client->Incoming().pop_front();
			if (msg.msg.header.id == TicMessages::TicMessages_ServerAccept)
			{
				ConnectionEstablishedEvent event;
				NetEventsManager::getInstance().SendEvent(&event);
			} else if (msg.msg.header.id == TicMessages::TicMessages_ServerError)
			{
				ConnectionLostEvent event;
				NetEventsManager::getInstance().SendEvent(&event);
			} else if (msg.msg.header.id == TicMessages::TicMessages_ServerAssignedRoom)
			{
				auto data = msg.msg;
				int playerCount;
				int roomID;
				data >> roomID;
				data >> playerCount;
				ServerAssignedRoomEvent* event = new ServerAssignedRoomEvent(roomID, playerCount);
				std::cout << "ROOM ID " << event->GetRoomID() << std::endl;
				// This works fine, so i have 0 idea why the event listener doesnt work
				NetEventsManager::getInstance().SendEvent(event);
			}
		}
	}
	
}

#include <algorithm>

template<typename ...Args>
inline void NetClient::Send(TicMessages msgType, Args... data)
{
    auto msg = net::message<TicMessages>();
    msg.header.id = msgType;

    (msg << ... << data);

    client->Send(msg);
}
