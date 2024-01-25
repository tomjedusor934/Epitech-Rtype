/*
** EPITECH PROJECT, 2023
** RType
** File description:
** ServerToServer
*/

#pragma once
    #include "../gameServerInfos/GameServerInfos.hpp"
    #include "../DbManager/DbManager.hpp"

//* this class is used for the communication between the main server and the clients
//* we use the tcp protocol because it's a reliable protocol
//* so we are sure that the messages will be received by the clients (here the game server) like for the important messages

namespace rtype
{
    class GameServerManager : public rtype::net::tcp::NetServer<CustomCommunicationServer>
    {
    public:
        GameServerManager(uint16_t port, std::mutex &mtx, std::map<int, rtype::GameServerInfos> &servers, std::string server_name, std::map<int, rtype::Lobby> &lobbies) : rtype::net::tcp::NetServer<CustomCommunicationServer>(port, server_name), _servers(servers), _mtx(mtx), _lobbies(lobbies)
        {
            this->_isServerRunning = true;
            this->_dbManager = std::make_unique<DbManager>();
            this->_dbManager->connect("127.0.0.1", "root", "test", "rtype");
        }

        bool OnClientConnect(std::shared_ptr<rtype::net::AConnection<CustomCommunicationServer>> client) override;
        void OnClientDisconnect(std::shared_ptr<rtype::net::AConnection<CustomCommunicationServer>> client) override;
        void OnClientValidated(std::shared_ptr<rtype::net::AConnection<CustomCommunicationServer>> client) override;

        void OnMessage(std::shared_ptr<rtype::net::AConnection<CustomCommunicationServer>> client, rtype::net::message<CustomCommunicationServer>& msg) override
        {
            if (this->_MsgHandlerMap.find(msg.header.id) != this->_MsgHandlerMap.end())
                (this->*_MsgHandlerMap[msg.header.id])(client, msg);
            else
                std::cerr << "Unknown message type" << std::endl;
        }

    public:
        void setServerRunning(bool isServerRunning)
        {
            this->_isServerRunning = isServerRunning;
        }

        bool getServerRunning() const
        {
            return this->_isServerRunning;
        }

    private:
        //* in this private section implement all the functions that will be called when we receive a message with the right type (MainServerMsgTypes)
        //! then when new function is added add the function in the map in the private section below

        void GameServer(std::shared_ptr<rtype::net::AConnection<CustomCommunicationServer>> client, rtype::net::message<CustomCommunicationServer>& msg);
        void GameFinished(std::shared_ptr<rtype::net::AConnection<CustomCommunicationServer>> client, rtype::net::message<CustomCommunicationServer>& msg);
        void GameHistoryPlayerInfos(std::shared_ptr<rtype::net::AConnection<CustomCommunicationServer>> client, rtype::net::message<CustomCommunicationServer>& msg);

    private:
        std::string prepareGameHistoryPlayerInfos(game_history_t, int part);

    private:
        //! here is the map that you have to fill with the right function

        //* this map is used to call the right function when we receive a message with the right type (MainServerMsgTypes)
        std::map<CustomCommunicationServer, void (GameServerManager::*)(std::shared_ptr<rtype::net::AConnection<CustomCommunicationServer>>, rtype::net::message<CustomCommunicationServer>&)> _MsgHandlerMap = {
            { CustomCommunicationServer::GameServer, &GameServerManager::GameServer },
            { CustomCommunicationServer::GameFinished, &GameServerManager::GameFinished },
            { CustomCommunicationServer::GameHistoryPlayerInfos, &GameServerManager::GameHistoryPlayerInfos }
        };

    private:
        //? here the map is used to store the data of the game sever conneected shared between threads
        std::map<int, rtype::GameServerInfos> &_servers;
        //? here the mutex is used to lock the access to data shared between threads
        std::mutex &_mtx;

    private:
        bool _isServerRunning;

    private:
        std::unique_ptr<DbManager> _dbManager;
        std::map<int, rtype::Lobby> &_lobbies;
    };
} // namespace rtype