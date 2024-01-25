/*
** EPITECH PROJECT, 2024
** RType
** File description:
** VideoSettings
*/

#pragma once
    #include "ASettings.hpp"
    #include "../../../../includes/MainInclusdes.hpp"

namespace rtype {
    class VideoSettings : public ASettings {
        public:
            VideoSettings(std::unique_ptr<rtype::ClientToServer> &clientToServer, std::unique_ptr<rtype::GameManager> &clientToGame, _Scene &scene, int &id, std::map<std::string, std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>>> entities, rtype::ContainerSettings &containerSettings, _Scene lastScene = _Scene::LOBBY)
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
                        if (entity.first == "Increment") {
                            _increments = entity.second;
                            int size = _increments.size() / 3 + 1;
                            for (int i = 0; i < size; i++) {
                                _incrementsValue.push_back(240);
                            }
                        }
                        if (entity.first == "Select") {
                            _selects = entity.second;
                            for (auto &select : _selects) {
                                _selectsTint.push_back(GRAY);
                            }
                        }
                    }
                    _isSaving = false;
                    _lastScene = lastScene;
                };
            ~VideoSettings() {};
            void draw();
            void update(float deltatime, float getTime);
            void updateVideo();

        protected:
        private:
            std::unique_ptr<rtype::ClientToServer> &_clientToServer;
            std::unique_ptr<rtype::GameManager> &_clientToGame;
            std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> _coches;
            std::vector<bool> _cochesBool;
            std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> _jaugeBars;
            std::vector<float> _jaugeBarsLevel;
            std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> _increments;
            std::vector<int> _incrementsValue;
            std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> _selects;
            std::vector<Color> _selectsTint;

            _Scene &_scene;
            _Scene _lastScene;
            int &_id;
            bool _isSaving;
            bool _start = false;

            rtype::ContainerSettings &_containerSettings;
            rtype::SoundManager soundManager;
    };
};
