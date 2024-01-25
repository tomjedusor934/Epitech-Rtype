/*
** EPITECH PROJECT, 2024
** RType
** File description:
** career
*/

#ifndef CAREER_HPP_
    #define CAREER_HPP_
    #include "History.hpp"

namespace rtype {

    class Career : public ECS::Ecs3D::AScene {
        public:
            Career(std::unique_ptr<rtype::ClientToServer> &clientToServer, std::unique_ptr<rtype::GameManager> &clientToGame, _Scene &scene, int &me, int &lobby_id, bool &shouldExit, SoundManager &soundManager, std::map<std::string, std::vector<int>> &historyValues)
                : _clientToServer(clientToServer), _clientToGame(clientToGame), _scene(scene), _me(me), _lobby_id(lobby_id), _shouldExit(shouldExit), _soundManager(soundManager), _historyValues(historyValues) {
                    _history = rtype::History(400, 200, 7);
                };
            ~Career() {};

            void draw() override;
            void update(float deltatime, float getime) override;

        protected:
        private:
            std::unique_ptr<rtype::ClientToServer> &_clientToServer;
            std::unique_ptr<rtype::GameManager> &_clientToGame;

            SoundManager &_soundManager;

            _Scene &_scene;
            int &_me;
            int &_lobby_id;
            bool &_shouldExit;
            bool _start = false;

            rtype::History _history;
            std::map<std::string, std::vector<int>> &_historyValues;
    };
}

#endif /* !CAREER_HPP_ */
