/*
** EPITECH PROJECT, 2024
** RType
** File description:
** ControlsSettings
*/

#pragma once
    #include "ASettings.hpp"
    #include "../../../../includes/MainInclusdes.hpp"

namespace rtype {
    class ControlsSettings : public ASettings {
        public:
            ControlsSettings(std::unique_ptr<rtype::ClientToServer> &clientToServer, std::unique_ptr<rtype::GameManager> &clientToGame, _Scene &scene, int &id, Font font, rtype::ContainerSettings &containerSettings, _Scene lastScene = _Scene::LOBBY)
                : _clientToServer(clientToServer), _clientToGame(clientToGame), _scene(scene), _id(id), _font(font), _containerSettings(containerSettings){
                RecVisible = -1;
                std::initializer_list<std::string> texts = {"Forward", "Backward", "Left", "Right", "Shoot", "Scope", "Reload", "Run", "Swap", "Jump", "Pause", "Chat", "Score", "Map", "Mute", "Quit"};
                std::initializer_list<std::string> values = {"Z", "S", "Q", "D", "Left click", "Right click", "R", "Shift", "Ctrl", "Space", "Esc", "T", "Tab", "M", "M", "Esc"};
                binding = createBindInBox(texts, values, Rectangle{(float)100, (float)150, (float)1600, (float)730}, _font, WHITE, 40.0f);
                _isSaving = false;
                _lastScene = lastScene;
            };
            ~ControlsSettings() {};

            void draw();
            void update(float deltatime, float getTime);
            void updateControls();

        protected:
        private:
            std::unique_ptr<rtype::ClientToServer> &_clientToServer;
            std::unique_ptr<rtype::GameManager> &_clientToGame;
            int RecVisible;
            Font _font;
            std::map<std::string, std::vector<std::shared_ptr<ECS::Ecs3D::IEntity>>> binding;

            _Scene &_scene;
            _Scene _lastScene;
            int &_id;
            bool _isSaving;
            bool _start = false;

            rtype::ContainerSettings &_containerSettings;
    };
};
