/*
** EPITECH PROJECT, 2023
** Framework
** File description:
** NetConnection
*/

#pragma once
#include "../abstract/AServer.hpp"

namespace rtype
{
    namespace net
    {
        namespace udp
        {
            template <typename T>
            class NetConnection : public AConnection<T>
            {
            public:
                NetConnection(typename AConnection<T>::owner parent, asio::io_context &asioContext, Queue<owned_message<T>> &qIn, asio::ip::udp::socket &socket, asio::ip::udp::endpoint senderEndpoint)
                    : AConnection<T>(parent, asioContext, qIn), m_socket(socket), _endPointToSend(senderEndpoint)
                    {
                        if (this->m_nOwnerType == AConnection<T>::owner::client)
                        {
                            this->m_socket = asio::ip::udp::socket(this->m_asioContext, asio::ip::udp::endpoint(asio::ip::udp::v4(), 0));
                        }

                        std::memset(this->_sendBuffer, 0, sizeof(this->_sendBuffer));
                        // std::memset(this->tmpDataBuffer, 0, sizeof(this->tmpDataBuffer));
                        this->_isReceive = false;
                        this->_packetNumber = 0;
                        this->_packetNumberReceive = 0;
                    }

                virtual ~NetConnection(){}

                void Disconnect()
                {
                    // Closing a UDP "connection" involves stopping any pending async operations.
                    // Since UDP is connectionless, there is no need to explicitly close a socket.
                    std::cout << "on deconnecte le client" << std::endl;
                    if (IsConnected())
                    {
                        std::cout << "[" << this->id << "] Disconnected.\n";
                        asio::post(this->m_asioContext,
                        [this]()
                        {
                            this->m_socket.close();
                        });
                    }
                }

                bool IsConnected() const
                {
                    // UDP doesn't have a persistent connection
                    return true;
                }
            public:
                asio::ip::udp::endpoint GetEndpoint() const
                {
                    return this->m_endpoint;
                }

                message<T> &getTemporaryMessage()
                {
                    return this->m_msgTemporaryIn;
                }

                void setIsReceive(bool isReceive)
                {
                    this->_isReceive = isReceive;
                }

            protected:
                void ConnectToClient(rtype::net::AServer<T>* server, uint32_t &uid = 0) override
                {
                    // we dont use serve so cast it to void  to avoid warning
                    (void)server;
                    if (this->m_nOwnerType == AConnection<T>::owner::server)
                    {
                            uid++;
                            this->id = uid;
                    }
                }

                void WriteHeader() override
                {
                    // set the number of the packet
                    this->m_qMessagesOut.front().setPacketNb(this->_packetNumber);
                    this->_packetNumber++;
                    if (this->_packetNumber >= 800)
                        this->_packetNumber = 0;
                    // prepare the buffer to send (header + body) need to memcopy all into our bool(it like a bit array) buffer

                    std::memcpy(this->_sendBuffer, &(this->m_qMessagesOut).front().header, sizeof(message_header<T>));
                    std::memcpy(this->_sendBuffer + sizeof(message_header<T>), this->m_qMessagesOut.front().body.data(), this->m_qMessagesOut.front().body.size());
                    // now we can send the buffer

                    this->m_socket.async_send_to(asio::buffer(this->_sendBuffer, sizeof(message_header<T>) + this->m_qMessagesOut.front().body.size()),
                        this->_endPointToSend,
                    [this](std::error_code ec, std::size_t length)
                    {
                        // asio has now sent the bytes - if there was a problem
                        // an error would be available...
                        if (!ec)
                        {
                                // so we are done with this message. Remove it from
                                // the outgoing message queue
                                this->m_qMessagesOut.pop_front();

                                // If the queue is not empty, there are more messages to send, so
                                // make this happen by issuing the task to send the next header.
                                if (!this->m_qMessagesOut.empty())
                                {
                                    this->WriteHeader();
                                }
                        }
                        else
                        {
                            // Sending failed, see WriteHeader() equivalent for description :P
                            std::cout << "[" << this->id << "] Write Header Fail.\n";
                            this->m_socket.close();
                        }
                    });

                }

                void ReadHeader() override
                {
                    if (this->m_msgTemporaryIn.getPacketNb() < this->_packetNumberReceive && !this->isPacketAvailable())
                    {
                        this->_isReceive = false;
                    }

                    if (this->_isReceive) {
                        this->_packetNumberReceive = this->m_msgTemporaryIn.getPacketNb();
                        this->_isReceive = false;
                        this->AddToIncomingMessageQueue();
                    }

                }

                bool isPacketAvailable()
                {
                    int packetNb = this->m_msgTemporaryIn.getPacketNb();

                    // if (this->_packetNumberReceive > 700000 && packetNb < 10000)
                    if (this->_packetNumberReceive > 700 && packetNb < 100)
                        return true;
                    else
                        return false;
                }

                void utilsForClient() override
                {
                    this->m_socket.async_receive_from(asio::buffer(this->tmpDataBuffer , 1024), this->m_recieveEndpoint,
                            [this](std::error_code ec, std::size_t length)
                            {
                                if (!ec) {
                                    // first check if the endpoint is the same as the one we want to connect to
                                    if (this->m_recieveEndpoint == this->_endPointToSend)
                                    {
                                        // thats mean the endpoint is the same as the one we want to connect to
                                        // we can read the message
                                        this->prepareRead(this->tmpDataBuffer);
                                    }
                                }
                                else
                                {
                                    std::cout << "Error on receive: " << ec.message() << "\n";
                                }
                                // Prime the asio context with more work - again simply wait for
                                // another connection...
                                this->utilsForClient();
                            });
                }

                void utilsForServer(asio::ip::udp::endpoint data, bool buffer[1024]) override
                {
                    if (data == this->_endPointToSend)
                    {
                        this->prepareRead(buffer);
                    }
                }

            public:
                virtual void WriteValidation() override
                {
                }

            protected:
                // Each NetConnection has a unique socket to a remote
                asio::ip::udp::socket &m_socket;
                // Remote endpoint as a hint to where to connect to
                asio::ip::udp::endpoint m_recieveEndpoint;

                asio::ip::udp::endpoint _endPointToSend;
                bool _sendBuffer[1024];
                bool _isReceive = false;
                unsigned int _packetNumber = 0;
                int _packetNumberReceive = 0;
                bool tmpDataBuffer[1024];
            public:
                void updateEndpoint(std::string host, uint16_t port) override
                {
                    asio::ip::udp::resolver resolver(this->m_asioContext);
                    asio::ip::udp::resolver::query query(asio::ip::udp::v4(), host, std::to_string(port));
                    asio::ip::udp::endpoint endpoint = *resolver.resolve(query);
                    this->_endPointToSend = endpoint;
                }

            protected:
                void prepareRead(bool buffer[1024]) override
                {
                    copyData(buffer);
                }

                void copyData(bool buffer[1024])
                {
                    std::memcpy(&(this->getTemporaryMessage().header), buffer, sizeof(message_header<T>));
                    // now we have the header we can get the body size
                    this->getTemporaryMessage().body.resize(this->getTemporaryMessage().header.size);
                    // now we can  copy the rest of the message in the body
                    std::memcpy(this->getTemporaryMessage().body.data(), buffer + sizeof(message_header<T>), this->getTemporaryMessage().body.size());
                    this->setIsReceive(true);
                    this->ReadHeader();
                }
            };
        }
    }
}