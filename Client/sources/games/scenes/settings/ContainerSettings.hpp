/*
** EPITECH PROJECT, 2024
** RType
** File description:
** ContainerSettings
*/

#pragma once
    #include "../../../../includes/CommonIncludes.hpp"

namespace rtype {
    class ContainerSettings {
        public:
            ContainerSettings()
            {
                std::memset(&_displaySettings, 0, sizeof(display_settings_t));
                std::memset(&_audioSettings, 0, sizeof(audio_settings_t));
                std::memset(&_mouseSettings, 0, sizeof(mouse_settings_t));
                std::memset(&_profile, 0, sizeof(profile_t));
                std::memset(&_binds, 0, sizeof(binds_t));
                std::memset(&_old_displaySettings, 0, sizeof(display_settings_t));
                std::memset(&_old_audioSettings, 0, sizeof(audio_settings_t));
                std::memset(&_old_mouseSettings, 0, sizeof(mouse_settings_t));
                std::memset(&_old_profile, 0, sizeof(profile_t));
                std::memset(&_old_binds, 0, sizeof(binds_t));
            };
            ~ContainerSettings() = default;

            //getters
            display_settings_t &getDisplaySettings() { return _displaySettings; };
            audio_settings_t &getAudioSettings() { return _audioSettings; };
            mouse_settings_t &getMouseSettings() { return _mouseSettings; };
            profile_t &getProfile() { return _profile; };
            profile_t *getProfilePtr() { return &_profile; };
            binds_t &getBinds() { return _binds; };
            display_settings_t &getOldDisplaySettings() { return _old_displaySettings; };
            audio_settings_t &getOldAudioSettings() { return _old_audioSettings; };
            mouse_settings_t &getOldMouseSettings() { return _old_mouseSettings; };
            profile_t &getOldProfile() { return _old_profile; };
            binds_t &getOldBinds() { return _old_binds; };

            //setters
            void setDisplaySettings(display_settings_t displaySettings) { _displaySettings = displaySettings; };
            void setAudioSettings(audio_settings_t audioSettings) { _audioSettings = audioSettings; };
            void setMouseSettings(mouse_settings_t mouseSettings) { _mouseSettings = mouseSettings; };
            void setProfile(profile_t profile) { _profile = profile; };
            void setBinds(binds_t binds) { _binds = binds; };
            void setOldDisplaySettings(display_settings_t old_displaySettings) { _old_displaySettings = old_displaySettings; };
            void setOldAudioSettings(audio_settings_t old_audioSettings) { _old_audioSettings = old_audioSettings; };
            void setOldMouseSettings(mouse_settings_t old_mouseSettings) { _old_mouseSettings = old_mouseSettings; };
            void setOldProfile(profile_t old_profile) { _old_profile = old_profile; };
            void setOldBinds(binds_t old_binds) { _old_binds = old_binds; };

        protected:
        private:
            display_settings_t _displaySettings;
            audio_settings_t _audioSettings;
            mouse_settings_t _mouseSettings;
            profile_t _profile;
            binds_t _binds;
            display_settings_t _old_displaySettings;
            audio_settings_t _old_audioSettings;
            mouse_settings_t _old_mouseSettings;
            profile_t _old_profile;
            binds_t _old_binds;
    };
}