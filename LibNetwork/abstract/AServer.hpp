/*
** EPITECH PROJECT, 2023
** Framework
** File description:
** AServer
*/

#pragma once
#include "NetCommon.hpp"
#include "AConnection.hpp"

namespace rtype
{
    namespace net
    {
        template<typename T>
		class AServer
		{
		public:
			// Create a server, ready to listen on specified port
			AServer(uint16_t port, std::string server_name)
			{
                this->m_port = port;
				this->_server_name = server_name;
			}

			virtual ~AServer()
			{
				Stop();
			}

			// Starts the server!
			bool Start()
			{
				try
				{
					// Issue a task to the asio context - This is important
					// as it will prime the context with "work", and stop it
					// from exiting immediately. Since this is a server, we
					// want it primed ready to handle clients trying to
					// connect.
					WaitForClientConnection();

					// Launch the asio context in its own thread
					m_threadContext = std::thread([this]() { m_asioContext.run(); });
				}
				catch (std::exception& e)
				{
					// Something prohibited the server from listening
					std::cerr << "[SERVER] Exception: " << e.what() << "\n";
					return false;
				}

				std::cout << "[SERVER " + this->_server_name + "] Started!\n";
				return true;
			}

			// Stops the server!
			void Stop()
			{
				// Request the context to close
				m_asioContext.stop();

				// Tidy up the context thread
				if (m_threadContext.joinable()) m_threadContext.join();

				// Inform someone, anybody, if they care...
				std::cout << "[SERVER] Stopped!\n";
			}

			// ASYNC - Instruct asio to wait for AConnection
			virtual void WaitForClientConnection() = 0;

			// Send a message to a specific client
			void MessageClient(std::shared_ptr<AConnection<T>> client, const message<T>& msg)
			{
				// Check client is legitimate...
				if (client && client->IsConnected())
				{
					// ...and post the message via the AConnection
					client->Send(msg);
				}
				else
				{
					// If we cant communicate with client then we may as
					// well remove the client - let the server know, it may
					// be tracking it somehow
					OnClientDisconnect(client);

					// Off you go now, bye bye!
					client.reset();

					// Then physically remove it from the container
					m_deqConnections.erase(
						std::remove(m_deqConnections.begin(), m_deqConnections.end(), client), m_deqConnections.end());
				}
			}

			// Send message to all clients
			void MessageAllClients(const message<T>& msg, std::shared_ptr<AConnection<T>> pIgnoreClient = nullptr)
			{
				bool bInvalidClientExists = false;

				// Iterate through all clients in container
				for (auto& client : m_deqConnections)
				{
					std::cout << "client id: " << client->GetID() << std::endl;
					// Check client is connected...
					if (client && client->IsConnected())
					{
						// ..it is!
						if(client != pIgnoreClient) {
							std::cout << "client id not ignored: " << client->GetID() << std::endl;
							client->Send(msg);
						}
					}
					else
					{
						// The client couldnt be contacted, so assume it has
						// disconnected.
						OnClientDisconnect(client);
						client.reset();

						// Set this flag to then remove dead clients from container
						bInvalidClientExists = true;
					}
				}

				// Remove dead clients, all in one go - this way, we dont invalidate the
				// container as we iterated through it.
				if (bInvalidClientExists)
					m_deqConnections.erase(
						std::remove(m_deqConnections.begin(), m_deqConnections.end(), nullptr), m_deqConnections.end());
			}

			// Force server to respond to incoming messages
			void Update(size_t nMaxMessages = -1, bool bWait = false)
			{
				// std::cout << "Update server " << this->_server_name << std::endl;
				for (auto& connection : m_deqConnections) {
					if (!connection || !connection->IsConnected())
					{
						connection.reset();
						OnClientDisconnect(connection);
					}
				}
				if (bWait) m_qMessagesIn.wait();

				// Process as many messages as you can up to the value
				// specified
				size_t nMessageCount = 0;
				while (nMessageCount < nMaxMessages && !m_qMessagesIn.empty())
				{
					// Grab the front message
					auto msg = m_qMessagesIn.pop_front();

					// Pass to message handler
					OnMessage(msg.remote, msg.msg);

					nMessageCount++;
				}
			}

			int getPort() const
			{
				return m_port;
			}

			int getNbConnections() const
			{
				return m_deqConnections.size();
			}

		protected:
			// This server class should override thse functions to implement
			// customised functionality

			// Called when a client connects, you can veto the AConnection by returning false
			virtual bool OnClientConnect(std::shared_ptr<AConnection<T>> client)
			{
				std::cout << "Default OnClientConnect is called and return false if you don't want please override it" << std::endl;
				return false;
			}

			// Called when a client appears to have disconnected
			virtual void OnClientDisconnect(std::shared_ptr<AConnection<T>> client)
			{
				std::cout << "Default OnClientDisconnect is called please override it" << std::endl;
			}

			// Called when a message arrives
			virtual void OnMessage(std::shared_ptr<AConnection<T>> client, message<T>& msg)
			{
				std::cout << "Default OnMessage is called please override it" << std::endl;
			}

		public:
			// Called when a client is validated
			virtual void OnClientValidated(std::shared_ptr<AConnection<T>> client)
			{
				std::cout << "Default OnClientValidated is called please override it" << std::endl;
			}


		protected:
			// Thread Safe Queue for incoming message packets
			Queue<owned_message<T>> m_qMessagesIn;

			// Container of active validated connections
			std::deque<std::shared_ptr<AConnection<T>>> m_deqConnections;

			// Order of declaration is important - it is also the order of initialisation
			asio::io_context m_asioContext;
			std::thread m_threadContext;

			// Clients will be identified in the "wider system" via an ID
			uint32_t nIDCounter = 10000;

            // Port
            uint16_t m_port;

			std::string _server_name;
		};
    }
}