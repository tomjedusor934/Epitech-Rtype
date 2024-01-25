/*
** EPITECH PROJECT, 2024
** RType
** File description:
** ProfilSettings
*/

#pragma once
    #include "ASettings.hpp"
    #include "../../../../includes/MainInclusdes.hpp"

namespace rtype {
    class ProfilSettings : public ASettings {
        public:
            ProfilSettings(std::unique_ptr<rtype::ClientToServer> &clientToServer, std::unique_ptr<rtype::GameManager> &clientToGame, _Scene &scene, int &id, rtype::ContainerSettings &containerSettings, Font font, _Scene lastScene = _Scene::LOBBY)
                : _clientToServer(clientToServer), _clientToGame(clientToGame), _scene(scene), _id(id), _containerSettings(containerSettings) {
                    this->_isSaving = false;
                    this->_editName = false;
                    this->_lastScene = lastScene;
                    this->_close = rtype::ECS::Ecs3D::createButtonWithImage("close", "./Media/close.png", {(float)GetScreenWidth() / 2 + 150, (float)GetScreenHeight() / 2 - 50, 22.0f, 22.0f});
                    this->_buttoneditName = rtype::ECS::Ecs3D::createBoxWithText("buttonAddFriend", {(float)GetScreenWidth() / 2 + 150, (float)GetScreenHeight() / 2 - 50, 100, 40}, "Ajouter", font, 20, WHITE, 0);
                };
            ~ProfilSettings() {};
            void draw();
            void update(float deltatime, float getTime);
            void drawChangeName();
            void updateChangeName();
        protected:
        private:
            std::unique_ptr<rtype::ClientToServer> &_clientToServer;
            std::unique_ptr<rtype::GameManager> &_clientToGame;

            _Scene &_scene;
            _Scene _lastScene;
            int &_id;
            bool _isSaving;
            bool _editName;

            rtype::ContainerSettings &_containerSettings;
            std::shared_ptr<rtype::ECS::Ecs3D::IEntity> _close;
            std::shared_ptr<rtype::ECS::Ecs3D::IEntity> _buttoneditName;
    };
};
