/*
** EPITECH PROJECT, 2023
** RType
** File description:
** MainServer
*/

#pragma once
    #include "../TeamMatchmaking/TeamMatch.hpp"
    #include "../../includes/MainServer.hpp"
namespace rtype {
    class MainServer {
        public:
            MainServer(): _serverLobby(MAIN_SERVER_PORT, _lobbies, _mutexLobby, "LOBBY"), _serverManager(MAIN_FOR_GAME_PORT, _mutexGame, _gameServersInfos, "MANAGER", _lobbies), _teamMatch1(1), _teamMatch2(2)
            {
                this->_serverLobby.Start();
                this->_serverManager.Start();

                this->_nbGameServerStarted = 0;
                this->_threadsGameServerStarter.resize(MAX_GAME_SERVERS);
                this->_start = std::chrono::high_resolution_clock::now();
            }
            ~MainServer()
            {
                this->_serverLobby.setServerRunning(false);
                this->_serverLobby.Stop();
                this->_serverManager.setServerRunning(false);
                this->_serverManager.Stop();

                if (this->_threadLobby.joinable())
                    this->_threadLobby.join();
                if (this->_threadGame.joinable())
                    this->_threadGame.join();

                for (std::thread &thread : this->_threadsGameServerStarter) {
                    if (thread.joinable())
                        thread.join();
                }
            }

            void StartServers()
            {
                //? on lance les deux serveurs dans deux threads différents
                this->_threadLobby = std::thread(&MainServer::UpdateLobby, this);

                this->_threadGame = std::thread(&MainServer::updateGame, this);
            }

            void UpdateLobby()
            {
                while(this->_serverLobby.getServerRunning()) {
                    this->_serverLobby.Update(-1, true);
                }
            }

            void updateGame()
            {
                while(this->_serverManager.getServerRunning()) {
                    this->_serverManager.Update(-1, true);
                }
            }

            void run();

        protected:

            void matchmaking();

            void createGameServer();

        private:
            // a tcp server for the lobby
            rtype::TcpMainServer _serverLobby;

            // a tcp server for the game server
            rtype::GameServerManager _serverManager;

            // a map of all the game server available
            std::map<int, rtype::GameServerInfos> _gameServersInfos;

            // a map of all the lobby
            std::map<int, rtype::Lobby> _lobbies;

        private:
            //? je pense qu'il serait intelligent de lancer les deux serveurs dans deux threads différents
            //? je lance le thread en lancant la fonction Start() de chaque serveur
            std::thread _threadLobby;
            std::thread _threadGame;

            //? il faudrait des mutex pour proteger les maps
            std::mutex _mutexLobby;
            std::mutex _mutexGame;

        private:
            //? cette section est utilisée pour le matchmaking
            //? on va stocker l'id des lobby qui sont pret et qui peuvent entre dans une team (leurs nombre de joueur ne depasse pas la capacite de la team)
            rtype::TeamMatch _teamMatch1;
            rtype::TeamMatch _teamMatch2;

            void fillTeamMatch(rtype::TeamMatch &teamMatch);
            int isTeamMatchReady(rtype::TeamMatch &teamMatchToCheck, rtype::TeamMatch &teamMatchToFill);
            void TeamMatchIsReady(rtype::TeamMatch &teamMatch, std::vector<rtype::TeamMatch> &allTeamMatch);
            void fillInfos(gameServerInfo_t &gameServerInfos, rtype::TeamMatch &teamMatch1, rtype::TeamMatch &teamMatch2);

            int canGameServerBeStarted();
        private:
            //? cette section est utilisée pour le lancement des serveurs de jeux
            //? on compte combien de serveur on a l ance car ils ne sont pas instantanement inclus dans la map donc ca evite d'en ajouter trop
            int _nbGameServerStarted;
            std::mutex _mutexGameServerStarter;
            std::chrono::high_resolution_clock::time_point _start;
            std::vector<std::thread> _threadsGameServerStarter;

        private:
            void setInfosTeamMatch(rtype::TeamMatch &teamMatch, gameServerInfo_t &gameServerInfos, int &index, int team);
            void setPlayerStruc(std::vector<players_t> &players, std::string ip);
    };
} // namespace rtype
