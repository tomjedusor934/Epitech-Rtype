/*
** EPITECH PROJECT, 2023
** RType
** File description:
** Lobby
*/

#include "Lobby.hpp"

rtype::Lobby::Lobby()
{
    this->_id = 0;
    this->_maxPlayers = 5;
    this->_isLobbyReady = false;
    this->_isLobbyPrivate = false;
    this->_lobbyInUse = false;

}

rtype::Lobby::Lobby(int id) : _id(id)
{
    this->_maxPlayers = 5;
    this->_isLobbyReady = false;
    this->_isLobbyPrivate = false;
    this->_lobbyInUse = false;
}

rtype::Lobby::~Lobby()
{
    this->_players.clear();
}

void rtype::Lobby::setId(int id)
{
    this->_id = id;
}

bool rtype::Lobby::getIsLobbyReady() const
{
    for (int i = 0; i < this->_players.size(); i++) {
        if (this->_players[i].isReady == 0) {
            return false;
        }
    }
    return true;
}

void rtype::Lobby::setIsLobbyReady(bool isLobbyReady)
{
    this->_isLobbyReady = isLobbyReady;

    for (int i = 0; i < this->_players.size(); i++) {
        this->_players[i].isReady = 0;
    }
}

int rtype::Lobby::getMaxPlayers() const
{
    return this->_maxPlayers;
}

void rtype::Lobby::setMaxPlayers(int maxPlayers)
{
    this->_maxPlayers = maxPlayers;
}

bool rtype::Lobby::getIsLobbyVisibility() const
{
    return this->_isLobbyPrivate;
}

void rtype::Lobby::setIsLobbyPrivacy(bool isLobbyPrivate)
{
    this->_isLobbyPrivate = isLobbyPrivate;
}

std::vector<players_t> &rtype::Lobby::getPlayers()
{
    return this->_players;
}

void rtype::Lobby::setPlayers(std::vector<players_t> players)
{
    if (players.size() > this->_maxPlayers)
        return;
    this->_players = players;
}

void rtype::Lobby::addPlayer(players_t player)
{
    if (this->_players.size() >= this->_maxPlayers)
        return;
    player.isReady = 0;
    this->_players.push_back(player);
}

void rtype::Lobby::removePlayer(players_t player)
{
    for (int i = 0; i < this->_players.size(); i++) {
        if (this->_players[i].id == player.id) {
            this->_players.erase(this->_players.begin() + i);
            break;
        }
    }
}

void rtype::Lobby::removePlayer(int id)
{
    for (int i = 0; i < this->_players.size(); i++) {
        if (this->_players[i].id == id) {
            this->_players.erase(this->_players.begin() + i);
            break;
        }
    }
}

size_t rtype::Lobby::getNbPlayers()
{
    return this->_players.size();
}

int rtype::Lobby::getLobbyId() const
{
    return this->_id;
}

int rtype::Lobby::getNbPlayersReady()
{
    int nbPlayersReady = 0;

    for (int i = 0; i < this->_players.size(); i++) {
        if (this->_players[i].isReady == 1)
            nbPlayersReady++;
    }
    return nbPlayersReady;
}

void rtype::Lobby::setPlayerReady(int playerId)
{
    try {
        for (int i = 0; i < this->_players.size(); i++) {
            if (this->_players[i].id == playerId) {
                std::cout << "Player " << playerId << " is ready" << std::endl;
                this->_players[i].isReady = 1;
                if (this->getNbPlayersReady() == this->getNbPlayers()) {
                    std::cout << "Lobby " << this->_id << " is ready" << std::endl;
                    this->_isLobbyReady = true;
                }
                break;
            }

        }
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void rtype::Lobby::setPlayerNotReady(int playerId)
{
    try {
        for (int i = 0; i < this->_players.size(); i++) {
            if (this->_players[i].id == playerId) {
                std::cout << "Player " << playerId << " is not ready" << std::endl;
                this->_players[i].isReady = 0;
                this->_isLobbyReady = false;
                break;
            }
        }
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void rtype::Lobby::notifyPlayers(rtype::net::message<MainServerMsgTypes> &msg)
{
    for (int i = 0; i < this->_players.size(); i++) {
        this->_players[i].client->Send(msg);
    }
}

void rtype::Lobby::notifyPlayer(rtype::net::message<MainServerMsgTypes> &msg, int playerId)
{
    try {
        for (auto &player : this->_players) {
            if (player.id == playerId) {
                player.client->Send(msg);
                break;
            }
        }
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void rtype::Lobby::notifyPlayersExceptOne(rtype::net::message<MainServerMsgTypes> &msg, int playerId)
{
    for (int i = 0; i < this->_players.size(); i++) {
        if (this->_players[i].id != playerId)
            this->_players[i].client->Send(msg);
    }
}

bool rtype::Lobby::IsLobbyAvailable()
{
    return !this->_lobbyInUse;
}

void rtype::Lobby::setIsLobbyInUse(bool lobbyInUse)
{
    this->_lobbyInUse = lobbyInUse;
}

bool rtype::Lobby::isClientInLobby(int id)
{
    for (int i = 0; i < this->_players.size(); i++) {
        if (this->_players[i].id == id)
            return true;
    }
    return false;
}

players_t rtype::Lobby::getPlayer(int id)
{
    for (int i = 0; i < this->_players.size(); i++) {
        if (this->_players[i].id == id)
            return this->_players[i];
    }
    throw std::exception();
}

void rtype::Lobby::setPlayerConnectionState(int playerId, int state)
{
    for (int i = 0; i < this->_players.size(); i++) {
        if (this->_players[i].id == playerId) {
            this->_players[i].tryToConnect = state;
            break;
        }
    }
}