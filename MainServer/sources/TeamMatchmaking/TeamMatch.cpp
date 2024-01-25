/*
** EPITECH PROJECT, 2023
** RType
** File description:
** TeamMatch
*/

#include "TeamMatch.hpp"

rtype::TeamMatch::TeamMatch(int team) : team(team)
{
    this->nbPlayers = 0;
    this->isReady = 0;
}

rtype::TeamMatch::~TeamMatch()
{
}

void rtype::TeamMatch::addLobby(int id, Lobby &lobby)
{
    this->_lobbies.insert(std::pair<int, Lobby&>(id, lobby));
    this->nbPlayers += lobby.getNbPlayers();
}

void rtype::TeamMatch::removeLobby(int id)
{
    this->_lobbies.erase(id);
}

int rtype::TeamMatch::getNbPlayers() const
{
    return this->nbPlayers;
}

int rtype::TeamMatch::getTeam() const
{
    return this->team;
}

void rtype::TeamMatch::clear()
{
    this->_lobbies.clear();
    this->nbPlayers = 0;
    this->isReady = 0;
}

bool rtype::TeamMatch::isTeamReady()
{
    return this->isReady;
}

void rtype::TeamMatch::setTeamState(bool state)
{
    this->isReady = state;
}

void rtype::TeamMatch::freeLobby()
{
    for (auto &lobby : this->_lobbies)
        lobby.second.setIsLobbyInUse(false);
}

std::map<int, rtype::Lobby&> &rtype::TeamMatch::getLobbies()
{
    return this->_lobbies;
}