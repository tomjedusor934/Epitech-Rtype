/*
** EPITECH PROJECT, 2023
** RType
** File description:
** DatabaseInclude
*/

#pragma once
    #include <iostream>
    #include <memory>
    #include <vector>
    #include <map>
    #include <string>
    #include <thread>
    #include <mutex>
    #include <deque>
    #include <optional>
    #include <algorithm>
    #include <chrono>
    #include <cstdint>
    #include <functional>
    #include <SFML/Audio.hpp>
    #include <SFML/System.hpp>
    #include <SFML/Window.hpp>
    #include <SFML/Network.hpp>

typedef struct profile_s {
    int id;
    int user_id;
    char username[21];
    int level;
} profile_t;

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


// cette structure c'est la principale, celle qu'on get qunad on veut les infos d'un joueur
typedef struct game_history_s {
    // ca pour l'instant on s en fout
    int id;
    int duration;
    char start_time[20];
    char end_time[20];
    char game_mode[20];
    char map[20];
    int winner;
    ////////
    // ca c'est tous lous joueurs presen dans la game
    int players[10];
    ////////
} game_history_t;

// ca c'est quand on sait tous les joueurs d'une game on chope leurs infos avec ca
typedef struct game_history_player_s {
    int id;
    // l'id de la game en question
    int game_history_id;
    // l'id du joueur
    int user_id;
    int team;
    int kills;
    int deaths;
    int assists;
    int damage; // to remove
    int shots; // done
    int hits; // done
    int headshots; // done
    int accuracy; // done
    int score; // to remove
} game_history_player_t;
