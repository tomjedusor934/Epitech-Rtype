/*
** EPITECH PROJECT, 2023
** Framework
** File description:
** AClient
*/

#pragma once
#include "NetCommon.hpp"
#include "AConnection.hpp"

namespace rtype
{
    namespace net
    {
        template <typename T>
		class AClient
		{
		public:
			AClient()
			{}

			virtual ~AClient()
			{
				// If the client is destroyed, always try and disconnect from server
				Disconnect();
			}

		public:
			// Connect to server with hostname/ip-address and port
			virtual bool Connect(const std::string& host, const uint16_t port) = 0;

			// Disconnect from server
			void Disconnect()
			{
				// If AConnection exists, and it's connected then...
				if(IsConnected())
				{
					// ...disconnect from server gracefully
					m_connection->Disconnect();
				}

				// Either way, we're also done with the asio context...
				m_context.stop();
				// ...and its thread
				if (thrContext.joinable())
					thrContext.join();

				// Destroy the AConnection object
				m_connection.release();
			}

			// Check if client is actually connected to a server
			bool IsConnected()
			{
				if (m_connection)
					return m_connection->IsConnected();
				else
					return false;
			}

		public:
			// Send message to server
			void Send(const message<T>& msg)
			{
				// If AConnection exists and it's connected then send message via AConnection*
				if (IsConnected())
					m_connection->Send(msg);
			}

			// Retrieve queue of messages from server
			Queue<owned_message<T>>& Incoming()
			{
				return m_qMessagesIn;
			}

		protected:
			// asio context handles the data transfer...
			asio::io_context m_context;
			// ...but needs a thread of its own to execute its work commands
			std::thread thrContext;
			// The client has a single instance of a "AConnection" object, which handles data transfer
			std::unique_ptr<AConnection<T>> m_connection;
			// This is the thread safe queue of incoming messages from server
			Queue<owned_message<T>> m_qMessagesIn;
		};
    }
}