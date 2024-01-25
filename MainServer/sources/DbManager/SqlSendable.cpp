/*
** EPITECH PROJECT, 2023
** RType
** File description:
** SqlSendable
*/

#include "SqlSendable.hpp"

// exemple
/*
void test_override_int(void *(&nb))
{
    int *ptr = static_cast<int *>(nb);
    *ptr = 42;
}

*/

void rtype::SqlSendable::convert_users_table(std::map<std::string, std::string> &map, void *data)
{
    login_t *login = static_cast<login_t *>(data);
    std::memcpy(login->username, map["username"].data(), map["username"].size());
    std::memcpy(login->password, map["password"].data(), map["password"].size());
}


/*
typedef struct binds_s {
    int id;
    int user_id;
    int forward;
    int backward;
    int left;
    int right;
    int shoot;
    int scope;
    int reload;
    int run;
    int swap;
    int jump;
    int pause;
    int chat;
    int score;
    int map;
    int mute;
    int quit;
} binds_t;
*/
void rtype::SqlSendable::convert_binds_table(std::map<std::string, std::string> &map, void *data)
{
    // {"id", "user_id", "forward", "backward", "left", "right", "shoot", "scope", "reload", "run", "swap", "jump", "pause", "chat", "score", "map", "mute", "quit"};
    binds_t *binds = static_cast<binds_t *>(data);

    binds->id = std::stoi(map["id"]);
    binds->user_id = std::stoi(map["user_id"]);

    binds->forward = std::stoi(map["forward"]);
    binds->backward = std::stoi(map["backward"]);
    binds->left = std::stoi(map["left"]);
    binds->right = std::stoi(map["right"]);
    binds->shoot = std::stoi(map["shoot"]);
    binds->scope = std::stoi(map["scope"]);
    binds->reload = std::stoi(map["reload"]);
    binds->run = std::stoi(map["run"]);
    binds->swap = std::stoi(map["swap"]);
    binds->jump = std::stoi(map["jump"]);
    binds->pause = std::stoi(map["pause"]);
    binds->chat = std::stoi(map["chat"]);
    binds->score = std::stoi(map["score"]);
    binds->map = std::stoi(map["map"]);
    binds->mute = std::stoi(map["mute"]);
    binds->quit = std::stoi(map["quit"]);
}


/*
typedef struct mouse_settings_s {
    int id;
    int user_id;
    int sensitivity;
    int invert_y;
    int invert_x;
    int mouse_acceleration;
    int mouse_smoothing;
    int mouse_filtering;
    int mouse_acceleration_x;
    int mouse_acceleration_y;
    int mouse_smoothing_x;
    int mouse_smoothing_y;
    int mouse_filtering_x;
    int mouse_filtering_y;
} mouse_settings_t;
*/
void rtype::SqlSendable::convert_mouse_settings_table(std::map<std::string, std::string> &map, void *data)
{
    // {"id", "user_id", "sensitivity", "invert_y", "invert_x", "mouse_acceleration", "mouse_smoothing", "mouse_filtering", "mouse_acceleration_x", "mouse_acceleration_y", "mouse_smoothing_x", "mouse_smoothing_y", "mouse_filtering_x", "mouse_filtering_y"};
    mouse_settings_t *mouse_settings = static_cast<mouse_settings_t *>(data);

    mouse_settings->id = std::stoi(map["id"]);
    mouse_settings->user_id = std::stoi(map["user_id"]);

    mouse_settings->sensitivity = std::stoi(map["sensitivity"]);
    mouse_settings->invert_y = std::stoi(map["invert_y"]);
    mouse_settings->invert_x = std::stoi(map["invert_x"]);
    mouse_settings->mouse_acceleration = std::stoi(map["mouse_acceleration"]);
    mouse_settings->mouse_smoothing = std::stoi(map["mouse_smoothing"]);
    mouse_settings->mouse_filtering = std::stoi(map["mouse_filtering"]);
    mouse_settings->mouse_acceleration_x = std::stoi(map["mouse_acceleration_x"]);
    mouse_settings->mouse_acceleration_y = std::stoi(map["mouse_acceleration_y"]);
    mouse_settings->mouse_smoothing_x = std::stoi(map["mouse_smoothing_x"]);
    mouse_settings->mouse_smoothing_y = std::stoi(map["mouse_smoothing_y"]);
    mouse_settings->mouse_filtering_x = std::stoi(map["mouse_filtering_x"]);
    mouse_settings->mouse_filtering_y = std::stoi(map["mouse_filtering_y"]);
}


/*
typedef struct display_settings_s {
    int id;
    int user_id;
    int vsync;
    int fov;
    int brightness;
    int gamma;
    int contrast;
    int saturation;
    int sharpness;
    char resolution[11];
    char framerate[11];
    char colorblind[15];
} display_settings_t;
*/
void rtype::SqlSendable::convert_display_settings_table(std::map<std::string, std::string> &map, void *data)
{
    // {"id", "user_id", "resolution", "framerate", "vsync", "fov", "brightness", "gamma", "contrast", "saturation", "sharpness", "colorblind"};
    display_settings_t *display_settings = static_cast<display_settings_t *>(data);

    display_settings->id = std::stoi(map["id"]);
    display_settings->user_id = std::stoi(map["user_id"]);

    display_settings->vsync = std::stoi(map["vsync"]);
    display_settings->fov = std::stoi(map["fov"]);
    display_settings->brightness = std::stoi(map["brightness"]);
    display_settings->gamma = std::stoi(map["gamma"]);
    display_settings->contrast = std::stoi(map["contrast"]);
    display_settings->saturation = std::stoi(map["saturation"]);
    display_settings->sharpness = std::stoi(map["sharpness"]);
    std::memcpy(display_settings->resolution, map["resolution"].data(), map["resolution"].size());
    std::memcpy(display_settings->framerate, map["framerate"].data(), map["framerate"].size());
    std::memcpy(display_settings->colorblind, map["colorblind"].data(), map["colorblind"].size());
}

/*
typedef struct audio_settings_s {
    int id;
    int user_id;
    int master;
    int music;
    int effects;
    int voice;
    int voice_chat;
    int voice_chat_push_to_talk;
} audio_settings_t;
*/
void rtype::SqlSendable::convert_audio_settings_table(std::map<std::string, std::string> &map, void *data)
{
    // {"id", "user_id", "master", "music", "effects", "voice", "voice_chat", "voice_chat_push_to_talk"};
    audio_settings_t *audio_settings = static_cast<audio_settings_t *>(data);

    audio_settings->id = std::stoi(map["id"]);
    audio_settings->user_id = std::stoi(map["user_id"]);

    audio_settings->master = std::stoi(map["master"]);
    audio_settings->music = std::stoi(map["music"]);
    audio_settings->effects = std::stoi(map["effects"]);
    audio_settings->voice = std::stoi(map["voice"]);
    audio_settings->voice_chat = std::stoi(map["voice_chat"]);
    audio_settings->voice_chat_push_to_talk = std::stoi(map["voice_chat_push_to_talk"]);
}

/*
typedef struct profile_s {
    int id;
    int user_id;
    char username[21];
    int level;
} profile_t;
*/
void rtype::SqlSendable::convert_profile_table(std::map<std::string, std::string> &map, void *data)
{
    // {"id", "user_id", "name", "level"};
    profile_t *profile = static_cast<profile_t *>(data);
    profile->id = std::stoi(map["id"]);
    profile->user_id = std::stoi(map["user_id"]);

    std::memcpy(profile->username, map["name"].data(), map["name"].size());
    profile->level = std::stoi(map["level"]);
}

/*
typedef struct game_history_s {
    int id;
    int duration;
    char start_time[20];
    char end_time[20];
    char game_mode[20];
    char map[20];
    int player1_id;
    int player2_id;
    int player3_id;
    int player4_id;
    int player5_id;
    int player6_id;
    int player7_id;
    int player8_id;
    int player9_id;
    int player10_id;
} game_history_t;
*/
void rtype::SqlSendable::convert_game_history_table(std::map<std::string, std::string> &map, void *data)
{
    // {"id", "duration", "start_time", "end_time", "game_mode", "map", "player1_id", "player2_id", "player3_id", "player4_id", "player5_id", "player6_id", "player7_id", "player8_id", "player9_id", "player10_id"};
    game_history_t *game_history = static_cast<game_history_t *>(data);

    game_history->id = std::stoi(map["id"]);

    game_history->duration = std::stoi(map["duration"]);
    std::memcpy(game_history->start_time, map["start_time"].data(), map["start_time"].size());
    std::memcpy(game_history->end_time, map["end_time"].data(), map["end_time"].size());
    std::memcpy(game_history->game_mode, map["game_mode"].data(), map["game_mode"].size());
    std::memcpy(game_history->map, map["map"].data(), map["map"].size());
    game_history->winner = std::stoi(map["winner"]);
    for (int i = 0; i < 10; i++)
        game_history->players[i] = std::stoi(map["player" + std::to_string(i + 1) + "_id"]);
    // game_history->player1_id = std::stoi(map["player1_id"]);
    // game_history->player2_id = std::stoi(map["player2_id"]);
    // game_history->player3_id = std::stoi(map["player3_id"]);
    // game_history->player4_id = std::stoi(map["player4_id"]);
    // game_history->player5_id = std::stoi(map["player5_id"]);
    // game_history->player6_id = std::stoi(map["player6_id"]);
    // game_history->player7_id = std::stoi(map["player7_id"]);
    // game_history->player8_id = std::stoi(map["player8_id"]);
    // game_history->player9_id = std::stoi(map["player9_id"]);
    // game_history->player10_id = std::stoi(map["player10_id"]);
}

/*
typedef struct game_history_player_s {
    int id;
    int game_history_id;
    int user_id;
    int kills;
    int deaths;
    int assists;
    int damage;
    int shots;
    int hits;
    int headshots;
    int accuracy;
    int score;
} game_history_player_t;
*/
void rtype::SqlSendable::convert_game_history_player_table(std::map<std::string, std::string> &map, void *data)
{
    // {"id", "game_history_id", "user_id", "kills", "deaths", "assists", "damage", "shots", "hits", "headshots", "accuracy", "score"};
    game_history_player_t *game_history_player = static_cast<game_history_player_t *>(data);

    game_history_player->id = std::stoi(map["id"]);

    game_history_player->game_history_id = std::stoi(map["game_history_id"]);
    game_history_player->user_id = std::stoi(map["user_id"]);
    game_history_player->team = std::stoi(map["team"]);
    game_history_player->kills = std::stoi(map["kills"]);
    game_history_player->deaths = std::stoi(map["deaths"]);
    game_history_player->assists = std::stoi(map["assists"]);
    game_history_player->damage = std::stoi(map["damage"]);
    game_history_player->shots = std::stoi(map["shots"]);
    game_history_player->hits = std::stoi(map["hits"]);
    game_history_player->headshots = std::stoi(map["headshots"]);
    game_history_player->accuracy = std::stoi(map["accuracy"]);
    game_history_player->score = std::stoi(map["score"]);
}

void *rtype::SqlSendable::get_data(TYPE type)
{
    std::map<TYPE, void *> map = {
        {TYPE::USERS, &this->_profile},
        {TYPE::BINDS, &this->_binds},
        {TYPE::MOUSE_SETTINGS, &this->_mouse_settings},
        {TYPE::DISPLAY_SETTINGS, &this->_display_settings},
        {TYPE::AUDIO_SETTINGS, &this->_audio_settings},
        {TYPE::PROFILE, &this->_profile},
        {TYPE::GAME_HISTORY, &this->_game_history},
        {TYPE::GAME_HISTORY_PLAYER, &this->_game_history_player},
    };
    auto it = map.find(type);
    if (it != map.end())
        return it->second;
    return nullptr;
}

void rtype::SqlSendable::ResetData(TYPE type, void *data)
{
    std::map<TYPE, std::pair<void *, size_t>> map = {
        {TYPE::USERS, {&this->_profile, sizeof(profile_t)}},
        {TYPE::BINDS, {&this->_binds, sizeof(binds_t)}},
        {TYPE::MOUSE_SETTINGS, {&this->_mouse_settings, sizeof(mouse_settings_t)}},
        {TYPE::DISPLAY_SETTINGS, {&this->_display_settings, sizeof(display_settings_t)}},
        {TYPE::AUDIO_SETTINGS, {&this->_audio_settings, sizeof(audio_settings_t)}},
        {TYPE::PROFILE, {&this->_profile, sizeof(profile_t)}},
        {TYPE::GAME_HISTORY, {&this->_game_history, sizeof(game_history_t)}},
        {TYPE::GAME_HISTORY_PLAYER, {&this->_game_history_player, sizeof(game_history_player_t)}},
    };
    auto it = map.find(type);
    if (it != map.end())
        std::memset(!data ? it->second.first : data, 0, it->second.second);
}