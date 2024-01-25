/*
** EPITECH PROJECT, 2023
** RType
** File description:
** custom_server
*/

#include "MainServer/MainServer.hpp"

// class gameServerInfos
// {
//     public:
//         gameServerInfos(std::shared_ptr<rtype::net::AConnection<CustomCommunicationServer>> &client): _client(client)
//         {
//             this->_infos.maxPlayers = 4;
//             this->_infos.nbPlayers = 0;
//             this->_infos.isGameStarted = false;
//             this->_infos.port = 0;
//             std::memset(this->_infos.ip, 0, sizeof(this->_infos.ip));
//             std::memset(this->_infos.players, 0, sizeof(this->_infos.players));
//         }
//         ~gameServerInfos() = default;

//         gameServerInfo_t &getInfos()
//         {
//             return this->_infos;
//         }

//         std::shared_ptr<rtype::net::AConnection<CustomCommunicationServer>> &getClient() const
//         {
//             return this->_client;
//         }

//         void setInfos(gameServerInfo_t infos)
//         {
//             this->_infos = infos;
//         }

//     public:
//         gameServerInfo_t _infos;
//         std::shared_ptr<rtype::net::AConnection<CustomCommunicationServer>> &_client;
// };


// class MainServer
// {
//     public:
//         MainServer(): serverLobby(MAIN_SERVER_PORT, _clients, _mutexLobby, _readyClients), tcpServerServer(MAIN_FOR_GAME_PORT, _mutexGame)
//         {
//             this->serverLobby.Start();
//             this->tcpServerServer.Start();
//         }
//         ~MainServer()
//         {
//             this->serverLobby.setServerRunning(false);
//             this->serverLobby.Stop();
//             this->tcpServerServer.setServerRunning(false);
//             this->tcpServerServer.Stop();

//             if (this->_threadLobby.joinable())
//                 this->_threadLobby.join();
//             if (this->_threadGame.joinable())
//                 this->_threadGame.join();
//         };

//         void StartServers()
//         {
//             //? on lance les deux serveurs dans deux threads différents
//             this->_threadLobby = std::thread(&MainServer::UpdateLobby, this);

//             this->_threadGame = std::thread(&MainServer::updateGame, this);
//         }

//         void UpdateLobby()
//         {
//             while(this->serverLobby.getServerRunning()) {
//                 this->serverLobby.Update(-1, true);
//             }
//         }

//         void updateGame()
//         {
//             while(this->tcpServerServer.getServerRunning()) {
//                 this->tcpServerServer.Update(-1, true);
//             }
//         }

//         size_t GetNbClients() const
//         {
//             return this->_clients.size();
//         }

//     public:
//         void Matchmaking()
//         {
//             //? on check si on a au moins 2 clients dans la map
//             std::cout << "Nb clients: " << this->_clients.size() << std::endl;
//             if (this->_clients.size() <= 2)
//                 return;
//             //? on check si on a au moins 2 clients ready dans la map
//             std::cout << "Nb ready clients: " << this->_readyClients.size() << std::endl;
//             if (this->_readyClients.size() <= 2)
//                 return;

//             //? on check si on a un serveur de jeu de libre
//             std::cout << "Nb game servers: " << this->gameServers.size() << std::endl;
//             if (this->gameServers.size() == 0)
//                 return;
//             std::vector<std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>>> ClientsToPrepare;
//             for (auto GameServerIt = this->gameServers.begin(); GameServerIt != this->gameServers.end(); GameServerIt++) {
//                 if (GameServerIt->second.getInfos().isGameStarted == false) {
//                     ClientsToPrepare = this->InitializePlayerDeplacement(GameServerIt->second.getInfos());

//                     // on envoie le message au game server pour qu'il accepte les clients
//                     rtype::net::message<CustomCommunicationServer> msg;
//                     msg.header.id = CustomCommunicationServer::StartAcceptingClients;
//                     msg << GameServerIt->second.getInfos();
//                     GameServerIt->second.getClient()->Send(msg);

//                     // on envoie le message au clients pour qu'ils se connectent au game server
//                     rtype::net::message<MainServerMsgTypes> msg2;
//                     msg2.header.id = MainServerMsgTypes::ConnectToGameServer;
//                     msg2 << GameServerIt->second.getInfos().ip << GameServerIt->second.getInfos().port;
//                     for (auto ClientReadyIt = ClientsToPrepare.begin(); ClientReadyIt != ClientsToPrepare.end(); ClientReadyIt++) {
//                         (*ClientReadyIt)->Send(msg2);
//                     }

//                     ClientsToPrepare.clear();
//                     break;
//                 }
//             }

//             // notifie le client qu'il va rejoindre un game server
//             // ? on initialise le deplacement des x premiers clients de la map dans une liste
//         }

//         std::vector<std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>>> InitializePlayerDeplacement(gameServerInfo_t &gameServer)
//         {
//             int client_id = 0;
//             std::vector<std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>>> clients;

//             // on set le gameServer.isGameStarted a true
//             gameServer.isGameStarted = true;

//             // on doit set les infos pour le game server
//             // on reset le tableau de player
//             std::memset(gameServer.players, 0, sizeof(gameServer.players));
//             int readyClientSize = this->_readyClients.size();
//             int i = 0;
//             for (; i < gameServer.maxPlayers && i < readyClientSize; i++) {
//                 // on recupere le premier client pret
//                 client_id = this->_readyClients.front();
//                 clients.push_back(this->_clients[client_id]);
//                 this->_readyClients.pop_front();

//                 // on set les infos du player
//                 std::memcpy(gameServer.players[i].username, this->_playersInfos[client_id]["username"].c_str(), 21);
//                 gameServer.players[i].id = client_id;
//                 gameServer.players[i].team = i % 2;
//                 gameServer.players[i].kills = 0;
//                 gameServer.players[i].deaths = 0;
//                 gameServer.players[i].assists = 0;
//             }
//             gameServer.nbPlayers = i;

//             return clients;
//         }

//     private:
//         rtype::TcpMainServer serverLobby;
//         rtype::GameServerManager tcpServerServer;
//         std::unordered_map<int, gameServerInfos> gameServers;

//         // le int represente l'id du lobby du client
//         std::map<int, std::vector<players_t>> _clients;
//         // map de client pret
//         std::deque<int> _readyClients;
//         std::unordered_map<int, std::map<std::string, std::string>> _playersInfos;


//     private:
//         //? je pense qu'il serait intelligent de lancer les deux serveurs dans deux threads différents
//         //? je lance le thread en lancant la fonction Start() de chaque serveur
//         std::thread _threadLobby;
//         std::thread _threadGame;

//         //? il faudrait des mutex pour proteger les maps
//         std::mutex _mutexLobby;
//         std::mutex _mutexGame;

// };

int main(void)
{
    rtype::MainServer mainServer;
    mainServer.StartServers();

    while (1)
	{
        // std::cout << "Main server running" << std::endl;
        mainServer.run();
	}
    return 0;
}