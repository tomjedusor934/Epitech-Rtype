/*
** EPITECH PROJECT, 2023
** Framework
** File description:
** NetClient
*/

#pragma once
#include "../abstract/AClient.hpp"
#include "NetConnection.hpp"

namespace rtype
{
    namespace net
    {
        namespace tcp
        {
            template <typename T>
            class NetClient : public AClient<T>
            {
            public:
                NetClient() : AClient<T>()
                {}

            public:
                // Connect to server with hostname/ip-address and port
                bool Connect(const std::string& host, const uint16_t port) override
                {
                    try
                    {
                        // Resolve hostname/ip-address into tangiable physical address
                        asio::ip::tcp::resolver resolver(this->m_context);
                        asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));

                        // Create connection
                        this->m_connection = std::make_unique<NetConnection<T>>(AConnection<T>::owner::client, this->m_context, asio::ip::tcp::socket(this->m_context), this->m_qMessagesIn);

                        // Tell the connection object to connect to server
                        this->m_connection->ConnectToServer(host, std::to_string(port));

                        // Start Context Thread
                        this->thrContext = std::thread([this]() { this->m_context.run(); });
                    }
                    catch (std::exception& e)
                    {
                        std::cerr << "Client Exception: " << e.what() << "\n";
                        return false;
                    }
                    return true;
                }
            };
        }
    }
}