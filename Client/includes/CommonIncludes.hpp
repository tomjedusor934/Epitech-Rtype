/*
** EPITECH PROJECT, 2023
** RType
** File description:
** CommonIncludes
*/

#pragma once
    #include "../../Includes/Includes.hpp"
    #include "../../LibNetwork/tcp/NetClient.hpp"
    #include "../../LibNetwork/udp/NetClient.hpp"
    #include "../../ECS/ECS.hpp"
    #include "../../Includes/rlights_lib.h"
    #include <SFML/Audio.hpp>
    #include <SFML/System.hpp>
    #include <SFML/Window.hpp>
    #include <SFML/Network.hpp>


#if defined (PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

    enum class _Scene: int {
        LOGIN,
        LOBBY,
        LOADING,
        STORE,
        COLLECTION,
        CAREER,
        VIDEOSETTINGS,
        MOUSESETTINGS,
        CONTROLSSETTINGS,
        AUDIOSETTINGS,
        PROFILSETTINGS,
        VIDEOSETTINGSGAME,
        MOUSESETTINGSGAME,
        CONTROLSSETTINGSGAME,
        AUDIOSETTINGSGAME,
        PROFILSETTINGSGAME,
        GAME,
        RESULT
    };