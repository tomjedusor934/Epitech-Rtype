/*
** EPITECH PROJECT, 2024
** RType
** File description:
** MouseSettings
*/

#pragma once
    #include "ASettings.hpp"
    #include "../../../../includes/MainInclusdes.hpp"

namespace rtype {
    class MouseSettings : public rtype::ASettings {
        public:
            MouseSettings(std::unique_ptr<rtype::ClientToServer> &clientToServer, std::unique_ptr<rtype::GameManager> &clientToGame, _Scene &scene, int &id, std::map<std::string, std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>>> entities, rtype::ContainerSettings &containerSettings, _Scene lastScene = _Scene::LOBBY)
                : _clientToServer(clientToServer), _clientToGame(clientToGame), _scene(scene), _id(id), _containerSettings(containerSettings){
                    for (auto &entity : entities) {
                        if (entity.first == "Coche") {
                            _coches = entity.second;
                            for (auto &coche : _coches) {
                                _cochesBool.push_back(false);
                            }
                        }
                        if (entity.first == "JaugeBar") {
                            _jaugeBars = entity.second;
                            for (auto &jaugeBar : _jaugeBars) {
                                _jaugeBarsLevel.push_back(1.0f);
                            }
                        }
                    }
                    _isSaving = false;
                    _lastScene = lastScene;
                };
            ~MouseSettings() {};
            void draw();
            void update(float deltatime, float getTime);
            void updateMouse();
        protected:
        private:
            std::unique_ptr<rtype::ClientToServer> &_clientToServer;
            std::unique_ptr<rtype::GameManager> &_clientToGame;
            std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> _coches;
            std::vector<bool> _cochesBool;
            std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> _jaugeBars;
            std::vector<float> _jaugeBarsLevel;

            _Scene &_scene;
            _Scene _lastScene;
            int &_id;
            bool _isSaving;
            bool _start = false;

            rtype::ContainerSettings &_containerSettings;
            rtype::SoundManager soundManager;
    };
};
