/*
** EPITECH PROJECT, 2024
** RType
** File description:
** loading
*/

#ifndef RESULT_HPP_
#define RESULT_HPP_

#include "../../../../includes/MainInclusdes.hpp"
#include "../../../net/ClientToServer.hpp"
#include "../../../net/ClientToGame.hpp"

#include "../../Game.hpp"

namespace rtype {

    class ResultScene : public ECS::Ecs3D::AScene {
        public:
            ResultScene(std::unique_ptr<rtype::ClientToServer> &clientToServer, std::unique_ptr<rtype::GameManager> &clientToGame, _Scene &scene, int &_me, int &_lobby_id, Font &myFont, SoundManager &soundManager, GameLogic &gameLogic, int &_id) : _clientToServer(clientToServer), _clientToGame(clientToGame), _scene(scene), _me(_me), _lobby_id(_lobby_id), _myFont(myFont), _soundManager(soundManager), _gameLogic(gameLogic), _id(_id) {};
            ~ResultScene();

            void draw() override;
            void update(float deltatime, float getTime) override;
            void resetGame();

            std::unique_ptr<rtype::ClientToServer> &_clientToServer;
            std::unique_ptr<rtype::GameManager> &_clientToGame;

            GameLogic &_gameLogic;

            SoundManager &_soundManager;

            _Scene &_scene;

            int &_me; // (id)
            int &_lobby_id;

            int _id;

            Font _myFont;

        protected:
        private:
    };
}

#endif /* !RESULT_HPP_ */
