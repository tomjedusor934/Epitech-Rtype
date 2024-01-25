/*
** EPITECH PROJECT, 2023
** RType
** File description:
** ServerToServer
*/

#include "ServerToServer.hpp"

bool rtype::GameServerManager::OnClientConnect(std::shared_ptr<rtype::net::AConnection<CustomCommunicationServer>> client)
{
    std::cout << "New client connected" << std::endl;
    // on doit l'ajouter dans la map des game servers
    std::lock_guard<std::mutex> lock(this->_mtx);
    // recuperer le dernier port dans la map + 1
    int port = DEFAULT_GAME_SERVER_PORT;
    if (this->_servers.size() > 0)
        port = this->_servers.rbegin()->first + 1;
    this->_servers.insert(std::pair<int, rtype::GameServerInfos>(port, rtype::GameServerInfos(client)));
    return true;
}

void rtype::GameServerManager::OnClientDisconnect(std::shared_ptr<rtype::net::AConnection<CustomCommunicationServer>> client)
{
    std::cout << "Client disconnected" << std::endl;
}

void rtype::GameServerManager::OnClientValidated(std::shared_ptr<rtype::net::AConnection<CustomCommunicationServer>> client)
{
    std::cout << "Client validated" << std::endl;
    rtype::net::message<CustomCommunicationServer> msg;
    msg.header.id = CustomCommunicationServer::Infos;
    client->Send(msg);
}

void rtype::GameServerManager::GameServer(std::shared_ptr<rtype::net::AConnection<CustomCommunicationServer>> client, rtype::net::message<CustomCommunicationServer>& msg)
{
    std::cout << "GameServer" << std::endl;
    char ip[INET_ADDRSTRLEN];
    uint16_t port;
    msg >> port >> ip;
    this->_mtx.lock();
    for (auto it = this->_servers.begin(); it != this->_servers.end(); it++) {
        // compare with port
        if (it->second._conn == client) {
            std::strcpy(it->second._infos.ip, ip);
            it->second._infos.port = port;
            break;
        }
    }
    this->_mtx.unlock();
}

void rtype::GameServerManager::GameFinished(std::shared_ptr<rtype::net::AConnection<CustomCommunicationServer>> client, rtype::net::message<CustomCommunicationServer>& msg)
{
    std::cout << "GameFinished" << std::endl;
    this->_mtx.lock();
    for (auto it = this->_servers.begin(); it != this->_servers.end(); it++) {
        // compare with port
        if (it->second._conn == client) {
            it->second._infos.isGameStarted = false;
            break;
        }
    }
    this->_mtx.unlock();
    game_history_t gameHistory;
    msg >> gameHistory;

    std::string query = "INSERT INTO game_history ( " + this->prepareGameHistoryPlayerInfos(gameHistory, 0) + ", winner ) VALUES ( " +  this->prepareGameHistoryPlayerInfos(gameHistory, 1) + ", "+ std::to_string(gameHistory.winner) + ");";
    std::cout << query << std::endl;
    this->_dbManager->insert(query);

    // faire une requete sql qui get la derniere game history id et qui l'envoie au game server
    query = "SELECT id FROM game_history ORDER BY id DESC LIMIT 1";
    std::vector<std::vector<std::string>> result = this->_dbManager->customQuery(query);
    for (auto it = result.begin(); it != result.end(); it++) {
        for (auto it2 = it->begin(); it2 != it->end(); it2++) {
            std::cout << *it2 << std::endl;
        }
    }
    int gameHistoryId = std::stoi(result[0][0]);
    std::cout << "gameHistoryId: " << gameHistoryId << std::endl;
    rtype::net::message<CustomCommunicationServer> msg2;
    msg2.header.id = CustomCommunicationServer::GameHistoryInfos;
    msg2 << gameHistoryId;
    client->Send(msg2);
}

std::string rtype::GameServerManager::prepareGameHistoryPlayerInfos(game_history_t gameHistory, int part)
{
    std::string query = "";

    if (part) {
        for (int i = 0; i < 10; i++) {
            if (i != 0 && gameHistory.players[i] != 0)
                query += ", ";
            if (gameHistory.players[i] == 0)
                break;
            query += std::to_string(gameHistory.players[i]);
        }
        return query;
    }

    for (int i = 0; i < 10; i++) {
        if (i != 0 && gameHistory.players[i] != 0)
            query += ", ";
        if (gameHistory.players[i] == 0)
            break;
        query += "player" + std::to_string(i + 1) + "_id";
    }
    return query;

}

void rtype::GameServerManager::GameHistoryPlayerInfos(std::shared_ptr<rtype::net::AConnection<CustomCommunicationServer>> client, rtype::net::message<CustomCommunicationServer>& msg)
{
    game_history_player_t gameHistoryPlayer;
    int nbPlayers;
    std::cout << "GameHistoryPlayerInfos" << std::endl;

    msg >> nbPlayers;
    std::cout << "nbPlayers: " << nbPlayers << std::endl;

    for (int i = 0; i < nbPlayers; i++) {
        msg >> gameHistoryPlayer;
        std::string query = "INSERT INTO game_history_player (game_history_id, user_id, team, kills, deaths, assists, shots, hits, headshots, accuracy) VALUES ( " +
        std::to_string(gameHistoryPlayer.game_history_id) + ", " +
        std::to_string(gameHistoryPlayer.user_id) + ", " +
        std::to_string(gameHistoryPlayer.team) + ", " +
        std::to_string(gameHistoryPlayer.kills) + ", " +
        std::to_string(gameHistoryPlayer.deaths) + ", " +
        std::to_string(gameHistoryPlayer.assists) + ", " +
        std::to_string(gameHistoryPlayer.shots) + ", " +
        std::to_string(gameHistoryPlayer.hits) + ", " +
        std::to_string(gameHistoryPlayer.headshots) + ", " +
        std::to_string(gameHistoryPlayer.accuracy) + ");";
        std::cout << query << std::endl;
        this->_dbManager->insert(query);
    }

}