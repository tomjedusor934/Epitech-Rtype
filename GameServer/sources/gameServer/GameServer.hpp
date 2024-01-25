/**
 * @file GameServer.hpp
 * @brief Definition of the CustomUdpGameServer class.
 */

#pragma once

#include "../../includes/CommonIncludes.hpp"
#include "../gameLogic/GameLogic.hpp"

namespace rtype {

    /**
     * @class CustomUdpGameServer
     * @brief This class is used for the communication between the game servers and the clients.
     *        It uses the UDP protocol for fast and reliable message delivery.
     */
    class CustomUdpGameServer : public rtype::net::udp::NetServer<GameServerMsgTypes>
    {
    public:
        /**
         * @brief Constructor for CustomUdpGameServer.
         * @param port The port number to bind the server to.
         * @param mtx A reference to the mutex used for thread synchronization.
         * @param canAcceptClients A reference to the flag indicating whether the server can accept new clients.
         * @param infos A reference to the game server information.
         * @param gameLogic A reference to the game logic instance.
         */
        CustomUdpGameServer(uint16_t port, std::mutex& mtx, int& canAcceptClients, gameServerInfo_t& infos, rtype::GameLogic& gameLogic)
            : rtype::net::udp::NetServer<GameServerMsgTypes>(port, ""), _mtx(mtx), _canAcceptClients(canAcceptClients), _infos(infos), gameLogic(gameLogic)
        {
            this->_isServerRunning = true;
            this->_gameStarted = false;
        }

        /**
         * @brief Callback function called when a client connects to the server.
         * @param client The connection object representing the client.
         * @return True if the client connection is accepted, false otherwise.
         */
        bool OnClientConnect(std::shared_ptr<rtype::net::AConnection<GameServerMsgTypes>> client) override;

        /**
         * @brief Callback function called when a client disconnects from the server.
         * @param client The connection object representing the client.
         */
        void OnClientDisconnect(std::shared_ptr<rtype::net::AConnection<GameServerMsgTypes>> client) override;

        /**
         * @brief Callback function called when a message is received from a client.
         * @param client The connection object representing the client.
         * @param msg The received message.
         */
        void OnMessage(std::shared_ptr<rtype::net::AConnection<GameServerMsgTypes>> client, rtype::net::message<GameServerMsgTypes>& msg) override
        {
            if (this->_MsgHandlerMap.find(msg.header.id) != this->_MsgHandlerMap.end())
                (this->*_MsgHandlerMap[msg.header.id])(client, msg);
            else
                std::cerr << "Unknown message type" << std::endl;
        }

        /**
         * @brief Setter for the server running flag.
         * @param isServerRunning The value to set for the server running flag.
         */
        void setServerRunning(bool isServerRunning)
        {
            this->_isServerRunning = isServerRunning;
        }

        /**
         * @brief Getter for the server running flag.
         * @return True if the server is running, false otherwise.
         */
        bool getServerRunning() const
        {
            return this->_isServerRunning;
        }

        /**
         * @brief Getter for the game started flag.
         * @return True if the game has started, false otherwise.
         */
        bool getGameStarted() const
        {
            return this->_gameStarted;
        }

        /**
         * @brief Setter for the game started flag.
         * @param gameStarted The value to set for the game started flag.
         */
        void setGameStarted(bool gameStarted)
        {
            this->_gameStarted = gameStarted;
        }

        /**
         * @brief Sends a message to all other players except the specified client.
         * @param client The connection object representing the client.
         * @param msg The message to send.
         */
        void sendToOtherPlayers(std::shared_ptr<rtype::net::AConnection<GameServerMsgTypes>> client, rtype::net::message<GameServerMsgTypes>& msg)
        {
            for (auto &player : this->gameLogic.teamOne) {
                if (player.second->client != client && player.second->client != nullptr) {
                    player.second->client->Send(msg);
                }
            }
            for (auto &player : this->gameLogic.teamTwo) {
                if (player.second->client != client && player.second->client != nullptr) {
                    player.second->client->Send(msg);
                }
            }
        }

    private:
        // Private member functions for handling specific message types
        void PlayerJump(std::shared_ptr<rtype::net::AConnection<GameServerMsgTypes>> client, rtype::net::message<GameServerMsgTypes> msg);
        void ConnectToGameServer(std::shared_ptr<rtype::net::AConnection<GameServerMsgTypes>> client, rtype::net::message<GameServerMsgTypes> msg);
        void PlayerMove(std::shared_ptr<rtype::net::AConnection<GameServerMsgTypes>> client, rtype::net::message<GameServerMsgTypes> msg);
        void PlayerLook(std::shared_ptr<rtype::net::AConnection<GameServerMsgTypes>> client, rtype::net::message<GameServerMsgTypes> msg);
        void PlayerShoot(std::shared_ptr<rtype::net::AConnection<GameServerMsgTypes>> client, rtype::net::message<GameServerMsgTypes> msg);

    private:
        // Map of message types and their corresponding handler functions
        std::map<GameServerMsgTypes, void (CustomUdpGameServer::*)(std::shared_ptr<rtype::net::AConnection<GameServerMsgTypes>>, rtype::net::message<GameServerMsgTypes>)> _MsgHandlerMap = {
            {GameServerMsgTypes::ConnectToGameServer, &CustomUdpGameServer::ConnectToGameServer},
            {GameServerMsgTypes::PlayerJump, &CustomUdpGameServer::PlayerJump},
            {GameServerMsgTypes::PlayerMove, &CustomUdpGameServer::PlayerMove},
            {GameServerMsgTypes::PlayerLook, &CustomUdpGameServer::PlayerLook},
            {GameServerMsgTypes::PlayerShoot, &CustomUdpGameServer::PlayerShoot}
        };

    private:
        std::mutex& _mtx; // Mutex for thread synchronization
        bool _isServerRunning; // Flag indicating whether the server is running
        int& _canAcceptClients; // Flag indicating whether the server can accept new clients
        gameServerInfo_t& _infos; // Game server information
        rtype::GameLogic& gameLogic; // Game logic instance
        bool _gameStarted; // Flag indicating whether the game has started
    };

} // namespace rtype
