/*
** EPITECH PROJECT, 2023
** RType
** File description:
** ClientToServer
*/

#include "ClientToServer.hpp"

void rtype::ClientToServer::LoginSuccess(rtype::net::message<MainServerMsgTypes> &msg)
{
    std::cout << "Login success" << std::endl;

    msg >> this->_lobby_id >> this->_id;

    this->_scene = _Scene::LOBBY;

    //? preparer le teamcontainer
    this->_teamContainer.setLobbyId(this->_lobby_id);
    this->_teamContainer.setMe(this->_id);
    this->_teamContainer.setMaxPlayers(5);

    //? preparer moi meme et me mettre dans le teamcontainer
    rtype::teamContainer::Player me(this->_id, this->_containerSettings.getProfilePtr(), false);
    this->_teamContainer.addPlayer(this->_id, me);

    rtype::net::message<MainServerMsgTypes> msgAskSettings;
    msgAskSettings.header.id = MainServerMsgTypes::Settings;
    msgAskSettings << this->_id;
    this->Send(msgAskSettings);

    SetWindowSize(1920, 1080);
    SetWindowPosition(0, 0);
}

void rtype::ClientToServer::LoginFailed(rtype::net::message<MainServerMsgTypes> &msg)
{
    std::cout << "Login failed" << std::endl;
}

void rtype::ClientToServer::DisplaySettings(rtype::net::message<MainServerMsgTypes> &msg)
{
    display_settings_t display_settings;
    msg >> display_settings;
    this->_containerSettings.setDisplaySettings(display_settings);
    this->_containerSettings.setOldDisplaySettings(display_settings);
}

void rtype::ClientToServer::AudioSettings(rtype::net::message<MainServerMsgTypes> &msg)
{
    audio_settings_t audio_settings;
    msg >> audio_settings;
    this->_containerSettings.setAudioSettings(audio_settings);
    this->_containerSettings.setOldAudioSettings(audio_settings);
}

void rtype::ClientToServer::Binds(rtype::net::message<MainServerMsgTypes> &msg)
{
    binds_t binds;
    msg >> binds;
    this->_containerSettings.setBinds(binds);
    this->_containerSettings.setOldBinds(binds);
}

void rtype::ClientToServer::Profile(rtype::net::message<MainServerMsgTypes> &msg)
{
    profile_t profile;
    msg >> profile;
    this->_containerSettings.setProfile(profile);
    this->_containerSettings.setOldProfile(profile);
}

void rtype::ClientToServer::MouseSettings(rtype::net::message<MainServerMsgTypes> &msg)
{
    mouse_settings_t mouse_settings;
    msg >> mouse_settings;
    this->_containerSettings.setMouseSettings(mouse_settings);
    this->_containerSettings.setOldMouseSettings(mouse_settings);
}


void rtype::ClientToServer::ConnectToGameServer(rtype::net::message<MainServerMsgTypes> &msg)
{
    char ip[16];
    uint16_t port;

    msg >> port >> ip;

    std::cout << "ip: " << ip << std::endl;
    std::cout << "port: " << port << std::endl;

    if (!this->_alreadyConnectedOneTime) {
        this->_clientToGame->Connect(ip, port);
        this->_alreadyConnectedOneTime = true;
    } else {
        // mettre a jour le sender endpoit du client to game
        this->_clientToGame->updateEndpoint(ip, port);
    }
    rtype::net::message<GameServerMsgTypes> msg2;
    msg2.header.id = GameServerMsgTypes::ConnectToGameServer;
    msg2 << this->_id;
    this->_clientToGame->Send(msg2);
    this->_teamContainer.teamStartGame();
}

void rtype::ClientToServer::LobbyInfos(rtype::net::message<MainServerMsgTypes> &msg)
{
    std::cout << "Lobby infos" << std::endl;

    int lobby_id;
    int nb_players;
    msg >> lobby_id >> nb_players;
    this->_teamContainer.setLobbyId(lobby_id);
    int is_ready;

    for (int i = 0; i < nb_players; i++) {
        profile_t *profile = new profile_t;
        msg >> is_ready;
        msg >> *profile;
        rtype::teamContainer::Player player(profile->user_id, profile, (bool)is_ready);
        this->_teamContainer.addPlayer(player.getId(), player);
    }
}

void rtype::ClientToServer::FriendsInfos(rtype::net::message<MainServerMsgTypes> &msg)
{
    size_t nb_friends;
    int is_connected;
    msg >> nb_friends;

    for (int i = 0; i < nb_friends; i++) {
        profile_t profile;
        msg >> is_connected >> profile;
        this->_teamContainer.getPlayerRef(this->_id).addFriend(profile, is_connected);
    }
}

void rtype::ClientToServer::FriendLeave(rtype::net::message<MainServerMsgTypes> &msg)
{
    int friendId;
    int nbId;
    msg >> nbId;
    for (int i = 0; i < nbId; i++) {
        msg >> friendId;
        this->_teamContainer.removePlayer(friendId);
    }
}

void rtype::ClientToServer::PendingFriendRequest(rtype::net::message<MainServerMsgTypes> &msg)
{
    int nbId;

    msg >> nbId;

    for (int i = 0; i < nbId; i++) {
        profile_t profile;
        msg >> profile;
        this->_teamContainer.getPlayerRef(this->_id).addPendingFriend(profile);
        this->_teamContainer.setChangeFor(1, "request");
    }
}

void rtype::ClientToServer::NewFriendConnected(rtype::net::message<MainServerMsgTypes> &msg)
{

    int friendIdConnected;
    msg >> friendIdConnected;

    auto &it = this->_teamContainer.getPlayerRef(this->_id).getFriendsRef();
    for (int i = 0; i < it.size(); i++) {
        if (it[i].first.user_id == friendIdConnected)
            it[i].second = 1;
    }
    this->_teamContainer.setChangeFor(1, "friend");
}

void rtype::ClientToServer::PlayerInLobbyIsReady(rtype::net::message<MainServerMsgTypes> &msg)
{
    int playerId;
    msg >> playerId;

    this->_teamContainer.getPlayerRef(playerId).setIsReady(true);
}

void rtype::ClientToServer::PlayerInLobbyIsNotReady(rtype::net::message<MainServerMsgTypes> &msg)
{
    int playerId;
    msg >> playerId;

    this->_teamContainer.getPlayerRef(playerId).setIsReady(false);
}

void rtype::ClientToServer::Alone(rtype::net::message<MainServerMsgTypes> &msg)
{
    int lobbyId;
    msg >> lobbyId;

    this->_teamContainer.clearTeam(lobbyId);
}

void rtype::ClientToServer::GameHistory(rtype::net::message<MainServerMsgTypes> &msg)
{
    int nbGameHistory;
    std::vector<int> Win;
    std::vector<game_history_player_t> game_history_player;
    msg >> nbGameHistory;

    for (int i = 0; i < nbGameHistory; i++) {
        game_history_player_t game_history;
        int isWin;
        msg >> isWin >> game_history;
        this->_historyValues["Win"].push_back(isWin);
        this->_historyValues["Kill"].push_back(game_history.kills);
        this->_historyValues["Death"].push_back(game_history.deaths);
        this->_historyValues["Assist"].push_back(game_history.assists);
        this->_historyValues["Accuracy"].push_back(game_history.accuracy);
        this->_historyValues["Headshot"].push_back(game_history.headshots);
        this->_historyValues["Hits"].push_back(game_history.hits);
        this->_historyValues["Shots"].push_back(game_history.shots);
    }
}
