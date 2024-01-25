/*
** EPITECH PROJECT, 2023
** RType
** File description:
** TeamMatch
*/

#pragma once
    #include "../gameServerInfos/GameServerInfos.hpp"
namespace rtype {
    class TeamMatch {
        public:
            TeamMatch(int team);
            ~TeamMatch();

            void addLobby(int id, Lobby &lobby);
            void removeLobby(int id);

            int getNbPlayers() const;
            int getTeam() const;

            bool isTeamReady();
            void setTeamState(bool state);

            void freeLobby();

            void clear();

            std::map<int, Lobby&> &getLobbies();
        protected:
        private:
            int nbPlayers;
            int team;
            bool isReady;
            std::map<int, Lobby&> _lobbies;
    };
} // namespace rtype