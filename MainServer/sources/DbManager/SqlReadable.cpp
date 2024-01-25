/*
** EPITECH PROJECT, 2023
** RType
** File description:
** SqlReadable
*/

#include "SqlReadable.hpp"

rtype::SqlReadable::SqlReadable()
{
}

rtype::SqlReadable::~SqlReadable()
{
}

std::vector<std::map<std::string, std::string>> rtype::SqlReadable::convert_sql_result_to_map(MYSQL_RES *result, std::string conversion_for)
{
    if (result == nullptr)
        throw std::runtime_error("Error: the result of the sql query is null");

    if (conversion_functions.find(conversion_for) == conversion_functions.end())
        throw std::runtime_error("Error: the conversion function for the table " + conversion_for + " doesn't exist");

    return (this->*conversion_functions[conversion_for])(result);
}

std::vector<std::map<std::string, std::string>> rtype::SqlReadable::convert_users_table(MYSQL_RES *result)
{
    MYSQL_ROW row;
    std::map<std::string, std::string> map;
    std::vector<std::map<std::string, std::string>> vector;
    std::vector<std::string> columns = {"id", "username", "password", "created_at"};

    if (mysql_num_rows(result) <= 0) {
        std::cout << "Error: the users table is empty" << std::endl;
        return vector;
    }

    while ((row = mysql_fetch_row(result))) {
        if (mysql_num_fields(result) != columns.size())
            throw std::runtime_error("Error: the users table has not the good number of columns");
        for (int i = 0; i < columns.size(); i++)
            map[columns[i]] = row[i];
        vector.push_back(map);
    }
    return vector;
}

std::vector<std::map<std::string, std::string>> rtype::SqlReadable::convert_binds_table(MYSQL_RES *result)
{
    MYSQL_ROW row;
    std::map<std::string, std::string> map;
    std::vector<std::map<std::string, std::string>> vector;
    std::vector<std::string> columns = {"id", "user_id", "forward", "backward", "left", "right", "shoot", "scope", "reload", "run", "swap", "jump", "pause", "chat", "score", "map", "mute", "quit"};

    if (mysql_num_rows(result) <= 0) {
        std::cout << "Error: the binds table is empty" << std::endl;
        return vector;
    }

    while ((row = mysql_fetch_row(result))) {
        if (mysql_num_fields(result) != columns.size())
            throw std::runtime_error("Error: the binds table has not the right number of columns");
        for (int i = 0; i < columns.size(); i++)
            map[columns[i]] = row[i];
        vector.push_back(map);
    }
    return vector;
}

std::vector<std::map<std::string, std::string>> rtype::SqlReadable::convert_mouse_settings_table(MYSQL_RES *result)
{
    MYSQL_ROW row;
    std::map<std::string, std::string> map;
    std::vector<std::map<std::string, std::string>> vector;
    std::vector<std::string> columns = {"id", "user_id", "sensitivity", "invert_y", "invert_x", "mouse_acceleration", "mouse_smoothing", "mouse_filtering", "mouse_acceleration_x", "mouse_acceleration_y", "mouse_smoothing_x", "mouse_smoothing_y", "mouse_filtering_x", "mouse_filtering_y"};

    if (mysql_num_rows(result) <= 0) {
        std::cout << "Error: the mouse_settings table is empty" << std::endl;
        return vector;
    }

    while ((row = mysql_fetch_row(result))) {
        if (mysql_num_fields(result) != columns.size())
            throw std::runtime_error("Error: the mouse_settings table has not the right number of columns");
        for (int i = 0; i < columns.size(); i++)
            map[columns[i]] = row[i];
        vector.push_back(map);
    }
    return vector;
}

std::vector<std::map<std::string, std::string>> rtype::SqlReadable::convert_display_settings_table(MYSQL_RES *result)
{
    MYSQL_ROW row;
    std::map<std::string, std::string> map;
    std::vector<std::map<std::string, std::string>> vector;
    std::vector<std::string> columns = {"id", "user_id", "resolution", "framerate", "vsync", "fov", "brightness", "gamma", "contrast", "saturation", "sharpness", "colorblind"};

    if (mysql_num_rows(result) <= 0) {
        std::cout << "Error: the display_settings table is empty" << std::endl;
        return vector;
    }

    while ((row = mysql_fetch_row(result))) {
        if (mysql_num_fields(result) != columns.size())
            throw std::runtime_error("Error: the display_settings table has not the right number of columns");
        for (int i = 0; i < columns.size(); i++)
            map[columns[i]] = row[i];
        vector.push_back(map);
    }
    return vector;
}

std::vector<std::map<std::string, std::string>> rtype::SqlReadable::convert_audio_settings_table(MYSQL_RES *result)
{
    MYSQL_ROW row;
    std::map<std::string, std::string> map;
    std::vector<std::map<std::string, std::string>> vector;
    std::vector<std::string> columns = {"id", "user_id", "master", "music", "effects", "voice", "voice_chat", "voice_chat_push_to_talk"};

    if (mysql_num_rows(result) <= 0) {
        std::cout << "Error: the audio_settings table is empty" << std::endl;
        return vector;
    }

    while ((row = mysql_fetch_row(result))) {
        if (mysql_num_fields(result) != columns.size())
            throw std::runtime_error("Error: the audio_settings table has not the right number of columns");
        for (int i = 0; i < columns.size(); i++)
            map[columns[i]] = row[i];
        vector.push_back(map);
    }
    return vector;
}

std::vector<std::map<std::string, std::string>> rtype::SqlReadable::convert_profile_table(MYSQL_RES *result)
{
    MYSQL_ROW row;
    std::map<std::string, std::string> map;
    std::vector<std::map<std::string, std::string>> vector;
    std::vector<std::string> columns = {"id", "user_id", "name", "level"};

    if (mysql_num_rows(result) <= 0) {
        std::cout << "Error: the profile table is empty" << std::endl;
        return vector;
    }

    while ((row = mysql_fetch_row(result))) {
        if (mysql_num_fields(result) != columns.size())
            throw std::runtime_error("Error: the profile table has not the right number of columns");
        for (int i = 0; i < columns.size(); i++)
            map[columns[i]] = row[i];
        vector.push_back(map);
    }
    return vector;
}

std::vector<std::map<std::string, std::string>> rtype::SqlReadable::convert_game_history_table(MYSQL_RES *result)
{
    MYSQL_ROW row;
    std::map<std::string, std::string> map;
    std::vector<std::map<std::string, std::string>> vector;
    std::vector<std::string> columns = {"id", "duration", "start_time", "end_time", "game_mode", "map", "winner", "player1_id", "player2_id", "player3_id", "player4_id", "player5_id", "player6_id", "player7_id", "player8_id", "player9_id", "player10_id"};

    if (mysql_num_rows(result) <= 0) {
        std::cout << "Error: the game_history table is empty" << std::endl;
        return vector;
    }

    while ((row = mysql_fetch_row(result))) {
        if (mysql_num_fields(result) != columns.size())
            throw std::runtime_error("Error: the game_history table has not the right number of columns");
        for (int i = 0; i < columns.size(); i++)
            map[columns[i]] = row[i];
        vector.push_back(map);
    }
    return vector;
}

std::vector<std::map<std::string, std::string>> rtype::SqlReadable::convert_game_history_player_table(MYSQL_RES *result)
{
    MYSQL_ROW row;
    std::map<std::string, std::string> map;
    std::vector<std::map<std::string, std::string>> vector;
    std::vector<std::string> columns = {"id", "game_history_id", "user_id", "team" , "kills", "deaths", "assists", "damage", "shots", "hits", "headshots", "accuracy", "score"};

    if (mysql_num_rows(result) <= 0) {
        std::cout << "Error: the game_history_player table is empty" << std::endl;
        return vector;
    }

    while ((row = mysql_fetch_row(result))) {
        if (mysql_num_fields(result) != columns.size())
            throw std::runtime_error("Error: the game_history_player table has not the right number of columns");
        for (int i = 0; i < columns.size(); i++)
            map[columns[i]] = row[i];
        vector.push_back(map);
    }
    return vector;
}

std::vector<std::map<std::string, std::string>> rtype::SqlReadable::convert_friendship_table(MYSQL_RES *result)
{
    MYSQL_ROW row;
    std::map<std::string, std::string> map;
    std::vector<std::map<std::string, std::string>> vector;
    std::vector<std::string> columns = {"id", "user_id_1", "user_id_2", "status"};

    if (mysql_num_rows(result) <= 0) {
        std::cout << "Error: the friendship table is empty" << std::endl;
        return vector;
    }

    while ((row = mysql_fetch_row(result))) {
        if (mysql_num_fields(result) != columns.size())
            throw std::runtime_error("Error: the friendship table has not the right number of columns");
        for (int i = 0; i < columns.size(); i++)
            map[columns[i]] = row[i];
        vector.push_back(map);
    }
    return vector;
}