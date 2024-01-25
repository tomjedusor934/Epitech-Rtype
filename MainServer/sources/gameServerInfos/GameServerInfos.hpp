/*
** EPITECH PROJECT, 2023
** RType
** File description:
** GameServerInfos
*/

#pragma once
    #include "../../includes/CommonIncludes.hpp"
    #include "../Lobby/Lobby.hpp"
namespace rtype
{
    class GameServerInfos
    {
    public:
        GameServerInfos(std::shared_ptr<rtype::net::AConnection<CustomCommunicationServer>> client) : _conn(client)
        {
            this->_infos.maxPlayers = 4;
            this->_infos.nbPlayers = 0;
            this->_infos.isGameStarted = false;
            this->_infos.port = 0;
            this->_infos.map = 0;
            std::memset(this->_infos.ip, 0, sizeof(this->_infos.ip));
            std::memset(this->_infos.players, 0, sizeof(this->_infos.players));
        }
        ~GameServerInfos() = default;

        gameServerInfo_t &getInfos();

        std::shared_ptr<rtype::net::AConnection<CustomCommunicationServer>> getClient() const;

        void setInfos(gameServerInfo_t infos);

    public:
        gameServerInfo_t _infos;
        std::shared_ptr<rtype::net::AConnection<CustomCommunicationServer>> _conn;
    };
} // namespace rtype