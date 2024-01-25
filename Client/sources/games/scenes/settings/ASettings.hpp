/*
** EPITECH PROJECT, 2024
** RType
** File description:
** ASettings
*/

#pragma once
    #include "ContainerSettings.hpp"
    #include "../../../net/ClientToServer.hpp"

namespace rtype {
    class ASettings : public ECS::Ecs3D::AScene {
        public:
            virtual ~ASettings() = default;
            virtual void draw() = 0;
            virtual void update(float deltatime, float getTime) = 0;
            void drawTextInRectangle(std::vector<std::string> texts, std::vector<std::string> values, Rectangle rectangle, Font font, Color color, float fontSize);
            std::map<std::string, std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>>> createBindInBox(std::initializer_list<std::string> texts, std::initializer_list<std::string> values, Rectangle rectangle, Font font, Color textColor, float fontSize);
            int WhichBindisClikcked(std::map<std::string, std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>>> map);
            void UpdateBinding(std::map<std::string, std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>>> &map, int &rectVisible, std::string whereAmI, rtype::ContainerSettings &containerSettings);
            void UpdateCoche(std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> coches, std::vector<bool> &enabled, std::string whereAmI, rtype::ContainerSettings &containerSettings);
            void UpdateSelect(std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> selects, std::vector<Color> &tints, std::string whereAmI, rtype::ContainerSettings &containerSettings);
            void UpdateJaugeBar(std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> bars, std::vector<float> &level, std::string whereAmI, rtype::ContainerSettings &containerSettings, rtype::SoundManager &soundManager);
            void UpdateIncrement(std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> increments, std::vector<int> &value, std::string whereAmI, rtype::ContainerSettings &containerSettings);
            void DrawCoche(std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> coches, std::vector<bool> enabled, Font font);
            void DrawJaugeBar(std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> bars, std::vector<float> level, Font font);
            void DrawIncrement(std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> increments, std::vector<int> value, Font font);
            void DrawSelect(std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> selects, std::vector<Color> tints, Font font);
            void UpdateBindStruct(std::string index, int value, rtype::ContainerSettings &containerSettings);
            void UpdateMouseSettingsStruct(std::string index, int value, rtype::ContainerSettings &containerSettings);
            void UpdateAudioStruct(std::string index, int value, rtype::ContainerSettings &containerSettings, rtype::SoundManager &sounds);
            void UpdateVideoSettingsStruct(std::string index, std::string value, rtype::ContainerSettings &containerSettings);
            void UpdateProfileStruct(std::string index, std::string value, rtype::ContainerSettings &containerSettings);
            void ChooseGoodUpdateStruct(std::string whereAmI, std::string index, std::string value, rtype::ContainerSettings &containerSettings, rtype::SoundManager &sounds);
            void sendSettingsToServer(std::unique_ptr<rtype::ClientToServer> &clientToServer, int id, rtype::ContainerSettings &containerSettings);
            void AppliedSettings(std::unique_ptr<rtype::ClientToServer> &clientToServer, int id, rtype::ContainerSettings &co);
            void WantToSave(bool wantToSave, std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> texts, Font font);
            void IfSaving(bool &wantToSave, std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> texts, std::unique_ptr<rtype::ClientToServer> &clientToServer, int id, _Scene &scene, rtype::ContainerSettings &containerSettings, _Scene lastScene);

        protected:
            binds_s _binds;
            mouse_settings_s _mouseSettings;
            audio_settings_s _audio;
            display_settings_s _videoSettings;
            profile_s _profile;
            binds_s _old_binds;
            mouse_settings_s _old_mouseSettings;
            audio_settings_s _old_audio;
            display_settings_s _old_videoSettings;
            profile_s _old_profile;
            bool _FirstSave = false;
        private:
    };
};
