/*
** EPITECH PROJECT, 2024
** RType
** File description:
** TeamContainer
*/

#include "TeamContainer.hpp"

rtype::TeamContainer::TeamContainer()
{
    lobby_id = -1;
    _me = -1;
    _max_players = -1;
    _changeInFriend = 0;
    _changeInRequest = 0;
}

rtype::TeamContainer::TeamContainer(int lobby_id, int me, int max_players)
    : lobby_id(lobby_id), _me(me), _max_players(max_players), _changeInRequest(0), _changeInFriend(0)
{
}

rtype::TeamContainer::~TeamContainer()
{
}

void rtype::TeamContainer::addPlayer(int id, rtype::teamContainer::Player player)
{
    this->_players.insert(std::pair<int, rtype::teamContainer::Player>(id, player));
    // this->_players[id] = player;
}

void rtype::TeamContainer::removePlayer(int id)
{
    _players.erase(id);
}

profile_t rtype::TeamContainer::getUserProfile(int userId)
{
    return _players[userId].getProfile();
}

profile_t rtype::TeamContainer::getMineProfile()
{
    return _players[_me].getProfile();
}

int rtype::TeamContainer::getLobbyReady()
{
    int nbPlayersReady = 0;

    for (auto it = _players.begin(); it != _players.end(); it++) {
        if (it->second.getIsReady())
            nbPlayersReady++;
    }
    if (nbPlayersReady == this->_players.size() && nbPlayersReady != 0)
        return 1;
    return 0;
}

void rtype::TeamContainer::clearTeam(int lobbyId)
{
    std::vector<int> itIndex;
    // je dois supprimer tt le monde suaf moi et reset mon lobby id
    for (auto it = _players.begin(); it != _players.end(); it++) {
        if (it->first != _me) {
            itIndex.push_back(it->first);
        } else {
            it->second.setIsReady(0);
        }
    }
    for (int i = 0; i < itIndex.size(); i++) {
        _players.erase(itIndex[i]);
    }
    this->lobby_id = lobbyId;
}

void rtype::TeamContainer::teamStartGame()
{
    for (auto it = _players.begin(); it != _players.end(); it++) {
        it->second.setIsReady(0);
    }
}