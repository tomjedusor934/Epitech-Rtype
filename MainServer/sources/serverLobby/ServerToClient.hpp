/*
** EPITECH PROJECT, 2023
** RType
** File description:
** serverToClient
*/

#pragma once
    #include "../../includes/CommonIncludes.hpp"
    #include "../DbManager/DbManager.hpp"
    #include "../Lobby/Lobby.hpp"

//* this class is used for the communication between the main server and the clients
//* we use the tcp protocol because it's a reliable protocol
//* so we are sure that the messages will be received by the clients like for the login and the register or any other important messages

namespace rtype
{
    class TcpMainServer : public rtype::net::tcp::NetServer<MainServerMsgTypes>
    {
        public:
            TcpMainServer(uint16_t port, std::map<int, rtype::Lobby> &lobbies, std::mutex &mtx, std::string server_name) : rtype::net::tcp::NetServer<MainServerMsgTypes>(port, server_name), _lobbies(lobbies), _mtx(mtx)
            {
                this->_sqlSendable = std::make_unique<SqlSendable>();
                this->_dbManager = std::make_unique<DbManager>();
                this->_dbManager->connect("127.0.0.1", "root", "test", "rtype");

                this->_isServerRunning = true;
            }

            bool OnClientConnect(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> client) override;
            void OnClientDisconnect(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> client) override;

            void OnMessage(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> client, rtype::net::message<MainServerMsgTypes>& msg) override
            {
                if (this->_MsgHandlerMap.find(msg.header.id) != this->_MsgHandlerMap.end())
                    (this->*_MsgHandlerMap[msg.header.id])(client, msg);
                else
                    std::cerr << "Unknown message type" << msg << std::endl;
            }

        public:
            void setServerRunning(bool isServerRunning)
            {
                this->_isServerRunning = isServerRunning;
            }

            bool getServerRunning() const
            {
                return this->_isServerRunning;
            }

        private:
            //* in this private section implement all the functions that will be called when we receive a message with the right type (MainServerMsgTypes)
            //! then when new function is added add the function in the map in the private section below
            void Login(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg);
            void Register(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg);
            void Logout(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg);

            void Ready(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg);
            void NotReady(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg);

            void Profile(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg);
            void GameHistory(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg);
            void Settings(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg);
            void UpdateSettings(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg);

            void DisplaySettings(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg);
            void AudioSettings(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg);
            void Binds(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg);
            void MouseSettings(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg);
            void ProfileInfos(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg);

            void JoinFriend(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg);
            void LeaveLobby(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg);
            void AddFriend(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg);
            void RemoveFriend(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg);
            void AcceptFriend(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg);
            void RefuseFriend(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg);

        private:
            //* in this private section wee develop  all the utils fonctions that we need for the message handlers
            void LoginFailed(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client);
            void LoginSuccesful(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, std::vector<std::map<std::string, std::string>> &profile, profile_t user_profile);

            void Success(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client);

            int RegisterUser(login_t &infos);

            void GetUserSettings(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, std::string &query, MainServerMsgTypes type, rtype::SqlSendable::TYPE convert_type);
            void PrepareSettings(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, void *data, MainServerMsgTypes type, rtype::SqlSendable::TYPE convert_type);

            void GetUserRelations(std::string &userId, players_t &player);
            void SendPendingFriendRequest(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, std::string &userId);
            void SendFriendsInfos(players_t &player, std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, int lobby_id);

            int PrepareRequest(int uid1, int uid2, std::string state);
        private:
            //! here is the map that you have to fill with the right function

            //* this map is used to call the right function when we receive a message with the right type (MainServerMsgTypes)
            std::map<MainServerMsgTypes, void (TcpMainServer::*)(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &, rtype::net::message<MainServerMsgTypes>)> _MsgHandlerMap = {
                { MainServerMsgTypes::Login, &TcpMainServer::Login },
                { MainServerMsgTypes::Register, &TcpMainServer::Register },
                { MainServerMsgTypes::Logout, &TcpMainServer::Logout },
                { MainServerMsgTypes::Ready, &TcpMainServer::Ready },
                { MainServerMsgTypes::NotReady, &TcpMainServer::NotReady },
                { MainServerMsgTypes::Profile, &TcpMainServer::Profile },
                { MainServerMsgTypes::GameHistory, &TcpMainServer::GameHistory },
                { MainServerMsgTypes::Settings, &TcpMainServer::Settings },
                { MainServerMsgTypes::UpdateSettings, &TcpMainServer::UpdateSettings },
                { MainServerMsgTypes::JoinFriend, &TcpMainServer::JoinFriend },
                { MainServerMsgTypes::LeaveLobby, &TcpMainServer::LeaveLobby },
                { MainServerMsgTypes::AddFriend, &TcpMainServer::AddFriend },
                { MainServerMsgTypes::RemoveFriend, &TcpMainServer::RemoveFriend },
                { MainServerMsgTypes::AcceptFriend, &TcpMainServer::AcceptFriend },
                { MainServerMsgTypes::RefuseFriend, &TcpMainServer::RefuseFriend },
                { MainServerMsgTypes::DisplaySettings, &TcpMainServer::DisplaySettings },
                { MainServerMsgTypes::AudioSettings, &TcpMainServer::AudioSettings },
                { MainServerMsgTypes::Binds, &TcpMainServer::Binds },
                { MainServerMsgTypes::MouseSettings, &TcpMainServer::MouseSettings },
                { MainServerMsgTypes::ProfileInfos, &TcpMainServer::ProfileInfos },
            };

        private:
            //? this unique_ptr is used to store the dbManager (the class that is used to communicate with the database)
            std::unique_ptr<DbManager> _dbManager;
            //? this unique_ptr is used to convert the data recieved from a call to the database to the right struct (prensent in the ~/includes/DatabaseIncludes.hpp)
            std::unique_ptr<SqlSendable> _sqlSendable;

            //? this map is used to store the clients that are connected to the server (connected with account so the int is the id of the account and the shared_ptr is the client)
            //? the client who s connected to the server is stored in the lobby map
            //? foa each client is assigned to a lobby
            //? then later it will be possible to him to join friend lobby
            //? its a & because we want to modify the map in the main server
            std::map<int, rtype::Lobby> &_lobbies;

            //? this mutex is used to lock data when we are using it (data shared between threads)
            std::mutex &_mtx;

        private:
            bool _isServerRunning;
    };
} // namespace rtype