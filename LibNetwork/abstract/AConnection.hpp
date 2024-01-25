/*
** EPITECH PROJECT, 2023
** Framework
** File description:
** AConnection
*/

#pragma once
#include "NetCommon.hpp"
#include "NetQueue.hpp"
#include "NetMessage.hpp"

namespace rtype
{
    namespace net
    {
		template<typename T>
		class AServer;

		template<typename T>
		class AConnection : public std::enable_shared_from_this<AConnection<T>>
		{
		public:
			// A AConnection is "owned" by either a server or a client, and its
			// behaviour is slightly different bewteen the two.
			enum class owner
			{
				server,
				client
			};

		public:
			// Constructor: Specify Owner, connect to context, transfer the socket
			//				Provide reference to incoming message queue
			AConnection(owner parent, asio::io_context& asioContext, Queue<owned_message<T>>& qIn)
				: m_asioContext(asioContext), m_qMessagesIn(qIn)
			{
				m_nOwnerType = parent;

				// Construct validation check numbers
				if (m_nOwnerType == owner::server)
				{
					// Connection is Server -> Client, construct random validation number
					m_nHandshakeOut = uint64_t(std::chrono::system_clock::now().time_since_epoch().count());
					// Pre-calculate the result for efficiency
					m_nHandshakeCheck = scramble(m_nHandshakeOut);
				}
				else
				{
					// Connection is Client -> Server, so we don't need to care
					// about m_nHandshakeOut, it is just 0. But we do need to generate
					// the validation number to receive from the server
					m_nHandshakeIn = 0;
					m_nHandshakeCheck = 0;
				}
			}

			virtual ~AConnection() = default;

			// This ID is used system wide - its how clients will understand other clients
			// exist across the whole system.
			uint32_t GetID() const
			{
				return id;
			}

		public:
			virtual void ConnectToClient(rtype::net::AServer<T>* server, uint32_t &uid = 0)
			{
				std::cout << "Default connectToClient" << std::endl;
			}

			virtual void ConnectToServer(const std::string& host, const std::string& port)
			{
				std::cout << "Default connectToServer" << std::endl;
			}

			virtual void Disconnect() {
				std::cout << "Default disconnect" << std::endl;
			}

			virtual bool IsConnected() const { return false; }

		public:
			// ASYNC - Send a message, connections are one-to-one so no need to specifiy
			// the target, for a client, the target is the server and vice versa
			void Send(const message<T>& msg)
			{
				asio::post(m_asioContext,
					[this, msg]()
					{
						// If the queue has a message in it, then we must
						// assume that it is in the process of asynchronously being written.
						// Either way add the message to the queue to be output. If no messages
						// were available to be written, then start the process of writing the
						// message at the front of the queue.
						bool bWritingMessage = !m_qMessagesOut.empty();
						m_qMessagesOut.push_back(msg);
						if (!bWritingMessage)
						{
							WriteHeader();
						}
					});
			}



		protected:
			// ASYNC - Prime context to write a message header
			virtual void WriteHeader() {
				std::cout << "Default writeHeader" << std::endl;
			}

			// ASYNC - Prime context to write a message body
			virtual void WriteBody() {
				std::cout << "Default writeBody" << std::endl;
			}

			// ASYNC - Prime context ready to read a message header
			virtual void ReadHeader() {
				std::cout << "Default readHeader" << std::endl;
			}

			// ASYNC - Prime context ready to read a message body
			virtual void ReadBody() {
				std::cout << "Default readBody" << std::endl;
			}

			// Once a full message is received, add it to the incoming queue
			void AddToIncomingMessageQueue()
			{
				// Shove it in queue, converting it to an "owned message", by initialising
				// with the a shared pointer from this AConnection object
				if(m_nOwnerType == owner::server)
					m_qMessagesIn.push_back({ this->shared_from_this(), m_msgTemporaryIn });
				else
					m_qMessagesIn.push_back({ nullptr, m_msgTemporaryIn });

				// We must now prime the asio context to receive the next message. It
				// wil just sit and wait for bytes to arrive, and the message construction
				// process repeats itself. Clever huh?
				ReadHeader();
			}

			uint64_t scramble(uint64_t nInput)
			{
				uint64_t out = nInput ^ 0xDEADBEEFC0DECAFE;
				return out;
			}
		public:
			virtual void updateEndpoint(std::string ip, uint16_t port)
			{
				std::cout << "Default updateEndpoint" << std::endl;
			}

		public:
			virtual void WriteValidation()
			{
				std::cout << "Default writeValidation" << std::endl;
			}

			virtual void ReadValidation(rtype::net::AServer<T>* server = nullptr)
			{
				std::cout << "Default readValidation" << std::endl;
			}

			// this function is called when the you need a custom function to override it
			// like something not handle in the AConnection class but in child class
			virtual void utilsForClient()
			{
				std::cout << "Default utils client" << std::endl;
			}

			virtual void utilsForServer(asio::ip::udp::endpoint data, bool buffer[1024])
			{
				std::cout << "Default utils server" << std::endl;
			}

			virtual void prepareRead(bool buffer[1024])
			{
				std::cout << "Default prepareRead" << std::endl;
			}

		protected:
			// This context is shared with the whole asio instance
			asio::io_context& m_asioContext;

			// This queue holds all messages to be sent to the remote side
			// of this AConnection
			Queue<message<T>> m_qMessagesOut;

			// This references the incoming queue of the parent object
			Queue<owned_message<T>>& m_qMessagesIn;

			// Incoming messages are constructed asynchronously, so we will
			// store the part assembled message here, until it is ready
			message<T> m_msgTemporaryIn;

			// The "owner" decides how some of the AConnection behaves
			owner m_nOwnerType = owner::server;

			uint32_t id = 0;

			// Handle checked of the clients
			uint64_t m_nHandshakeOut = 0;
			uint64_t m_nHandshakeIn = 0;
			uint64_t m_nHandshakeCheck = 0;

		};
    }
}