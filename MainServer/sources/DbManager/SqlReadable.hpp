/*
** EPITECH PROJECT, 2023
** RType
** File description:
** SqlReadable
*/

#pragma once
    #include "../../includes/CommonIncludes.hpp"

namespace rtype {
    class SqlReadable {
        public:
            SqlReadable();
            ~SqlReadable();

            std::vector<std::map<std::string, std::string>> convert_sql_result_to_map(MYSQL_RES *result, std::string conversion_for);

        private:
            std::vector<std::map<std::string, std::string>> convert_users_table(MYSQL_RES *result);
            std::vector<std::map<std::string, std::string>> convert_binds_table(MYSQL_RES *result);
            std::vector<std::map<std::string, std::string>> convert_mouse_settings_table(MYSQL_RES *result);
            std::vector<std::map<std::string, std::string>> convert_display_settings_table(MYSQL_RES *result);
            std::vector<std::map<std::string, std::string>> convert_audio_settings_table(MYSQL_RES *result);
            std::vector<std::map<std::string, std::string>> convert_profile_table(MYSQL_RES *result);
            std::vector<std::map<std::string, std::string>> convert_game_history_table(MYSQL_RES *result);
            std::vector<std::map<std::string, std::string>> convert_game_history_player_table(MYSQL_RES *result);
            std::vector<std::map<std::string, std::string>> convert_friendship_table(MYSQL_RES *result);

        private:
            // tableau de pointeur sur fonctions qui me permet d'appeler la fonction de conversion en fonction du type de conversion
            std::map<std::string, std::vector<std::map<std::string, std::string>> (SqlReadable::*)(MYSQL_RES *)> conversion_functions = {
                {"users", &SqlReadable::convert_users_table},
                {"binds", &SqlReadable::convert_binds_table},
                {"mouse_settings", &SqlReadable::convert_mouse_settings_table},
                {"display_settings", &SqlReadable::convert_display_settings_table},
                {"audio_settings", &SqlReadable::convert_audio_settings_table},
                {"profile", &SqlReadable::convert_profile_table},
                {"game_history", &SqlReadable::convert_game_history_table},
                {"game_history_player", &SqlReadable::convert_game_history_player_table},
                {"friendships", &SqlReadable::convert_friendship_table}
            };
    };
} // namespace rtype