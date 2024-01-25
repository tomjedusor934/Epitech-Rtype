/*
** EPITECH PROJECT, 2024
** RType
** File description:
** TeamContainer
*/

#pragma once
    #include "Player.hpp"

namespace rtype {
    class TeamContainer {
        public:
            TeamContainer();
            TeamContainer(int lobby_id, int me, int max_players);
            ~TeamContainer();

            void addPlayer(int id, rtype::teamContainer::Player player);
            void removePlayer(int id);

            std::map<int, rtype::teamContainer::Player> &getPlayers() { return _players; };

            profile_t getUserProfile(int userId);
            profile_t getMineProfile();

            int getLobbyId() { return lobby_id; };
            int getMe() { return _me; };
            int getMaxPlayers() { return _max_players; };

            void setLobbyId(int lobby_id) { this->lobby_id = lobby_id; };
            void setMe(int me) { this->_me = me; };
            void setMaxPlayers(int max_players) { this->_max_players = max_players; };

            std::map<int, rtype::teamContainer::Player> &getPlayersRef() { return _players; };
            rtype::teamContainer::Player &getPlayerRef(int id) { return _players[id]; };

            /*
                choice = 0 -> no change
                choice = 1 -> change

                isFor = "friend" -> change in friend
                isFor = "request" -> change in request
            */
            void setChangeFor(int change, std::string isFor)
            {
                if (isFor == "friend")
                    _changeInFriend = change;
                else if (isFor == "request")
                    _changeInRequest = change;
            };

            /*
                isFor = "friend" -> get change in friend
                isFor = "request" -> get change in request
            */
            int hasChangeFor(std::string isFor) {
                if (isFor == "friend")
                    return _changeInFriend;
                else if (isFor == "request")
                    return _changeInRequest;
                return 0;
            };

            int getLobbyReady();

            void clearTeam(int lobbyId);
            void teamStartGame();

        protected:
        private:
            int lobby_id;
            int _me;
            int _max_players;
            int _isLobbyReady;
            int _changeInFriend;
            int _changeInRequest;
            std::map<int, rtype::teamContainer::Player> _players;
    };
}
