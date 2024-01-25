/*
** EPITECH PROJECT, 2023
** RType
** File description:
** main
*/

#include "../includes/GameIncludes.hpp"
#include <curl/curl.h>

class GameServer
{
    public:
        GameServer(uint16_t serverNb): udpServer(DEFAULT_GAME_SERVER_PORT + serverNb, mutexGameServer, _canAcceptClients, _gameServerInfo, gameLogic), tcpClient(mutexClientLogic, _ip, DEFAULT_GAME_SERVER_PORT +  serverNb, _canAcceptClients, _gameServerInfo, gameLogic) 
        {
            std::memset(this->_ip, 0, sizeof(this->_ip));
            //! a remettre pour le vrai fonctionnement: this->getPublicIP();
            // std::strcpy(this->_ip, "127.0.0.1");
            std::strcpy(this->_ip, "10.18.207.231");
            std::cout << "IP: " << this->_ip << std::endl;
            this->udpServer.Start();

            this->tcpClient.Connect("127.0.0.1", MAIN_FOR_GAME_PORT);
            this->_canAcceptClients = 0;
        }

        void Start()
        {
            this->threadServer = std::thread(&GameServer::UpdateServer, this);
            this->threadClientLogic = std::thread(&GameServer::UpdateClientLogic, this);
        }

        ~GameServer()
        {
            if (this->threadServer.joinable())
                this->threadServer.join();
            if (this->threadClientLogic.joinable())
                this->threadClientLogic.join();
        }

        void UpdateServer()
        {
            while (this->udpServer.getServerRunning())
                this->udpServer.Update(-1, true);
        }

        void UpdateClientLogic()
        {
            //? le code de la communication entre le game server (client) et le main server (server)
            while (this->tcpClient.IsConnected()) {
                this->tcpClient.handleMessages();
            }
        }

        bool getGameDataSent() const
        {
            return this->gameDataSent;
        }

        void playGame()
        {
            if (!this->udpServer.getGameStarted())
                return;
            for (auto &player : this->gameLogic.teamOne)
                this->gameLogic.updateGravity(this->gameLogic.deltatime, player.second);
            for (auto &player : this->gameLogic.teamTwo)
                this->gameLogic.updateGravity(this->gameLogic.deltatime, player.second);
            this->gameLogic.checkRound();
            bool gameEnded = this->gameLogic.endGame();
            if (gameEnded == true) {
                int winningTeam = this->gameLogic.scoreTeamOne > this->gameLogic.scoreTeamTwo ? 1 : 2;
                rtype::net::message<GameServerMsgTypes> msg2;
                msg2.header.id = GameServerMsgTypes::GameEnded;
                msg2 << winningTeam;
                this->udpServer.MessageAllClients(msg2);
                this->udpServer.setGameStarted(false);
                this->gameDataSent = false;
                std::cout << "Game Ended" << std::endl;
                rtype::net::message<CustomCommunicationServer> msg;
                msg.header.id = CustomCommunicationServer::GameFinished;
                std::cout << "Winner: " << winningTeam << std::endl;
                this->gameLogic.gameHistory.winner = winningTeam;
                msg << this->gameLogic.gameHistory;
                this->tcpClient.Send(msg);
            }
            //? le code du jeu
        }

        void loadMap()
        {
            //? le code pour charger la map
            if (this->gameLogic.mapToLoad == -1)
                return;
            this->gameLogic.parseMap(std::to_string(this->gameLogic.mapToLoad) + ".txt");
            this->udpServer.setGameStarted(true);
            this->gameLogic.mapToLoad = -1;
        }

        void sendGameData()
        {
            //? le code pour envoyer les données du jeu
            if (this->gameLogic.isTeamOneReady() == true && this->gameLogic.isTeamTwoReady() == true && this->udpServer.getGameStarted() == true)
            {
                this->_canAcceptClients = 0;
                for (auto &player : this->gameLogic.teamOne) {
                    if (player.second->client != nullptr) {
                        rtype::net::message<GameServerMsgTypes> resp;
                        resp.header.id = GameServerMsgTypes::SpawnPosition;
                        resp << player.second->data;
                        resp << player.second->spawnPosition;
                        resp << player.second->spawnRotation;
                        resp << 1;
                        resp << player.first;
                        player.second->client->Send(resp);
                        for (auto &otherPlayer : this->gameLogic.teamOne) {
                            if (otherPlayer.first != player.first) {
                                rtype::net::message<GameServerMsgTypes> resp;
                                resp.header.id = GameServerMsgTypes::OtherSpawnPosition;
                                resp << otherPlayer.second->data;
                                resp << otherPlayer.second->spawnPosition;
                                resp << otherPlayer.second->spawnRotation;
                                resp << 1;
                                resp << otherPlayer.first;
                                player.second->client->Send(resp);
                            }
                        }
                        for (auto &otherPlayer : this->gameLogic.teamTwo) {
                            rtype::net::message<GameServerMsgTypes> resp;
                            resp.header.id = GameServerMsgTypes::OtherSpawnPosition;
                            resp << otherPlayer.second->data;
                            resp << otherPlayer.second->spawnPosition;
                            resp << otherPlayer.second->spawnRotation;
                            resp << 2;
                            resp << otherPlayer.first;
                            player.second->client->Send(resp);
                        }
                    }
                }
                for (auto &player : this->gameLogic.teamTwo) {
                    if (player.second->client != nullptr) {
                        rtype::net::message<GameServerMsgTypes> resp;
                        resp.header.id = GameServerMsgTypes::SpawnPosition;
                        resp << player.second->data;
                        resp << player.second->spawnPosition;
                        resp << player.second->spawnRotation;
                        resp << 2;
                        resp << player.first;
                        player.second->client->Send(resp);
                        for (auto &otherPlayer : this->gameLogic.teamTwo) {
                            if (otherPlayer.first != player.first) {
                                rtype::net::message<GameServerMsgTypes> resp;
                                resp.header.id = GameServerMsgTypes::OtherSpawnPosition;
                                resp << otherPlayer.second->data;
                                resp << otherPlayer.second->spawnPosition;
                                resp << otherPlayer.second->spawnRotation;
                                resp << 2;
                                resp << otherPlayer.first;
                                player.second->client->Send(resp);
                            }
                        }
                        for (auto &otherPlayer : this->gameLogic.teamOne) {
                            rtype::net::message<GameServerMsgTypes> resp;
                            resp.header.id = GameServerMsgTypes::OtherSpawnPosition;
                            resp << otherPlayer.second->data;
                            resp << otherPlayer.second->spawnPosition;
                            resp << otherPlayer.second->spawnRotation;
                            resp << 1;
                            resp << otherPlayer.first;
                            player.second->client->Send(resp);
                        }
                    }
                }
                this->gameDataSent = true;
            }
        }

        void setDeltaTime(float deltaTime)
        {
            this->gameLogic.deltatime = deltaTime;
        }

    private:
        rtype::CustomTcpServerAsClient tcpClient;
        rtype::CustomUdpGameServer udpServer;

        std::thread threadServer;
        std::thread threadClientLogic;
        bool gameDataSent = false;

        std::mutex mutexGameServer;
        std::mutex mutexClientLogic;

    private:
        int _canAcceptClients;
        gameServerInfo_t _gameServerInfo;
        rtype::GameLogic gameLogic;

    private:
        char _ip[INET_ADDRSTRLEN];
        std::string _ipstr;
        static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
            size_t total_size = size * nmemb;
            output->append((char*)contents, total_size);
            return total_size;
        }

        // Fonction pour récupérer l'adresse IP publique
        void getPublicIP()
        {
            CURL *curl;
            CURLcode res;

            curl_global_init(CURL_GLOBAL_DEFAULT);
            curl = curl_easy_init();

            std::string ip;

            if (curl)
            {
                // Remplacez "http://httpbin.org/ip" par l'URL du service que vous souhaitez utiliser
                curl_easy_setopt(curl, CURLOPT_URL, "http://httpbin.org/ip");

                // Configuration du callback pour gérer la réponse HTTP
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, this->WriteCallback);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ip);

                // Exécution de la requête HTTP
                res = curl_easy_perform(curl);

                // Vérification des erreurs
                if (res != CURLE_OK)
                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

                // Libération des ressources
                curl_easy_cleanup(curl);
            }

            curl_global_cleanup();

           int size = 0;

           for (int n = 15; ip[n] != '"'; n++)
                size++;
            // je veux memcpy de ip[13] a ip[13 + size]
            std::memcpy(this->_ip, &ip[15], size);
        }
};

int main(int ac, char **av)
{
    if (ac != 2) {
        std::cerr << "Usage: ./gameServer [serverNb]" << std::endl;
        return (84);
    }
    // SetTraceLogLevel(LOG_NONE);
    InitWindow(1, 1, "Game Server");
    SetWindowState(FLAG_WINDOW_HIDDEN);  // Hide the window
    std::cout << "Game Server" << std::endl;
    GameServer gameServer(std::stoi(av[1]));

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point end;
    std::chrono::duration<double> elapsed_seconds;
    // je veux que l'updte se fasse toute les 32e de secondes
    // soit toutes l 1/32e de secondes

    gameServer.Start();
    while (1) {
        end = std::chrono::steady_clock::now();
        elapsed_seconds = end - begin;
        double deltaTime = elapsed_seconds.count();  // Delta time in seconds
        gameServer.loadMap();
        gameServer.setDeltaTime(deltaTime);

        if (deltaTime >= 1.0 / TICK_RATE) {
            begin = std::chrono::steady_clock::now();

            if (gameServer.getGameDataSent() == false) {
                gameServer.sendGameData();
            } else {
                gameServer.playGame();
            }
        }
    }
    return (0);
}
