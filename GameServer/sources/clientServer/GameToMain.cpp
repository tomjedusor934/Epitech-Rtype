/*
** EPITECH PROJECT, 2023
** RType
** File description:
** GameToMain
*/

#include "GameToMain.hpp"

void rtype::CustomTcpServerAsClient::Infos(rtype::net::message<CustomCommunicationServer> &msg)
{
    // //? on envoie un message au main server pour lui dire qu'on est un game server
    // rtype::net::message<CustomCommunicationServer> msg;
    // msg.header.id = CustomCommunicationServer::GameServer;
    // msg << this->_ip << this->udpServer.getPort();
    // this->tcpClient.Send(msg);

    rtype::net::message<CustomCommunicationServer> msg2;
    msg2.header.id = CustomCommunicationServer::GameServer;
    msg2 << this->_ip << this->_port;
    this->Send(msg2);
}

void rtype::CustomTcpServerAsClient::StartAcceptingClients(rtype::net::message<CustomCommunicationServer> &msg)
{
    std::cout << "StartAcceptingClients" << std::endl;
    msg >> this->_infos;

    std::cout << "IP: " << _infos.ip << std::endl;
    std::cout << "Port: " << _infos.port << std::endl;
    std::cout << "NbPlayers: " << _infos.nbPlayers << std::endl;
    std::cout << "MaxPlayers: " << _infos.maxPlayers << std::endl;
    std::cout << "IsGameStarted: " << _infos.isGameStarted << std::endl;
    std::cout << "Players: " << std::endl;

    Vector3 teamOneSpawnPosition = {0, 5, -62};
    Quaternion teamOneSpawnRotation = QuaternionFromEuler(0, 180, 0);
    Vector3 teamTwoSpawnPosition = {0, 5, 62};
    Quaternion teamTwoSpawnRotation = QuaternionFromEuler(0, 0, 0);

    for (int i = 0; i < this->_infos.nbPlayers / 2; ++i) {
        this->gameLogic.addPlayer(this->_infos.players[i].username, this->_infos.players[i].id, 1, this->_infos.players[i], teamOneSpawnPosition, teamOneSpawnRotation);
        this->gameLogic.gameHistory.players[i] = this->_infos.players[i].id;
    }
    for (int i = this->_infos.nbPlayers / 2; i < this->_infos.nbPlayers; ++i) {
        this->gameLogic.addPlayer(this->_infos.players[i].username, this->_infos.players[i].id, 2, this->_infos.players[i], teamTwoSpawnPosition, teamTwoSpawnRotation);
        this ->gameLogic.gameHistory.players[i] = this->_infos.players[i].id;
    }
    for (int i = this->_infos.nbPlayers; i < 10; ++i) {
        this->gameLogic.gameHistory.players[i] = 0;
    }
    this->gameLogic.mapToLoad = 1;
    this->_canAcceptClient = 1;
}

void rtype::CustomTcpServerAsClient::GameHistoryInfos(rtype::net::message<CustomCommunicationServer> &msg)
{
    int gameHistoryId = 0;
    msg >> gameHistoryId;
    int nbPlayers = 0;
    rtype::net::message<CustomCommunicationServer> msg2;
    msg2.header.id = CustomCommunicationServer::GameHistoryPlayerInfos;
    for (const auto &team : {this->gameLogic.teamOne, this->gameLogic.teamTwo}) {
        for (const auto &player : team) {
            player.second->history.game_history_id = gameHistoryId;
            msg2 << player.second->history;
            nbPlayers++;
        }
    }
    msg2 << nbPlayers;
    this->Send(msg2);
    this->gameLogic.teamOne.clear();
    this->gameLogic.teamTwo.clear();
}
