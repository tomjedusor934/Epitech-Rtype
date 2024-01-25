/*
** EPITECH PROJECT, 2023
** RType
** File description:
** GameToMain
*/

#pragma once
#include "../../includes/CommonIncludes.hpp"
#include "../gameLogic/GameLogic.hpp"

namespace rtype
{
    /**
     * \brief This class is used for the communication between the main server and the game servers.
     * 
     * The main server has a second server tcp dedicated to the communication with the game servers,
     * while the game servers have a client tcp dedicated to the communication with the main server.
     */
    class CustomTcpServerAsClient : public rtype::net::tcp::NetClient<CustomCommunicationServer>
    {
        public:
            /**
             * \brief Constructor for CustomTcpServerAsClient.
             * 
             * \param mtx A reference to the mutex used to lock the access to data shared between threads.
             * \param ip The IP address to connect to.
             * \param port The port number to connect to.
             * \param canAcceptClient A reference to the flag indicating whether the server can accept clients.
             * \param infos A reference to the game server information.
             * \param gameLogic A reference to the game logic object.
             */
            CustomTcpServerAsClient(std::mutex &mtx, char (&ip)[INET_ADDRSTRLEN], uint16_t port, int &canAcceptClient, gameServerInfo_t &infos, rtype::GameLogic &gameLogic): NetClient<CustomCommunicationServer>(), _mtx(mtx), _ip(ip), _port(port), _canAcceptClient(canAcceptClient), _infos(infos), gameLogic(gameLogic)
            {
                // std::memset(this->_ip, 0, sizeof(this->_ip));
                // std::strcpy(this->_ip, this->_ipstr.c_str());
            };

            /**
             * \brief Destructor for CustomTcpServerAsClient.
             */
            ~CustomTcpServerAsClient()
            {};

            /**
             * \brief Handles the incoming messages.
             */
            void handleMessages()
            {
                rtype::net::message<CustomCommunicationServer> msg;

                while (!this->Incoming().empty()) {
                    msg = this->m_qMessagesIn.pop_front().msg;
                    if (this->_MsgHandlers.find(msg.header.id) != this->_MsgHandlers.end()) {
                        (this->*_MsgHandlers[msg.header.id])(msg);
                    } else {
                        std::cerr << "Message not handled" << std::endl;
                    }
                }
            }

        private:
            // Functions to handle specific message types
            void Infos(rtype::net::message<CustomCommunicationServer> &msg);
            void StartAcceptingClients(rtype::net::message<CustomCommunicationServer> &msg);
            void GameHistoryInfos(rtype::net::message<CustomCommunicationServer> &msg);

        private:
            // Map of message handlers
            std::map<CustomCommunicationServer, void (CustomTcpServerAsClient::*)(rtype::net::message<CustomCommunicationServer> &)> _MsgHandlers =
            {
                { CustomCommunicationServer::Infos, &CustomTcpServerAsClient::Infos },
                { CustomCommunicationServer::StartAcceptingClients, &CustomTcpServerAsClient::StartAcceptingClients},
                { CustomCommunicationServer::GameHistoryInfos, &CustomTcpServerAsClient::GameHistoryInfos}
            };

        private:
            // Mutex for thread-safe access to shared data
            std::mutex &_mtx;
            std::string _ipstr;
            char (&_ip)[INET_ADDRSTRLEN];
            uint16_t _port;

        private:
            // References to shared data
            int &_canAcceptClient;
            gameServerInfo_t &_infos;
            rtype::GameLogic &gameLogic;
    };
} // namespace rtype
