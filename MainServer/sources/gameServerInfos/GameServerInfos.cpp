/*
** EPITECH PROJECT, 2023
** RType
** File description:
** GameServerInfos
*/

#include "GameServerInfos.hpp"

gameServerInfo_t &rtype::GameServerInfos::getInfos()
{
    return this->_infos;
}

std::shared_ptr<rtype::net::AConnection<CustomCommunicationServer>> rtype::GameServerInfos::getClient() const
{
    return this->_conn;
}

void rtype::GameServerInfos::setInfos(gameServerInfo_t infos)
{
    this->_infos = infos;
}