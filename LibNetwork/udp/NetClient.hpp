/*
** TAL-WEB PROJECT, 2023
** net-framework
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
        namespace udp
        {
            template <typename T>
            class NetClient : public AClient<T>
            {
            public:
                NetClient() : AClient<T>(), m_socket(AClient<T>::m_context)
                {
                }

            public:
                // Connect to server with hostname/ip-address and port
                bool Connect(const std::string& host, const uint16_t port) override
                {
                    try
                    {
                        std::cout << "Connecting to " << host << ":" << port << std::endl;
                        // Resolve hostname/ip-address into tangiable physical address for UDP
                        asio::ip::udp::resolver resolver(this->m_context);
                        asio::ip::udp::resolver::query query(asio::ip::udp::v4(), host, std::to_string(port));
                        asio::ip::udp::endpoint endpoint = *resolver.resolve(query);

                        // Create connection
                        this->m_connection = std::make_unique<NetConnection<T>>(AConnection<T>::owner::client, this->m_context, this->m_qMessagesIn, this->m_socket, endpoint);

                        this->update();

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

                protected:
                    asio::ip::udp::socket m_socket;

                    void update()
                    {
                         // read the message from the server
                        this->m_connection->utilsForClient();
                    }

            };
        }
    }
}