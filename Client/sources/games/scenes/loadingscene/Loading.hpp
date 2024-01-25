/*
** EPITECH PROJECT, 2024
** RType
** File description:
** loading
*/

#ifndef LOADING_HPP_
#define LOADING_HPP_

#include "../../../../includes/MainInclusdes.hpp"
#include "../../../net/ClientToServer.hpp"
#include "../../../net/ClientToGame.hpp"

#include "../../Game.hpp"

namespace rtype {

    class Loading : public ECS::Ecs3D::AScene {
        public:
            Loading(std::unique_ptr<rtype::ClientToServer> &clientToServer, std::unique_ptr<rtype::GameManager> &clientToGame, _Scene &scene, int &_me, int &_lobby_id, Font &myFont, SoundManager &soundManager) : _clientToServer(clientToServer), _clientToGame(clientToGame), _scene(scene), _me(_me), _lobby_id(_lobby_id), _myFont(myFont), _soundManager(soundManager) {};
            ~Loading();

            void draw() override;
            void update(float deltatime, float getTime) override;

            std::unique_ptr<rtype::ClientToServer> &_clientToServer;
            std::unique_ptr<rtype::GameManager> &_clientToGame;

            SoundManager &_soundManager;

            _Scene &_scene;

            int &_me; // (id)
            int &_lobby_id;

            Font _myFont;

        protected:
        private:
    };
}

#endif /* !LOADING_HPP_ */
