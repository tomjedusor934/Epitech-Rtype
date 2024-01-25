/*
** EPITECH PROJECT, 2023
** Framework
** File description:
** NetServer
*/

#pragma once
#include "NetConnection.hpp"

namespace rtype
{
    namespace net
    {
        namespace tcp
        {
            template<typename T>
            class NetServer : public AServer<T>
            {
            public:
                // Create a server, ready to listen on specified port
                NetServer(uint16_t port, std::string server_name)
                    : AServer<T>(port, server_name),
                    m_asioAcceptor(AServer<T>::m_asioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
                {
                }


                virtual ~NetServer()
                {
                    // May as well try and tidy up
                    this->Stop();
                }

                // ASYNC - Instruct asio to wait for NetConnection
                void WaitForClientConnection() override
                {
                    // Prime context with an instruction to wait until a socket connects. This
                    // is the purpose of an "acceptor" object. It will provide a unique socket
                    // for each incoming NetConnection attempt
                    m_asioAcceptor.async_accept(
                        [this](std::error_code ec, asio::ip::tcp::socket socket)
                        {
                            // Triggered by incoming NetConnection request
                            if (!ec)
                            {
                                // Display some useful(?) information
                                std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << "\n";

                                // Create a new NetConnection to handle this client
                                std::shared_ptr<NetConnection<T>> newconn =
                                    std::make_shared<NetConnection<T>>(NetConnection<T>::owner::server,
                                        this->m_asioContext, std::move(socket), this->m_qMessagesIn);

                                // Give the user server a chance to deny NetConnection
                                if (this->OnClientConnect(newconn))
                                {
                                    // Connection allowed, so add to container of new connections
                                    this->m_deqConnections.push_back(std::move(newconn));

                                    // And very important! Issue a task to the NetConnection's
                                    // asio context to sit and wait for bytes to arrive!
                                    this->m_deqConnections.back()->ConnectToClient(this, this->nIDCounter);

                                    std::cout << "[" << this->m_deqConnections.back()->GetID() << "] Connection Approved\n";
                                }
                                else
                                {
                                    std::cout << "[-----] Connection Denied\n";

                                    // Connection will go out of scope with no pending tasks, so will
                                    // get destroyed automagically due to the wonder of smart pointers
                                }
                            }
                            else
                            {
                                // Error has occurred during acceptance
                                std::cout << "[SERVER] New Connection Error: " << ec.message() << "\n";
                            }

                            // Prime the asio context with more work - again simply wait for
                            // another NetConnection...
                            WaitForClientConnection();
                        });
                }

            protected:
                // These things need an asio context
                asio::ip::tcp::acceptor m_asioAcceptor; // Handles new incoming NetConnection attempts...
            };
        }
    }
}