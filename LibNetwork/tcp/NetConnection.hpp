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
        namespace tcp
        {
            template <typename T>
            class NetConnection : public AConnection<T>
            {
            public:
                // Constructor: Specify Owner, connect to context, transfer the socket
                //				Provide reference to incoming message queue
                NetConnection(typename AConnection<T>::owner parent, asio::io_context &asioContext, asio::ip::tcp::socket socket, Queue<owned_message<T>> &qIn)
                    : AConnection<T>(parent, asioContext, qIn), m_socket(std::move(socket))
                {
                }

                virtual ~NetConnection()
                {
                }

            public:
                void ConnectToClient(rtype::net::AServer<T> *server, uint32_t &uid = 0) override
                {
                    if (this->m_nOwnerType == AConnection<T>::owner::server)
                    {
                        if (this->m_socket.is_open())
                        {
                            uid++;
                            this->id = uid;
                            WriteValidation();

                            ReadValidation(server);
                        }
                    }
                }

                void ConnectToServer(const std::string& host, const std::string& port) override
                {
                    std::cout << "Client Connection to IP/Host: " << host << " Port: " << port << std::endl;
                    // Only clients can connect to servers
                    if (this->m_nOwnerType == AConnection<T>::owner::client)
                    {
                        // Create endpoint as a hint to where we want to connect to
                        asio::ip::tcp::resolver resolver(this->m_asioContext);
                        asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, port);

                        // Request asio attempts to connect to an endpoint
                        asio::async_connect(this->m_socket, endpoints,
                            [this](std::error_code ec, asio::ip::tcp::endpoint endpoint)
                            {
                                if (!ec)
                                {
                                    ReadValidation();
                                } else {
                                    std::cout << "[" << this->id << "] Connect Fail.\n";
                                    std::cout << ec.message() << std::endl;
                                }
                            });
                    }
                }

                void Disconnect()
                {
                    if (IsConnected())
                        asio::post(this->m_asioContext,
                        [this]()
                        {
                            this->m_socket.close();
                        });
                }

                bool IsConnected() const
                {
                    return this->m_socket.is_open();
                }

            protected:
                // ASYNC - Prime context to write a message header
                void WriteHeader() override
                {
                    // If this function is called, we know the outgoing message queue must have
                    // at least one message to send. So allocate a transmission buffer to hold
                    // the message, and issue the work - asio, send these bytes
                    // print the heder size
                    asio::async_write(this->m_socket, asio::buffer(&(this->m_qMessagesOut).front().header, sizeof(message_header<T>)),
                    [this](std::error_code ec, std::size_t length)
                    {
                        // asio has now sent the bytes - if there was a problem
                        // an error would be available...
                        if (!ec)
                        {
                            // ... no error, so check if the message header just sent also
                            // has a message body...
                            if (this->m_qMessagesOut.front().body.size() > 0)
                            {
                                // ...it does, so issue the task to write the body bytes
                                this->WriteBody();
                            }
                            else
                            {
                                // ...it didnt, so we are done with this message. Remove it from
                                // the outgoing message queue
                                this->m_qMessagesOut.pop_front();

                                // If the queue is not empty, there are more messages to send, so
                                // make this happen by issuing the task to send the next header.
                                if (!this->m_qMessagesOut.empty())
                                {
                                    this->WriteHeader();
                                }
                            }
                        }
                        else
                        {
                            // ...asio failed to write the message, we could analyse why but
                            // for now simply assume the NetConnection has died by closing the
                            // socket. When a future attempt to write to this client fails due
                            // to the closed socket, it will be tidied up.
                            std::cout << "[" << this->id << "] Write Header Fail.\n";
                            this->m_socket.close();
                        }
                    });
                }

                // ASYNC - Prime context to write a message body
                void WriteBody() override
                {
                    // If this function is called, a header has just been sent, and that header
                    // indicated a body existed for this message. Fill a transmission buffer
                    // with the body data, and send it!
                    asio::async_write(this->m_socket, asio::buffer(this->m_qMessagesOut.front().body.data(), this->m_qMessagesOut.front().body.size()),
                    [this](std::error_code ec, std::size_t length)
                    {
                        if (!ec)
                        {
                            // Sending was successful, so we are done with the message
                            // and remove it from the queue
                            this->m_qMessagesOut.pop_front();

                            // If the queue still has messages in it, then issue the task to
                            // send the next messages' header.
                            if (!this->m_qMessagesOut.empty())
                            {
                                WriteHeader();
                            }
                        }
                        else
                        {
                            // Sending failed, see WriteHeader() equivalent for description :P
                            std::cout << "[" << this->id << "] Write Body Fail.\n";
                            this->m_socket.close();
                        }
                    });
                }

                // ASYNC - Prime context ready to read a message header
                void ReadHeader() override
                {
                    // If this function is called, we are expecting asio to wait until it receives
                    // enough bytes to form a header of a message. We know the headers are a fixed
                    // size, so allocate a transmission buffer large enough to store it. In fact,
                    // we will construct the message in a "temporary" message object as it's
                    // convenient to work with.
                    asio::async_read(this->m_socket, asio::buffer(&this->m_msgTemporaryIn.header, sizeof(message_header<T>)),
                    [this](std::error_code ec, std::size_t length)
                    {
                        if (!ec)
                        {
                            // A complete message header has been read, check if this message
                            // has a body to follow...
                            if (this->m_msgTemporaryIn.header.size > 0)
                            {
                                // ...it does, so allocate enough space in the messages' body
                                // vector, and issue asio with the task to read the body.
                                this->m_msgTemporaryIn.body.resize(this->m_msgTemporaryIn.header.size);
                                ReadBody();
                            }
                            else
                            {
                                // it doesn't, so add this bodyless message to the connections
                                // incoming message queue
                                this->AddToIncomingMessageQueue();
                            }
                        }
                        else
                        {
                            // Reading form the client went wrong, most likely a disconnect
                            // has occurred. Close the socket and let the system tidy it up later.
                            std::cout << "[" << this->id << "] Read Header Fail.\n";
                            this->m_socket.close();
                        }
                    });
                }

                // ASYNC - Prime context ready to read a message body
                void ReadBody() override
                {
                    // If this function is called, a header has already been read, and that header
                    // request we read a body, The space for that body has already been allocated
                    // in the temporary message object, so just wait for the bytes to arrive...
                    asio::async_read(this->m_socket, asio::buffer(this->m_msgTemporaryIn.body.data(), this->m_msgTemporaryIn.body.size()),
                    [this](std::error_code ec, std::size_t length)
                    {
                        if (!ec)
                        {
                            // ...and they have! The message is now complete, so add
                            // the whole message to incoming queue
                            this->AddToIncomingMessageQueue();
                        }
                        else
                        {
                            // As above!
                            std::cout << "[" << this->id << "] Read Body Fail.\n";
                            this->m_socket.close();
                        }
                    });
                }

                void WriteValidation() override
                {
                    asio::async_write(this->m_socket, asio::buffer(&(this->m_nHandshakeOut), sizeof(uint64_t)),
                    [this](std::error_code ec, std::size_t length)
                    {
                        if (!ec)
                        {
                            // Validation data sent, client should sit and wait
                            // for a response (or a disconnect)
                            if (this->m_nOwnerType == AConnection<T>::owner::client)
                                ReadHeader();
                        }
                        else
                        {
                            // Some bigger failure occurred, close NetConnection
                            this->m_socket.close();
                        }
                    });
                }

                void ReadValidation(rtype::net::AServer<T> *server = nullptr) override
                {
                    asio::async_read(this->m_socket, asio::buffer(&(this->m_nHandshakeIn), sizeof(uint64_t)),
                    [this, server](std::error_code ec, std::size_t length)
                    {
                        if (!ec)
                        {
                            if (this->m_nOwnerType == AConnection<T>::owner::server)
                            {
                                if (this->m_nHandshakeIn == this->m_nHandshakeCheck)
                                {
                                    std::cout << "Client Validated" << std::endl;
                                    server->OnClientValidated(this->shared_from_this());
                                    ReadHeader();
                                }
                                else
                                {
                                    std::cout << "Client Disconnected (Fail Validation)" << std::endl;
                                    this->m_socket.close();
                                }
                            }
                            else
                            {
                                // Client is waiting for server approval
                                this->m_nHandshakeOut = this->scramble(this->m_nHandshakeIn);

                                // Write the result
                                WriteValidation();
                            }
                        }
                        else
                        {
                            // Some bigger failure occurred, close NetConnection
                            std::cout << "Client Disconnected (ReadValidation)" << std::endl;
                            std::cout << ec.message() << std::endl;
                            this->m_socket.close();
                        }
                    });
                }

            protected:
                // Each NetConnection has a unique socket to a remote
                asio::ip::tcp::socket m_socket;
            };
        }
    }
}