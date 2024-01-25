/*
** TAL-WEB PROJECT, 2023
** net-framework
** File description:
** NetServer
*/

#pragma once
#include "NetConnection.hpp"

namespace rtype
{
    namespace net
    {
        namespace udp
        {
            template <typename T>
            class NetServer : public AServer<T>
            {
                public:
                    NetServer(uint16_t port, std::string server_name)
                        : AServer<T>(port, server_name),
                        m_socket(AServer<T>::m_asioContext, asio::ip::udp::endpoint(asio::ip::udp::v4(), port))
                    {
                        std::memset(this->tmpDataBuffer, 0, sizeof(this->tmpDataBuffer));
                    }

                    virtual ~NetServer()
                    {
                        this->Stop();
                    }

                    void WaitForClientConnection() override
                    {
                        this->m_socket.async_receive_from(asio::buffer(this->tmpDataBuffer , 1024), this->m_remoteEndpoint,
                            [this](std::error_code ec, std::size_t length)
                            {
                                if (!ec) {
                                    // first check if the endpoint is already in the vector of of connected endpoint
                                    if (std::find(this->_loggedInEndpoints.begin(), this->_loggedInEndpoints.end(), this->m_remoteEndpoint) != this->_loggedInEndpoints.end())
                                    {
                                        // thats mean the endpoint is already connected
                                        // find the endpoint in the  std::deque<std::shared_ptr<AConnection<T>>> m_deqConnections to call his read header function
                                        for (auto &conn : this->m_deqConnections)
                                        {
                                            conn->utilsForServer(this->m_remoteEndpoint, this->tmpDataBuffer);
                                        }
                                    } else {
                                        std::shared_ptr<NetConnection<T>> newconn =
                                            std::make_shared<NetConnection<T>>(NetConnection<T>::owner::server,
                                            this->m_asioContext, this->m_qMessagesIn, this->m_socket, this->m_remoteEndpoint);

                                        this->m_deqConnections.push_back(std::move(newconn));
                                        this->m_deqConnections.back()->ConnectToClient(this, this->nIDCounter);
                                        if (this->OnClientConnect(this->m_deqConnections.back())) {

                                            this->_loggedInEndpoints.push_back(this->m_remoteEndpoint);
                                            this->m_deqConnections.back()->prepareRead(tmpDataBuffer);

                                        } else {
                                            // if the connection is not ok we close the connection and we add the endpoint to the vector of banned endpoint
                                            this->validationFailed("New conenction not allowed yet.", newconn);
                                            // supprimer la derniere connexion
                                            this->nIDCounter--;
                                            this->m_deqConnections.pop_back();

                                        }
                                    }
                                } else {
                                    std::cout << "[SERVER] New Connection Error: " << ec.message() << "\n";
                                    exit(0);
                                }
                                // Prime the asio context with more work - again simply wait for
                                // another connection...
                                this->WaitForClientConnection();
                            }
                        );
                    }

                protected:
                    void validationFailed(std::string msg, std::shared_ptr<NetConnection<T>> conn)
                    {
                        std::cout << "Validation failed:" << msg << std::endl;
                        // conn->Disconnect();
                        conn.reset();
                    }

                protected:
                    asio::ip::udp::socket m_socket;
                    std::vector<asio::ip::udp::endpoint> _loggedInEndpoints;
                    asio::ip::udp::endpoint m_remoteEndpoint;

                    // this is used ti store all the data received from the client until we know if it is a login or a message is authorized
                    bool tmpDataBuffer[1024];

            };
        }
    }
}