/*
** EPITECH PROJECT, 2023
** RType
** File description:
** Lobby
*/

#pragma once
    #include "../../includes/CommonIncludes.hpp"
namespace rtype {
    class Lobby {
        public:
            Lobby();
            Lobby(int id);
            ~Lobby();

            void setId(int id);

            bool getIsLobbyReady() const;
            void setIsLobbyReady(bool isLobbyReady);

            int getMaxPlayers() const;
            void setMaxPlayers(int maxPlayers);

            bool getIsLobbyVisibility() const;
            void setIsLobbyPrivacy(bool isLobbyPrivate);

            std::vector<players_t> &getPlayers();

            void setPlayers(std::vector<players_t> players);
            void addPlayer(players_t player);
            void removePlayer(players_t player);
            void removePlayer(int id);
            players_t getPlayer(int id);

            size_t getNbPlayers();
            int getLobbyId() const;
            int getNbPlayersReady();

            void setPlayerReady(int playerId);
            void setPlayerNotReady(int playerId);

            void notifyPlayers(rtype::net::message<MainServerMsgTypes> &msg);
            void notifyPlayer(rtype::net::message<MainServerMsgTypes> &msg, int playerId);
            void notifyPlayersExceptOne(rtype::net::message<MainServerMsgTypes> &msg, int playerId);

            bool IsLobbyAvailable();
            void setIsLobbyInUse(bool lobbyInUse);

            bool isClientInLobby(int id);

            void setPlayerConnectionState(int playerId, int state);
        protected:
        private:
            int _id;
            int _maxPlayers;
            bool _isLobbyReady;
            bool _isLobbyPrivate;
            bool _lobbyInUse;
            std::vector<players_t> _players;
    };
} // namespace rtype