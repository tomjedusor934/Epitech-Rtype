/*
** EPITECH PROJECT, 2024
** RType
** File description:
** GameScene
*/

#pragma once
#include "../../../../includes/MainInclusdes.hpp"
#include "../../../net/ClientToServer.hpp"
#include "../../../net/ClientToGame.hpp"
#include "../../Game.hpp"
#include "GameLogic.hpp"

namespace rtype {
    class GameScene : public ECS::Ecs3D::AScene {
        public:
            GameScene(std::unique_ptr<rtype::ClientToServer> &clientToServer, std::unique_ptr<rtype::GameManager> &clientToGame, _Scene &scene, GameLogic &gameLogic, int &_id, SoundManager &soundManager, int &_lobby_Id, bool &shouldExit, rtype::ContainerSettings &co)
                : _clientToServer(clientToServer), _clientToGame(clientToGame), _scene(scene), _gameLogic(gameLogic), _id(_id), _soundManager(soundManager), _lobbyId(_lobby_Id), _shouldExit(shouldExit), _binds(co.getBinds()), _audioSettings(co.getAudioSettings()), _mouseSettings(co.getMouseSettings()) {};
            ~GameScene();

            Vector3 HandleMovement(std::unique_ptr<rtype::GameManager> &clientToGame, float deltatime);
            Vector3 HandleRotation(std::unique_ptr<rtype::GameManager> &clientToGame, float deltatime);

            void HandleShoot(std::unique_ptr<rtype::GameManager> &clientToGame);
            Vector3 CheckCollisionAndMove(Vector3 desiredMovement, float deltatime);

            void drawHealthPlayer(std::shared_ptr<PlayerLogic> player, int id);
            void drawAmmoPlayer(std::shared_ptr<PlayerLogic> player, int id);
            void drawStatsGame();
            void drawPlayerAth();

            std::string FormatFloatToString(float timer);

            void draw() override;
            void update(float deltatime, float getTime) override;

            std::unique_ptr<rtype::ClientToServer> &_clientToServer;
            std::unique_ptr<rtype::GameManager> &_clientToGame;

            SoundManager &_soundManager;

            _Scene &_scene;
            GameLogic &_gameLogic;

            int &_id;
            int &_lobbyId;
            bool &_shouldExit;

            void updateTick(float deltatime);

            double lastUpdate = 0;
            bool shouldUpdate = false;

            // Data to send
            Vector3 move = {0, 0, 0};
            Vector3 rota = {0, 0, 0};
            bool shoot = false;
            bool jump = false;

            float _elapsedTimer = 0;
            float _timer = 0;

            bool mapIsLoaded = false;

        private:
            binds_t &_binds;
            audio_settings_t &_audioSettings;
            mouse_settings_t &_mouseSettings;
            bool _pause = false;
    };
}