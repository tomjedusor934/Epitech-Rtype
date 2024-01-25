/*
** EPITECH PROJECT, 2023
** RType
** File description:
** GameManager
*/

#pragma once
    #include "../../includes/CommonIncludes.hpp"
    #include "../games/scenes/gameScene/GameLogic.hpp"
    #include "../games/soundManager/soundManager.hpp"

namespace rtype
{
    class GameManager: public rtype::net::udp::NetClient<GameServerMsgTypes>
    {
        public:
            GameManager(GameLogic &gameLogic, int &_id, _Scene &scene, SoundManager &soundManager) : _gameLogic(gameLogic), _id(_id), _scene(scene), _soundManager(soundManager)
            {};
            ~GameManager()
            {};
            void handleMessages()
            {
                rtype::net::message<GameServerMsgTypes> msg;

                while (!this->Incoming().empty()) {
                    msg = this->m_qMessagesIn.pop_front().msg;
                    if (this->_MsgHandlers.find(msg.header.id) != this->_MsgHandlers.end()) {
                        (this->*_MsgHandlers[msg.header.id])(msg);
                    } else {
                        std::cerr << "Message not handled" << std::endl;
                    }
                }
            }

            void updateEndpoint(std::string ip, uint16_t port)
            {
                this->m_connection->updateEndpoint(ip, port);
            }

            GameLogic &_gameLogic;
            SoundManager &_soundManager;

            int &_id;
            _Scene &_scene;
        private:
            //* in this private section implement all the functions that will be called when we receive a message with the right type (GameServerMsgTypes)
            //! then when new function is added add the function in the map in the private section below
            void SpawnPosition(rtype::net::message<GameServerMsgTypes> &msg);
            void OtherSpawnPosition(rtype::net::message<GameServerMsgTypes> &msg);
            void PlayerMove(rtype::net::message<GameServerMsgTypes> &msg);
            void OtherPlayerMove(rtype::net::message<GameServerMsgTypes> &msg);
            void PlayerLook(rtype::net::message<GameServerMsgTypes> &msg);
            void OtherPlayerLook(rtype::net::message<GameServerMsgTypes> &msg);
            void PlayerHitted(rtype::net::message<GameServerMsgTypes> &msg);
            void ClientConnected(rtype::net::message<GameServerMsgTypes> &msg);
            void OtherPlayerJump(rtype::net::message<GameServerMsgTypes> &msg);
            void OtherPlayerShoot(rtype::net::message<GameServerMsgTypes> &msg);
            void SomeoneGotHitted(rtype::net::message<GameServerMsgTypes> &msg);
            void NextRound(rtype::net::message<GameServerMsgTypes> &msg);
            void UpdateScoreboard(rtype::net::message<GameServerMsgTypes> &msg);
            void GameEnded(rtype::net::message<GameServerMsgTypes> &msg);

        private:
            //! here is the map that you have to fill with the right function

            //* this map is used to call the right function when we receive a message with the right type (GameServerMsgTypes)
            std::map<GameServerMsgTypes, void (GameManager::*)(rtype::net::message<GameServerMsgTypes> &)> _MsgHandlers = {
                {GameServerMsgTypes::SpawnPosition, &GameManager::SpawnPosition},
                {GameServerMsgTypes::OtherSpawnPosition, &GameManager::OtherSpawnPosition},
                {GameServerMsgTypes::PlayerMove, &GameManager::PlayerMove},
                {GameServerMsgTypes::OtherPlayerMove, &GameManager::OtherPlayerMove},
                {GameServerMsgTypes::PlayerLook, &GameManager::PlayerLook},
                {GameServerMsgTypes::OtherPlayerLook, &GameManager::OtherPlayerLook},
                {GameServerMsgTypes::PlayerHitted, &GameManager::PlayerHitted},
                {GameServerMsgTypes::ClientConnected, &GameManager::ClientConnected},
                {GameServerMsgTypes::OtherPlayerJump, &GameManager::OtherPlayerJump},
                {GameServerMsgTypes::OtherPlayerShoot, &GameManager::OtherPlayerShoot},
                {GameServerMsgTypes::SomeoneGotHitted, &GameManager::SomeoneGotHitted},
                {GameServerMsgTypes::NextRound, &GameManager::NextRound},
                {GameServerMsgTypes::UpdateScoreboard, &GameManager::UpdateScoreboard},
                {GameServerMsgTypes::GameEnded, &GameManager::GameEnded},
            };

    };
} // namespace rtype