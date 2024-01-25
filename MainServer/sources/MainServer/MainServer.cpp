/*
** EPITECH PROJECT, 2023
** RType
** File description:
** MainServer
*/

#include "MainServer.hpp"

void rtype::MainServer::run()
{
    this->matchmaking();
}

void rtype::MainServer::fillTeamMatch(rtype::TeamMatch &teamMatch)
{
    // ceci est une fonction tourner tant qu'il y a des lobby ou qu'on a pas atteint le nombre de joueur voulu
    // on va parcourir la map des lobby et on va regarder si on peut les ajouter dans la team
    // on va faire un vecteur de teammatch pour pouvoir les trier comme ca des qu'on en a 1 qui est bon on le met dans la team
    std::vector<rtype::TeamMatch> teamMatchs;
    // on va remplir le vecteur
    teamMatch.setTeamState(false);
    teamMatchs.push_back(rtype::TeamMatch(teamMatch.getTeam()));
    int teamFound = 0;

    // on va parcourir la map des lobby
    std::lock_guard<std::mutex> lock(this->_mutexLobby);
    // std::cout << "nb lobbies: " << this->_lobbies.size() << std::endl;
    for (auto &lobby : this->_lobbies) {
        // on regarde si on peut ajouter le lobby dans la team
        teamFound = 0;
        for (auto &tempTeamMatch : teamMatchs) {
            // print si le lobbyest ready
            // std::cout << "lobby " << lobby.second.getLobbyId() << " ready: " << lobby.second.getIsLobbyReady() << std::endl;
            // std::cout << "lobby " << lobby.second.getLobbyId() << " available: " << lobby.second.IsLobbyAvailable() << std::endl;
            if ((lobby.second.IsLobbyAvailable() && lobby.second.getIsLobbyReady()) && (tempTeamMatch.getNbPlayers() + lobby.second.getNbPlayers() <= NB_PLAYERS_PER_TEAM)) {
                // on ajoute le lobby dans la team
                // std::cout << "add lobby " << lobby.second.getLobbyId() << " to team " << tempTeamMatch.getTeam() << std::endl;
                tempTeamMatch.addLobby(lobby.first, lobby.second);
                // on dit au lobby qu'il est dans une team
                lobby.second.setIsLobbyInUse(true);
                // on dit qu'on a trouve une team pour le lobby
                teamFound = 1;
                // break si on a atteint le nombre de joueur voulu
                if (this->isTeamMatchReady(tempTeamMatch, teamMatch)) {
                    this->TeamMatchIsReady(teamMatch, teamMatchs);
                    return;
                }
            }
        }
        // si on a pas trouve de team pour le lobby on en cree une nouvelle
        if (teamFound == 0 && lobby.second.IsLobbyAvailable() && lobby.second.getIsLobbyReady()) {
            // std::cout << "new team" << std::endl;
            teamMatchs.push_back(rtype::TeamMatch(teamMatch.getTeam()));
            // on ajoute le lobby dans la team
            teamMatchs[teamMatchs.size() - 1].addLobby(lobby.first, lobby.second);
            // std::cout << "add lobby " << lobby.second.getLobbyId() << " to team " << teamMatchs[teamMatchs.size() - 1].getTeam() << std::endl;
            // on dit au lobby qu'il est dans une team
            lobby.second.setIsLobbyInUse(true);
            // break si on a atteint le nombre de joueur voulu
            if (this->isTeamMatchReady(teamMatchs[teamMatchs.size() - 1], teamMatch)) {
                // std::cout << "2 team " << teamMatchs[teamMatchs.size() - 1].getTeam() << " ready" << std::endl;
                this->TeamMatchIsReady(teamMatch, teamMatchs);
                return;
            }
        }
    }
}

int rtype::MainServer::isTeamMatchReady(rtype::TeamMatch &teamMatchToCheck, rtype::TeamMatch &teamMatchToFill)
{
    if (teamMatchToCheck.getNbPlayers() == NB_PLAYERS_PER_TEAM) {
        teamMatchToFill = teamMatchToCheck;
        // sert a dire que la team est prête
        teamMatchToFill.setTeamState(true);
        return 1;
    } else {
        return 0;
    }
}

void rtype::MainServer::TeamMatchIsReady(rtype::TeamMatch &teamMatch, std::vector<rtype::TeamMatch> &allTeamMatch)
{
    for (auto &tempTeamMatch2 : allTeamMatch) {
        tempTeamMatch2.freeLobby();
    }
    // mettre les lobby de la team en in use
    for (auto &lobby : teamMatch.getLobbies()) {
        lobby.second.setIsLobbyInUse(true);
    }
}

void rtype::MainServer::matchmaking()
{
    // on va decouper en plusieurs etapes
    /*
        1. on regarde si un lobby est pret et on compte le nombre de joueurs
        2. si le nombre de joueurs est suffisant on lance une recherche de partie (sachant qu'il faut pas avoir de conflit entre les lobby)
        3. si 1 serveur de jeux est disponible ou qu'on a pas atteint la limite de serveur on lance une partie/demare un serveur de jeux
        4. on initialise le deplacement de joueurs et on previent le serveur
        5. la partie commence

        ! important: il faut bien compter par exemple
        (on veut une partie de 6 (3v3)) on a 3 lobby de 2 joueurs alors c'est pas bon car des joueurs du meme lobby ne peuvent pas etre dans une equipe differnete
    */

   // clear the 2 team match
    this->_teamMatch1.clear();
    this->_teamMatch2.clear();

    this->fillTeamMatch(this->_teamMatch1);
    this->fillTeamMatch(this->_teamMatch2);

    // std::cout << "team 1 ready: " << this->_teamMatch1.isTeamReady() << std::endl;
    // std::cout << "team 2 ready: " << this->_teamMatch2.isTeamReady() << std::endl;
    if (!this->_teamMatch1.isTeamReady() || !this->_teamMatch2.isTeamReady()) {
        // il faut remettre les lobby a available si 1 des 2 team n'est pas prete
        for (auto &lobby : this->_lobbies) {
            lobby.second.setIsLobbyInUse(false);
        }
        // for (auto &lobby : this->_teamMatch1.getLobbies()) {
        //     lobby.second.setIsLobbyInUse(false);
        // }
        return;
    }

    // on a deux team pret
    // on regarde si un serveur de jeux est disponible ou si on doit en creer un
    rtype::GameServerInfos *gameServer = nullptr;
    std::unique_lock<std::mutex> lock(this->_mutexGame);
    for (auto &game_server : this->_gameServersInfos) {
        if (game_server.second.getInfos().isGameStarted == false && game_server.second.getInfos().ip[0]) {
            std::cout << "game server found" << std::endl;
            gameServer = &game_server.second;
            break;
        }
    }

    if (gameServer == nullptr && this->_nbGameServerStarted < MAX_GAME_SERVERS && this->canGameServerBeStarted()) {
        // on doit creer un serveur de jeux
        std::cout << "create game server" << std::endl;
        this->_threadsGameServerStarter[this->_nbGameServerStarted] = std::thread(&MainServer::createGameServer, this);
        this->_nbGameServerStarted++;
        this->_teamMatch1.freeLobby();
        this->_teamMatch2.freeLobby();
        return;
    } else if (gameServer == nullptr) {
        // on a pas de serveur de jeux disponible
        this->_teamMatch1.freeLobby();
        this->_teamMatch2.freeLobby();
        return;
    }

    // on va initialiser le deplacement des joueurs et previent le serveur de jeux
    // on previent le serveur de jeux qu'il va recevoir des clients
    rtype::net::message<CustomCommunicationServer> msg;
    msg.header.id = CustomCommunicationServer::StartAcceptingClients;
    this->fillInfos(gameServer->getInfos(), this->_teamMatch1, this->_teamMatch2);
    msg << gameServer->getInfos();
    gameServer->getClient()->Send(msg);
    lock.unlock();

    usleep(100000);

    // on initialise le deplacement des joueurs
    rtype::net::message<MainServerMsgTypes> msg2;
    msg2.header.id = MainServerMsgTypes::ConnectToGameServer;
    msg2 << gameServer->getInfos().ip << gameServer->getInfos().port;
    // il faut remettre les joueurs de tous les lobby a pas pret et leur dire de se connecter au serveur de jeux
    std::unique_lock<std::mutex> lock2(this->_mutexLobby);
    for (auto &lobby : this->_teamMatch1.getLobbies()) {
        std::cout << "1 lobby " << lobby.second.getLobbyId() << std::endl;
        for (auto &player : lobby.second.getPlayers()) {
            player.client->Send(msg2);
            player.isReady = 0;
        }
        lobby.second.setIsLobbyReady(false);
        lobby.second.setIsLobbyInUse(false);
    }
    for (auto &lobby : this->_teamMatch2.getLobbies()) {
        std::cout << "2 lobby " << lobby.second.getLobbyId() << std::endl;
        for (auto &player : lobby.second.getPlayers()) {
            player.client->Send(msg2);
            player.isReady = 0;
        }
        lobby.second.setIsLobbyReady(false);
        lobby.second.setIsLobbyInUse(false);
    }
    lock2.unlock();
}

int rtype::MainServer::canGameServerBeStarted()
{
    auto now = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed_time = now - this->_start;
    double elapsed_minutes = elapsed_time.count() / 60.0;

    if (elapsed_minutes >= 0.35) {
        this->_start = now;
        return 1;
    } else {
        return 0;
    }
}

void rtype::MainServer::createGameServer()
{
    // executer une fonction system qui va lancer un serveur de jeux
    // on va recuperer le port du serveur de jeux (qui est le defaut game server port + le nombre de serveur de jeux)
    std::string command = "./game " + std::to_string(this->_gameServersInfos.size());
    std::cout << "command: " << command << std::endl;
    int ret = std::system(command.c_str());
    std::cout << "ret: " << ret << std::endl;
    if (ret == -1) {
        std::cout << "Error: system() failed" << std::endl;
        throw std::runtime_error("Error: system() failed");
    } else
        std::cout << "Game server exit succesfully" << std::endl;
}

void rtype::MainServer::fillInfos(gameServerInfo_t &gameServerInfos, rtype::TeamMatch &teamMatch1, rtype::TeamMatch &teamMatch2)
{
    // on va remplir les infos du game server
    gameServerInfos.isGameStarted = true;
    gameServerInfos.nbPlayers = teamMatch1.getNbPlayers() + teamMatch2.getNbPlayers();
    gameServerInfos.maxPlayers = NB_PLAYERS_PER_TEAM * 2;

    // on va remplir les infos des joueurs
    int i = 0;
    this->setInfosTeamMatch(teamMatch1, gameServerInfos, i, 0);
    this->setInfosTeamMatch(teamMatch2, gameServerInfos, i, 1);

    // faire un random entre 2 pour choisir la map
    int map = rand() % 2;
    gameServerInfos.map = map;

    // for (auto &lobby : teamMatch1.getLobbies()) {
    //     for (auto &player : lobby.second.getPlayers()) {
    //         std::memcpy(gameServerInfos.players[i].username, player.name.data(), player.name.size());
    //         gameServerInfos.players[i].id = player.id;
    //         gameServerInfos.players[i].team = 0;
    //         i++;
    //     }
    // }
    // for (auto &lobby : teamMatch2.getLobbies()) {
    //     for (auto &player : lobby.second.getPlayers()) {
    //         std::memcpy(gameServerInfos.players[i].username, player.name.data(), player.name.size());
    //         gameServerInfos.players[i].id = player.id;
    //         gameServerInfos.players[i].team = 1;
    //         i++;
    //     }
    // }
}

void rtype::MainServer::setInfosTeamMatch(rtype::TeamMatch &teamMatch, gameServerInfo_t &gameServerInfos, int &index, int team)
{
    for (auto &lobby : teamMatch.getLobbies()) {
        for (auto &player : lobby.second.getPlayers()) {
            std::memcpy(gameServerInfos.players[index].username, player.name.data(), player.name.size());
            gameServerInfos.players[index].id = player.id;
            gameServerInfos.players[index].team = 0;
            index++;
        }
    }
}

void rtype::MainServer::setPlayerStruc(std::vector<players_t> &players, std::string ip)
{
    // on va remplir le tableau de player
    for (auto &player : players) {
        player.ip = ip;
        player.isReady = 0;
        player.tryToConnect = 1;
    }
}