/*
** EPITECH PROJECT, 2023
** RType
** File description:
** custom_server
*/

#include "../tcp/NetServer.hpp"
#include "../udp/NetServer.hpp"
#include "../tcp/NetClient.hpp"

//* this class is used for the communication between the main server and the clients
//* we use the tcp protocol because it's a reliable protocol
//* so we are sure that the messages will be received by the clients like for th e login and the register or any other important messages

enum class MainServerMsgTypes : uint32_t
{
};

class CustomTcpMainServer : public rtype::net::tcp::NetServer<MainServerMsgTypes>
{
};


//* this class is used for the communication between the game servers and the clients
//* we use the udp protocol because it's a fast protocol so we are sure that the messages will be received by the clients like for the game

enum class GameServerMsgTypes : uint32_t
{
};

class CustomUdpGameServer : public rtype::net::udp::NetServer<GameServerMsgTypes>
{
};


//* this two class is used for the communication between the main server and the game servers
//* the main server has a second server tcp dedicated to the communication with the game servers
//* the game servers have a client tcp dedicated to the communication with the main server

enum class CustomCommunicationServer : uint32_t
{
};

class CustomTcpServerServer : public rtype::net::tcp::NetServer<CustomCommunicationServer>
{
};

class CustomTcpServerAsClient : public rtype::net::tcp::NetClient<CustomCommunicationServer>
{
};

//? je pense qu'il serait intelligent de lancer les deux serveurs dans deux threads différents

class MainServer
{
    private:
        CustomTcpMainServer tcpServer;
        CustomTcpServerServer tcpServerServer;
        std::vector<std::string, uint16_t> gameServersAddresses;
};

class GameServer
{
    private:
        CustomTcpServerAsClient tcpClient;
        CustomUdpGameServer udpServer;
};

int main(void)
{
    return 0;
}