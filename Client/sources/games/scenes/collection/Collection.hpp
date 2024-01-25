/*
** EPITECH PROJECT, 2024
** RType
** File description:
** store
*/

#ifndef COLLECTION_HPP_
    #define COLLECTION_HPP_
    #include "../../../../includes/MainInclusdes.hpp"

namespace rtype {

    class Collection : public ECS::Ecs3D::AScene {
        public:
            Collection(std::unique_ptr<rtype::ClientToServer> &clientToServer, std::unique_ptr<rtype::GameManager> &clientToGame, _Scene &scene, int &me, int &lobby_id, bool &shouldExit, SoundManager &soundManager)
                : _clientToServer(clientToServer), _clientToGame(clientToGame), _scene(scene), _me(me), _lobby_id(lobby_id), _shouldExit(shouldExit), _soundManager(soundManager) {};
            ~Collection() {};

            void draw() override;
            void update(float deltatime, float gettime) override;

        protected:
        private:
            std::unique_ptr<rtype::ClientToServer> &_clientToServer;
            std::unique_ptr<rtype::GameManager> &_clientToGame;
            _Scene &_scene;

            SoundManager &_soundManager;

            int &_me;
            int &_lobby_id;
            bool &_shouldExit;
    };
}

#endif /* !COLLECTION_HPP_ */
