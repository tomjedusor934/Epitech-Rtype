/*
** EPITECH PROJECT, 2023
** RType
** File description:
** CommonIncludes
*/

#pragma once
    #include "../../Includes/Includes.hpp"
    #include "../../LibNetwork/tcp/NetServer.hpp"
    #include "../../LibNetwork/udp/NetServer.hpp"
    #include "../../LibNetwork/tcp/NetClient.hpp"
    #include <mysql/mysql.h>

typedef struct players_s {
    std::string name;
    std::string ip;
    int id;
    int isReady;
    int tryToConnect;
    std::vector<int> friends;
    std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> client;
} players_t;