#include <NetClient.hpp>
#include <Network.h>

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

inline void NetClient::ConnectToServer()
{
	client->Connect("127.0.0.1", 62323);
}

inline void NetClient::DisconnectFromServer()
{
	client->Disconnect();
}

inline void NetClient::Update()
{
	if (client->IsConnected())
	{
		if (!client->Incoming().empty())
		{
			auto msg = client->Incoming().pop_front();
			
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
