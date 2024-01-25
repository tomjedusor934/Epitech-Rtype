/*
** EPITECH PROJECT, 2023
** RType
** File description:
** ClientToServer
*/

#pragma once
    #include "../../includes/CommonIncludes.hpp"
    #include "ClientToGame.hpp"
    #include "../games/scenes/settings/ContainerSettings.hpp"
    #include "../games/teamContainer/TeamContainer.hpp"

namespace rtype {
    class ClientToServer : public rtype::net::tcp::NetClient<MainServerMsgTypes>
    {
        public:
            ClientToServer(_Scene &scene, int &id, int &lobby_id, std::unique_ptr<rtype::GameManager> &client, rtype::ContainerSettings &containerSettings, rtype::TeamContainer &teamContainer, std::map<std::string, std::vector<int>> &historyValues)
                : _scene(scene), _id(id), _lobby_id(lobby_id), _clientToGame(client), _containerSettings(containerSettings), _teamContainer(teamContainer), _historyValues(historyValues), _alreadyConnectedOneTime(false)
            {};
            ~ClientToServer()
            {};

            void handleMessages()
            {
                rtype::net::message<MainServerMsgTypes> msg;

                while (!this->Incoming().empty()) {
                    msg = this->m_qMessagesIn.pop_front().msg;
                    if (this->_MsgHandlers.find(msg.header.id) != this->_MsgHandlers.end()) {
                        (this->*_MsgHandlers[msg.header.id])(msg);
                    } else {
                        std::cerr << "Message not handled" << std::endl;
                        std::cerr << "Message id: " << msg << std::endl;
                    }
                }
            }

        private:
            //* in this private section implement all the functions that will be called when we receive a message with the right type (MainServerMsgTypes)
            //! then when new function is added add the function in the map in the private section below
            void LoginSuccess(rtype::net::message<MainServerMsgTypes> &msg);
            void LoginFailed(rtype::net::message<MainServerMsgTypes> &msg);

            void DisplaySettings(rtype::net::message<MainServerMsgTypes> &msg);
            void AudioSettings(rtype::net::message<MainServerMsgTypes> &msg);
            void Binds(rtype::net::message<MainServerMsgTypes> &msg);
            void Profile(rtype::net::message<MainServerMsgTypes> &msg);
            void MouseSettings(rtype::net::message<MainServerMsgTypes> &msg);

            void ConnectToGameServer(rtype::net::message<MainServerMsgTypes> &msg);

            void LobbyInfos(rtype::net::message<MainServerMsgTypes> &msg);

            void FriendsInfos(rtype::net::message<MainServerMsgTypes> &msg);

            void FriendLeave(rtype::net::message<MainServerMsgTypes> &msg);

            void PendingFriendRequest(rtype::net::message<MainServerMsgTypes> &msg);

            void NewFriendConnected(rtype::net::message<MainServerMsgTypes> &msg);

            void PlayerInLobbyIsReady(rtype::net::message<MainServerMsgTypes> &msg);
            void PlayerInLobbyIsNotReady(rtype::net::message<MainServerMsgTypes> &msg);
            void Alone(rtype::net::message<MainServerMsgTypes> &msg);

            void GameHistory(rtype::net::message<MainServerMsgTypes> &msg);

        private:
            //! here is the map that you have to fill with the right function

            //* this map is used to call the right function when we receive a message with the right type (MainServerMsgTypes)
            std::map<MainServerMsgTypes, void (ClientToServer::*)(rtype::net::message<MainServerMsgTypes> &)> _MsgHandlers =
            {
                { MainServerMsgTypes::LoginSuccess, &ClientToServer::LoginSuccess },
                { MainServerMsgTypes::LoginFailed, &ClientToServer::LoginFailed },
                { MainServerMsgTypes::DisplaySettings, &ClientToServer::DisplaySettings},
                { MainServerMsgTypes::AudioSettings, &ClientToServer::AudioSettings},
                { MainServerMsgTypes::Binds, &ClientToServer::Binds},
                { MainServerMsgTypes::ProfileInfos, &ClientToServer::Profile},
                { MainServerMsgTypes::MouseSettings, &ClientToServer::MouseSettings },
                { MainServerMsgTypes::ConnectToGameServer, &ClientToServer::ConnectToGameServer},
                { MainServerMsgTypes::LobbyInfos, &ClientToServer::LobbyInfos},
                { MainServerMsgTypes::FriendsInfos, &ClientToServer::FriendsInfos},
                { MainServerMsgTypes::LeaveLobby, &ClientToServer::FriendLeave},
                { MainServerMsgTypes::PendingFiendRequest, &ClientToServer::PendingFriendRequest},
                { MainServerMsgTypes::NewFriendConnected, &ClientToServer::NewFriendConnected},
                { MainServerMsgTypes::Ready, &ClientToServer::PlayerInLobbyIsReady},
                { MainServerMsgTypes::NotReady, &ClientToServer::PlayerInLobbyIsNotReady},
                { MainServerMsgTypes::Alone, &ClientToServer::Alone},
                { MainServerMsgTypes::GameHistory, &ClientToServer::GameHistory}
            };

        private:
            _Scene &_scene;
            int &_id;
            int &_lobby_id;

            std::unique_ptr<rtype::GameManager> &_clientToGame;

        private:
            rtype::ContainerSettings &_containerSettings;

            rtype::TeamContainer &_teamContainer;

            std::map<std::string, std::vector<int>> &_historyValues;
        private:
            bool _alreadyConnectedOneTime;
    };
} // namespace rtype