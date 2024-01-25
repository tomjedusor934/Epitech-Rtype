/*
** EPITECH PROJECT, 2023
** area_network_framework
** File description:
** net_message
*/

#pragma once
#include "NetCommon.hpp"

namespace rtype
{
	namespace net
	{
		// Message Header is sent at start of all messages. The template allows us
		// to use "enum class" to ensure that the messages are valid at compile time
		template <typename T>
		struct message_header
		{
			T id{};
			uint32_t size = 0;
			unsigned int packetNb = 0;
		};

		// Message Body contains a header and a std::vector, containing raw bytes
		// of infomation. This way the message can be variable length, but the size
		// in the header must be updated.
		template <typename T>
		struct message
		{
			// Header & Body vector
			message_header<T> header{};
			std::vector<uint8_t> body;

			// returns size of entire message packet in bytes
			size_t size() const
			{
				return body.size();
			}

			unsigned int getPacketNb()
			{
				return header.packetNb;
			}

			void setPacketNb(unsigned int packetNb)
			{
				header.packetNb = packetNb;
			}

			void clear()
			{
				header.size = 0;
				body.clear();
			}

			// Override for std::cout compatibility - produces friendly description of message
			friend std::ostream& operator << (std::ostream& os, const message<T>& msg)
			{
				os << "ID: " << int(msg.header.id) << " Size: " << msg.header.size;
				return os;
			}

			// Convenience Operator overloads - These allow us to add and remove stuff from
			// the body vector as if it were a stack, so First in, Last Out. These are a
			// template in itself, because we dont know what data type the user is pushing or
			// popping, so lets allow them all. NOTE: It assumes the data type is fundamentally
			// Plain Old Data (POD). TLDR: Serialise & Deserialise into/from a vector

			// Pushes any POD-like data into the message buffer
			template<typename DataType>
			friend message<T>& operator << (message<T>& msg, const DataType& data)
			{
				// Check that the type of the data being pushed is trivially copyable
				static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

				// Cache current size of vector, as this will be the point we insert the data
				size_t i = msg.body.size();

				// Resize the vector by the size of the data being pushed
				msg.body.resize(msg.body.size() + sizeof(DataType));

				// Physically copy the data into the newly allocated vector space
				std::memcpy(msg.body.data() + i, &data, sizeof(DataType));

				// Recalculate the message size
				msg.header.size = msg.size();

				// Return the target message so it can be "chained"
				return msg;
			}

			// pushes a string into the message buffer
			friend message<T>& operator << (message<T>& msg, const std::string& data)
			{
				// Cache current size of vector, as this will be the point we insert the data
				size_t i = msg.body.size();

				msg.body.resize(msg.body.size() + data.size());

				// Physically copy the data into the newly allocated vector space
				for (int j = 0; j < data.size(); j++)
					msg.body[i + j] = data[j];

				// Recalculate the message size
				msg.header.size = msg.size();

				// Return the target message so it can be "chained"
				return msg;
			}

			// Pulls any POD-like data form the message buffer
			template<typename DataType>
			friend message<T>& operator >> (message<T>& msg, DataType& data)
			{
				// Check that the type of the data being pushed is trivially copyable
				static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pulled from vector");

				// Cache the location towards the end of the vector where the pulled data starts
				size_t i = msg.body.size() - sizeof(DataType);

				// Physically copy the data from the vector into the user variable
				std::memcpy(&data, msg.body.data() + i, sizeof(DataType));

				// Shrink the vector to remove read bytes, and reset end position
				msg.body.resize(i);

				// Recalculate the message size
				msg.header.size = msg.size();

				// Return the target message so it can be "chained"
				return msg;
			}

			// Pushes any std::vector or std::string of POD-like data into the message buffer (already resized)
			template<typename DataType>
			friend message<T>& operator >> (message<T> &msg, std::vector<DataType>& data)
			{
				// Check that the type of the data being pushed is trivially copyable
				static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pulled from vector");

				// Cache the location towards the end of the vector where the pulled data starts
				size_t i = msg.body.size() - (data.size() * sizeof(DataType));

				// Physically copy the data from the vector into the user variable
				std::memcpy(data.data(), msg.body.data() + i, data.size() * sizeof(DataType));

				// Shrink the vector to remove read bytes, and reset end position
				msg.body.resize(i);

				// Recalculate the message size
				msg.header.size = msg.size();

				// Return the target message so it can be "chained"
				return msg;
			}

			// same but for std::string
			friend message<T>& operator >> (message<T> &msg, std::string& data)
			{
				// Cache the location towards the end of the vector where the pulled data starts
				size_t i = msg.body.size() - data.size();

				// Physically copy the data from the vector into the user variable
				for (int j = 0; j < data.size(); j++)
					data[j] = msg.body[i + j];

				// Shrink the vector to remove read bytes, and reset end position
				msg.body.resize(i);

				// Recalculate the message size
				msg.header.size = msg.size();

				// Return the target message so it can be "chained"
				return msg;
			}

		};


		// An "owned" message is identical to a regular message, but it is associated with
		// a AConnection. On a server, the owner would be the client that sent the message,
		// on a client the owner would be the server.

		// Forward declare the AConnection
		template <typename T>
		class AConnection;

		template <typename T>
		struct owned_message
		{
			std::shared_ptr<AConnection<T>> remote = nullptr;
			message<T> msg;

			// Again, a friendly string maker
			friend std::ostream& operator<<(std::ostream& os, const owned_message<T>& msg)
			{
				os << msg.msg;
				return os;
			}
		};
	}
}
