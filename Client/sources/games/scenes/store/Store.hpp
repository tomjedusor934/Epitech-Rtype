/*
** EPITECH PROJECT, 2024
** RType
** File description:
** store
*/

#ifndef STORE_HPP_
    #define STORE_HPP_
    #include "../../../../includes/MainInclusdes.hpp"

namespace rtype {

    class Store : public ECS::Ecs3D::AScene {
        public:
            Store(std::unique_ptr<rtype::ClientToServer> &clientToServer, std::unique_ptr<rtype::GameManager> &clientToGame, _Scene &scene, int &me, int &lobby_id, bool &shouldExit, SoundManager &soundManager)
                : _clientToServer(clientToServer), _clientToGame(clientToGame), _scene(scene), _me(me), _lobby_id(lobby_id), _shouldExit(shouldExit), _soundManager(soundManager) {};

            void draw() override;
            void update(float deltatime, float getTime) override;

        protected:
        private:
            std::unique_ptr<rtype::ClientToServer> &_clientToServer;
            std::unique_ptr<rtype::GameManager> &_clientToGame;
            SoundManager &_soundManager;

            _Scene &_scene;
            int &_me;
            int &_lobby_id;
            bool &_shouldExit;
    };
}

#endif /* !STORE_HPP_ */
