/*
** EPITECH PROJECT, 2023
** area_network_framework
** File description:
** client
*/

#include <iostream>
#include "../tcp/NetClient.hpp"
#include <ncurses.h>
// #include <unistd.h>

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

class CustomClient : public rtype::net::tcp::NetClient<CustomMsgTypes>
{
public:
	void PingServer()
	{
		rtype::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::ServerPing;

		// Caution with this...
		std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();

		msg << timeNow;
		std::cout << "PingServer\n";
		Send(msg);
	}

	void MessageAll()
	{
		rtype::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::MessageAll;
		Send(msg);
	}

	void MoveUp()
	{
		// MOVE IN 3d
		rtype::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::moveUp;

		// t_player pos;
		// pos.id = 1;
		// pos.x = 234;
		int toto = 23;
		msg << toto;
		// char mot[] = "toto";
		// char str[] = "totoammzaz";
		// msg << str << toto << pos << mot;
		// std::vector<int> vec = {1, 2};
		// std::cout << "msg body: " << msg.body.size() << std::endl;
		// std::cout << "msg size: " << msg.size() << std::endl;
		// std::cout << "body content: " << msg.body[0] << std::endl;
		// for (int x = 0; x < 2; x++) {
		// 	msg << vec.data()[x];
		// }
		Send(msg);
	}
};

int main()
{
	// initscr();
	timeout(0);
	CustomClient c;
	c.Connect("127.0.0.1", 60000);

	bool key[3] = {false, false, false};
	bool old_key[3] = {false, false, false};

	bool bQuit = false;
	while (bQuit == false) {
		std::cout << "c.IsConnected(): " << c.IsConnected() << std::endl;
		if (c.IsConnected()) {
			std::cout << !c.Incoming().empty() << std::endl;
			if (!c.Incoming().empty()) {
				auto msg = c.Incoming().pop_front().msg;
				switch (msg.header.id) {
					case CustomMsgTypes::ServerAccept: {
						std::cout << "Server Accepted Connection\n";
					}
					break;
					case CustomMsgTypes::ServerPing: {
						std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
						std::chrono::system_clock::time_point timeThen;
						msg >> timeThen;
						std::cout << "Ping: " << std::chrono::duration<double>(timeNow - timeThen).count() << "\n";
					}
					break;
					case CustomMsgTypes::ServerMessage: {
						uint32_t clientID;

						msg >> clientID;
						std::cout << "Hello from [" << clientID << "]\n";
					}
					break;
					case CustomMsgTypes::ServerMessage2: {
						uint32_t clientID;
						std::string mot;
						msg >> clientID;
						std::cout << "Hello from [" << clientID << mot << "]\n";
					}
					break;
				}
			}
			std::string userInput;
    		std::getline(std::cin, userInput);
			std::cout << "userInput: " << userInput << std::endl;
			if (userInput == "1") {
				std::cout << "ping server" << std::endl;
				c.PingServer();
			} else if (userInput == "2") {
				c.MoveUp();
			} else if (userInput == "3") {
				bQuit = true;
			}
		} else {
			std::cout << "Server Down\n";
			bQuit = true;
		}
	}
	return 0;
}
