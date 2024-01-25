/*
** EPITECH PROJECT, 2023
** RType
** File description:
** SqlSendable
*/

#pragma once
    #include "../../includes/CommonIncludes.hpp"
namespace rtype {
    class SqlSendable {
        public:
            SqlSendable()
            {
                this->_profile = {0};
                this->_binds = {0};
                this->_mouse_settings = {0};
                this->_display_settings = {0};
                this->_audio_settings = {0};
                this->_game_history = {0};
                this->_game_history_player = {0};
            };
            ~SqlSendable()
            {};

            enum class TYPE {
                USERS,
                BINDS,
                MOUSE_SETTINGS,
                DISPLAY_SETTINGS,
                AUDIO_SETTINGS,
                PROFILE,
                GAME_HISTORY,
                GAME_HISTORY_PLAYER
            };

            void call_conversion_function(std::map<std::string, std::string> &map, void *data, TYPE type)
            {
                if (data == nullptr) {
                    std::cerr << "Data is null" << std::endl;
                    return;
                }

                this->ResetData(type, data);
                if (conversion_functions.find(type) == conversion_functions.end()) {
                    std::cout << "Conversion function not found" << std::endl;
                    return;
                }
                (this->*conversion_functions[type])(map, data);
            };

            void *get_data(TYPE type);

        private:
            profile_t _profile;
            binds_t _binds;
            mouse_settings_t _mouse_settings;
            display_settings_t _display_settings;
            audio_settings_t _audio_settings;
            game_history_t _game_history;
            game_history_player_t _game_history_player;

        private:
            void convert_users_table(std::map<std::string, std::string> &map, void *data);
            void convert_binds_table(std::map<std::string, std::string> &map, void *data);
            void convert_mouse_settings_table(std::map<std::string, std::string> &map, void *data);
            void convert_display_settings_table(std::map<std::string, std::string> &map, void *data);
            void convert_audio_settings_table(std::map<std::string, std::string> &map, void *data);
            void convert_profile_table(std::map<std::string, std::string> &map, void *data);
            void convert_game_history_table(std::map<std::string, std::string> &map, void *data);
            void convert_game_history_player_table(std::map<std::string, std::string> &map, void *data);

            void ResetData(TYPE type, void *data = nullptr);

        private:
            std::map<TYPE, void (SqlSendable::*)(std::map<std::string, std::string> &, void *)> conversion_functions = {
                {TYPE::USERS, &SqlSendable::convert_users_table},
                {TYPE::BINDS, &SqlSendable::convert_binds_table},
                {TYPE::MOUSE_SETTINGS, &SqlSendable::convert_mouse_settings_table},
                {TYPE::DISPLAY_SETTINGS, &SqlSendable::convert_display_settings_table},
                {TYPE::AUDIO_SETTINGS, &SqlSendable::convert_audio_settings_table},
                {TYPE::PROFILE, &SqlSendable::convert_profile_table},
                {TYPE::GAME_HISTORY, &SqlSendable::convert_game_history_table},
                {TYPE::GAME_HISTORY_PLAYER, &SqlSendable::convert_game_history_player_table}
            };
    };
} // namespace rtype