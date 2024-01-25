/*
** EPITECH PROJECT, 2023
** RType
** File description:
** Game
*/

#pragma once
    #include "../../includes/MainInclusdes.hpp"

    //? includes of all the scenes
    #include "scenes/login/Login.hpp"
    #include "scenes/gameScene/GameScene.hpp"
    #include "scenes/lobby/Lobby.hpp"
    #include "scenes/settings/VideoSettings.hpp"
    #include "scenes/settings/ControlsSettings.hpp"
    #include "scenes/settings/AudioSettings.hpp"
    #include "scenes/settings/ProfilSettings.hpp"
    #include "scenes/settings/MouseSettings.hpp"
    #include "scenes/loadingscene/Loading.hpp"
    #include "scenes/store/Store.hpp"
    #include "scenes/collection/Collection.hpp"
    #include "scenes/career/Career.hpp"
    #include "scenes/resultScene/resultScene.hpp"

    #include "scenes/settings/ContainerSettings.hpp"

    #include "teamContainer/TeamContainer.hpp"

    #include "soundManager/soundManager.hpp"

class Game {
    public:
        Game();
        ~Game();

        //? this function is used to create the thread for the connection between the client and the game server
        void detach()
        {
            // create the thread
            this->_thread = std::thread(&Game::runClientToServer, this);
        }
        void run();

    protected:
        void runClientToServer()
        {
            while (1)
                this->_clientToServer->handleMessages();
        }
    private:
        //? this is for all the game
        std::thread _thread;
        std::unique_ptr<rtype::ClientToServer> _clientToServer;
        std::unique_ptr<rtype::GameManager> _clientToGame;

        rtype::SoundManager _soundManager;

    private:
        int _me; // (id)
        int _lobby_id;

        _Scene _scene;
        rtype::GameLogic _gameLogic;

        std::map<_Scene, std::shared_ptr<rtype::ECS::Ecs3D::AScene>> allScenes;

        rtype::ContainerSettings _containerSettings;

        rtype::TeamContainer _teamContainer;

        std::map<std::string, std::vector<int>> _historyValues;

    private:
        //? ici tu peux mettre tout ce que tu veux pour ton jeu
};
