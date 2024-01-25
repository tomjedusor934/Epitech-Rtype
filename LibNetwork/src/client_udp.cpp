/*
** EPITECH PROJECT, 2023
** net-framework
** File description:
** client_udp
*/

#include <iostream>
#include "../udp/NetClient.hpp"
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

class CustomClient : public rtype::net::udp::NetClient<CustomMsgTypes>
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
			std::cout << "MoveUp" << std::endl;

			int x = 12;
            // t_player pos;
            // pos.id = 1;
            // pos.x = 1;
            // msg << pos;
			msg << x;
            Send(msg);
        }
};

int main(void)
{
    CustomClient c;
    c.Connect("127.0.0.1", 60000);


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