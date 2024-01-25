/*
** EPITECH PROJECT, 2023
** area_network_framework
** File description:
** server
*/

#include <iostream>
#include "../tcp/NetServer.hpp"
#include "../tcp/NetConnection.hpp"

enum class CustomMsgTypes : uint32_t
{
	ServerAccept,
	ServerDeny,
	ServerPing,
	MessageAll,
	ServerMessage,
	moveUp,
	ServerMessage2,
};

typedef struct s_player
{
	int id;
	int x;
} t_player;

class CustomServer : public rtype::net::tcp::NetServer<CustomMsgTypes>
{
public:
	CustomServer(uint16_t nPort) : rtype::net::tcp::NetServer<CustomMsgTypes>(nPort)
	{

	}

protected:
	bool OnClientConnect(std::shared_ptr<rtype::net::AConnection<CustomMsgTypes>> client) override
	{
		rtype::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::ServerAccept;
		client->Send(msg);
		return true;
	}

	// Called when a client appears to have disconnected
	void OnClientDisconnect(std::shared_ptr<rtype::net::AConnection<CustomMsgTypes>> client) override
	{
		std::cout << "Removing client [" << client->GetID() << "]\n";
		for (auto& c : m_deqConnections)
		{
			if (c->GetID() == client->GetID())
			{
				c->Disconnect();
				break;
			}
		}
	}

	// Called when a message arrives
	void OnMessage(std::shared_ptr<rtype::net::AConnection<CustomMsgTypes>> client, rtype::net::message<CustomMsgTypes>& msg) override
	{
		std::cout << "[" << client->GetID() << "]: ";
		switch (msg.header.id)
		{
		case CustomMsgTypes::ServerPing:
		{
			std::cout << "[" << client->GetID() << "]: Server Ping\n";

			// Simply bounce message back to client
			client->Send(msg);
		}
		break;

		case CustomMsgTypes::MessageAll:
		{
			std::cout << "[" << client->GetID() << "]: Message All\n";

			// Construct a new message and send it to all clients
			rtype::net::message<CustomMsgTypes> msg;
			msg.header.id = CustomMsgTypes::ServerMessage;
			msg << client->GetID();
			MessageAllClients(msg, client);

		}
		break;

		case CustomMsgTypes::moveUp:
		{
			std::cout << "[" << client->GetID() << "]: moveUp\n";

			// Construct a new message and send it to all clients
			std::cout << msg.size() << std::endl;
			// std vector of size 3
			t_player pos;
			int i = 0;
			char toto[5];
			char toto2[10];
			msg  >> toto >> pos >> i;
			std::vector<int> str;
			str.reserve(2);
			str = {0, 0};
			// msg >> toto2;
			msg >> str.data()[1] >> str.data()[0];
			std::cout << "str[0]: " << str[0] << ", " << str[1] << std::endl;

			// std::cout << "i: " << i << std::endl;
			// std::cout << "pos[0]: " << pos.id << " pos[1]: " << pos.x << std::endl;
			// std::cout << "toto: " << toto << std::endl;
			// std::cout << "toto2: " << toto2 << std::endl;
			rtype::net::message<CustomMsgTypes> newmsg;
			newmsg.header.id = CustomMsgTypes::ServerMessage2;
			newmsg << client->GetID();
			MessageAllClients(newmsg, client);
		}
		break;
		}
	}
};

int main()
{
	CustomServer server(60000);
	server.Start();

	while (1)
	{
		server.Update(-1, true);
	}
	return 0;
}