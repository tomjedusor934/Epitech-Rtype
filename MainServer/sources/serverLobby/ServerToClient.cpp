/*
** EPITECH PROJECT, 2023
** RType
** File description:
** ServerToClient
*/

#include "ServerToClient.hpp"

bool rtype::TcpMainServer::OnClientConnect(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> client)
{
    std::cout << "New client connected" << std::endl;
    return true;
}

void rtype::TcpMainServer::OnClientDisconnect(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> client)
{
    std::cout << "Client disconnected" << std::endl;

    // remove the client from the map
    // lock the mutex
    std::unique_lock<std::mutex> lock(this->_mtx);
    for (auto lobby = this->_lobbies.begin(); lobby != this->_lobbies.end(); lobby++) {
        for (auto player = lobby->second.getPlayers().begin(); player != lobby->second.getPlayers().end(); player++) {
            if (player->client == client) {
                lobby->second.removePlayer(player->id);
                if (lobby->second.getNbPlayers() == 0) {
                    this->_lobbies.erase(lobby);
                }
                break;
            }
        }
    }
    lock.unlock();
    // je dois enlever le client de la queue de Aconnection (m_deqConnections)

    for (auto it = this->m_deqConnections.begin(); it != this->m_deqConnections.end(); it++)
        if (*it == client) {
            this->m_deqConnections.erase(it);
            break;
        }

}

void rtype::TcpMainServer::Login(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg)
{
    login_t infos_user;
    msg >> infos_user;
    std::string query;
    std::vector<std::map<std::string, std::string>> profile;
    // now we have the login and the register we call the db to see if we find somthing
    try {
        query = "SELECT * FROM users WHERE username = \"" + std::string(infos_user.username) + "\" AND password = \"" + std::string(infos_user.password) + "\"";
        profile = this->_dbManager->query(query);
        if (profile.size() != 1)
            this->LoginFailed(client);
        else {
            // verifier que le joueur n'est pas deja connecte
            for (auto lobby = this->_lobbies.begin(); lobby != this->_lobbies.end(); lobby++) {
                for (auto player = lobby->second.getPlayers().begin(); player != lobby->second.getPlayers().end(); player++) {
                    if (player->id == std::stoi(profile[0]["id"])) {
                        std::cout << "player already connected" << std::endl;
                        this->LoginFailed(client);
                        return;
                    }
                }
            }
            // on va lui envoyer son profile_t
            query = "SELECT * FROM profile WHERE user_id = " + profile[0]["id"];
            // this->_dbManager->query(query);
            this->_sqlSendable->call_conversion_function(this->_dbManager->query(query)[0], this->_sqlSendable->get_data(rtype::SqlSendable::TYPE::PROFILE), rtype::SqlSendable::TYPE::PROFILE);

            this->LoginSuccesful(client, profile, *(profile_t *)this->_sqlSendable->get_data(rtype::SqlSendable::TYPE::PROFILE));
        }
    } catch(std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
        this->LoginFailed(client);
    }
}

void rtype::TcpMainServer::Register(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg)
{
    // on insert dans la db
    login_t infos_user;
    msg >> infos_user;

    std::string query;

    try {
       // create the user in the db
       if (!this->RegisterUser(infos_user)) {
              this->LoginFailed(client);
            return;
       }

        rtype::net::message<MainServerMsgTypes> msg2;
        msg2 << infos_user;
        this->Login(client, msg2);
    } catch(std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
        this->LoginFailed(client);
    }
}

void rtype::TcpMainServer::Logout(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg)
{
    int client_id = 0;
    int lobby_id = 0;
    msg >> client_id >> lobby_id;

    // remove the client from the map
    // lock the mutex
    std::cout << "id: " << client_id << " lobby_id: " << lobby_id << std::endl;
    std::unique_lock<std::mutex> lock(this->_mtx);
    if (this->_lobbies.find(lobby_id) == this->_lobbies.end()) {
        std::cout << "lobby not found" << std::endl;
        return;
    }
    this->_lobbies[lobby_id].removePlayer(client_id);
    if (this->_lobbies[lobby_id].getNbPlayers() == 0)
        this->_lobbies.erase(lobby_id);
    else {
        // il faut prevenir tous les autres qu'il vient de quitter le lobby
        msg.clear();
        msg.header.id = MainServerMsgTypes::LeaveLobby;
        msg << client_id << 1;
        this->_lobbies[lobby_id].notifyPlayers(msg);
    }
    lock.unlock();
}

void rtype::TcpMainServer::Ready(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg)
{
        int client_id = 0;
        int lobby_id = 0;
        msg >> lobby_id >> client_id;
        // lock the mutex
        std::unique_lock<std::mutex> lock(this->_mtx);
        // find the client in the lobby map
        this->_lobbies[lobby_id].setPlayerReady(client_id);
        // il faut notifier tous les autres que ce joueur vient de mettre pret
        msg.clear();
        msg.header.id = MainServerMsgTypes::Ready;
        msg << client_id;
        this->_lobbies[lobby_id].notifyPlayersExceptOne(msg, client_id);
        lock.unlock();

}

void rtype::TcpMainServer::NotReady(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg)
{
    int client_id = 0;
    int lobby_id = 0;
    msg >> lobby_id >> client_id;

    std::cout << "client_id: " << client_id << " lobby_id: " << lobby_id << std::endl;

    // lock the mutex
    std::unique_lock<std::mutex> lock(this->_mtx);
    // find the client in the lobby map
    this->_lobbies[lobby_id].setPlayerNotReady(client_id);
    lock.unlock();
    msg << client_id;
    this->_lobbies[lobby_id].notifyPlayersExceptOne(msg, client_id);
}

void rtype::TcpMainServer::Profile(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg)
{
    int client_id = 0;
    msg >> client_id;

    std::string query;
    std::vector<std::map<std::string, std::string>> profile;
    // now we have the login and the register we call the db to see if we find somthing
    try {
        query = "SELECT * FROM profile WHERE user_id = " + std::to_string(client_id);
        profile = this->_dbManager->query(query);
        if (profile.size() != 1) {
            throw std::runtime_error("Error: the profile table is empty");
        }
        else {
            // print the profile
            this->_dbManager->dump_result(profile);
            rtype::net::message<MainServerMsgTypes> msg2;
            msg2.header.id = MainServerMsgTypes::ProfileInfos;
            for (auto it = profile[0].begin(); it != profile[0].end(); it++)
                msg2 << it->second.data();
            client->Send(msg2);
        }
    } catch(std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void rtype::TcpMainServer::Settings(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg)
{
    // for the settings we have 5 informations
    // 1 - display_settings
    // 2 - audio_settings
    // 3 - binds
    // 4 - mouse_settings
    // 5 - profile
    int client_id = 0;
    msg >> client_id;

    std::string query;

    // now we have the login and the register we call the db to see if we find somthing
    try {
        query = "SELECT * FROM display_settings WHERE user_id = " + std::to_string(client_id);
        this->GetUserSettings(client, query, MainServerMsgTypes::DisplaySettings, rtype::SqlSendable::TYPE::DISPLAY_SETTINGS);
        query = "SELECT * FROM audio_settings WHERE user_id = " + std::to_string(client_id);
        this->GetUserSettings(client, query, MainServerMsgTypes::AudioSettings, rtype::SqlSendable::TYPE::AUDIO_SETTINGS);
        query = "SELECT * FROM binds WHERE user_id = " + std::to_string(client_id);
        this->GetUserSettings(client, query, MainServerMsgTypes::Binds, rtype::SqlSendable::TYPE::BINDS);
        query = "SELECT * FROM mouse_settings WHERE user_id = " + std::to_string(client_id);
        this->GetUserSettings(client, query, MainServerMsgTypes::MouseSettings, rtype::SqlSendable::TYPE::MOUSE_SETTINGS);
        query = "SELECT * FROM profile WHERE user_id = " + std::to_string(client_id);
        this->GetUserSettings(client, query, MainServerMsgTypes::ProfileInfos, rtype::SqlSendable::TYPE::PROFILE);
    } catch(std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void rtype::TcpMainServer::UpdateSettings(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg)
{

}

//* in this private section wee develop  all the utils fonctions that we need for the message handlers

void rtype::TcpMainServer::LoginFailed(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client)
{
    std::cout << "login Failed" << std::endl;
    rtype::net::message<MainServerMsgTypes> msg;
    msg.header.id = MainServerMsgTypes::LoginFailed;
    client->Send(msg);
}

void rtype::TcpMainServer::LoginSuccesful(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, std::vector<std::map<std::string, std::string>> &profile, profile_t user_profile)
{
    std::cout << "login Succesful" << std::endl;
    players_t newPlayer;
    rtype::net::message<MainServerMsgTypes> msg;
    msg.header.id = MainServerMsgTypes::LoginSuccess;

    //give the id of the client
    std::cout << "id: " << profile[0]["id"] << std::endl;
    msg << std::stoi(profile[0]["id"]);

    // fill new player informations
    newPlayer.id = std::stoi(profile[0]["id"]);
    newPlayer.name = user_profile.username;
    newPlayer.client = client;
    this->GetUserRelations(profile[0]["id"], newPlayer);

    int lobby_id = 0;
    // get the last lobby id to also send to the client
    std::unique_lock<std::mutex> lock(this->_mtx);
    if (this->_lobbies.size() != 0)
        lobby_id = this->_lobbies.rbegin()->first + 1;
    // insert new player in his new lobby
    rtype::Lobby lobby(lobby_id);
    lobby.addPlayer(newPlayer);
    std::cout << "id et lobby_id: " << newPlayer.id << " " << lobby_id << std::endl;
    this->_lobbies[lobby_id] = lobby;
    lock.unlock();

    msg << lobby_id;
    client->Send(msg);

    msg.clear();
    msg.header.id = MainServerMsgTypes::LobbyInfos;
    msg << user_profile << 0 << 1 << lobby_id;
    client->Send(msg);

    // on va lui envoyer les infos sur ses amis si il en a et si ils ceux qui sont connecter
    this->SendFriendsInfos(newPlayer, client, lobby_id);
    this->SendPendingFriendRequest(client, profile[0]["id"]);
}

void rtype::TcpMainServer::GetUserRelations(std::string &userId, players_t &player)
{
    std::string query = "SELECT DISTINCT CASE WHEN user_id_1 = " + userId + " THEN user_id_2 ELSE user_id_1 END AS other_user_id FROM friendships WHERE " + userId + " IN (user_id_1, user_id_2) AND status = 'accepted';";
    std::vector<std::vector<std::string>> query_result;

    query_result = this->_dbManager->customQuery(query);

    if (query_result.size() == 0)
        std::cout << "no friends" << std::endl;
    for (auto it = query_result.begin(); it != query_result.end(); it++) {
        //? ici le it[0] c'est l'id de l'ami
        player.friends.push_back(std::stoi((*it)[0]));
    }
}

void rtype::TcpMainServer::Success(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client)
{
    rtype::net::message<MainServerMsgTypes> msg;
    msg.header.id = MainServerMsgTypes::Success;

    client->Send(msg);
}

int rtype::TcpMainServer::RegisterUser(login_t &infos)
{
    std::string query;
    std::vector<std::map<std::string, std::string>> profile;

    query = "SELECT * FROM users WHERE username = \"" + std::string(infos.username) + "\"";
    profile = this->_dbManager->query(query);

    if (profile.size() != 0)
        return 0;

    try {
        // first insert the user in the db
        query = "INSERT INTO users (username, password) VALUES (\"" + std::string(infos.username) + "\", \"" + std::string(infos.password) + "\")";
        this->_dbManager->insert(query);
        query = "SELECT * FROM users WHERE username = \"" + std::string(infos.username) + "\"";
        profile = this->_dbManager->query(query);
        // then create the profile ("id", "user_id", "name", "level")
        query = "INSERT INTO profile (user_id, name, level) VALUES (" + profile[0]["id"] + ", \"" + std::string(infos.username) + "\", 1)";
        this->_dbManager->insert(query);
        // then create the display_settings insert INTO display_settings (user_id) values ((SELECT id FROM users WHERE username = "test"));
        query = "INSERT INTO display_settings (user_id) VALUES (" + profile[0]["id"] + ")";
        this->_dbManager->insert(query);
        // then create the audio_settings
        query = "INSERT INTO audio_settings (user_id) VALUES (" + profile[0]["id"] + ")";
        this->_dbManager->insert(query);
        // then create the bindings
        query = "INSERT INTO binds (user_id) VALUES (" + profile[0]["id"] + ")";
        this->_dbManager->insert(query);
        // then create the mouse_settings
        query = "INSERT INTO mouse_settings (user_id) VALUES (" + profile[0]["id"] + ")";
        this->_dbManager->insert(query);
        return 1;
    } catch(std::runtime_error &e)
    {
        return 0;
        std::cerr << e.what() << std::endl;
    }
    return 1;
}

void rtype::TcpMainServer::GetUserSettings(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, std::string &query, MainServerMsgTypes type, rtype::SqlSendable::TYPE convert_type)
{
    std::vector<std::map<std::string, std::string>> query_result;
    query_result = this->_dbManager->query(query);
    if (query_result.size() != 1)
        throw std::runtime_error("Error: the query_result table is empty");
    else {
        this->_sqlSendable->call_conversion_function(query_result[0], this->_sqlSendable->get_data(convert_type), convert_type);
        this->PrepareSettings(client, this->_sqlSendable->get_data(convert_type), type, convert_type);
    }
}

void rtype::TcpMainServer::PrepareSettings(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, void *data, MainServerMsgTypes type, rtype::SqlSendable::TYPE convert_type)
{
    display_settings_t display_settings;
    audio_settings_t audio_settings;
    binds_t binds;
    mouse_settings_t mouse_settings;
    profile_t profile;
    rtype::net::message<MainServerMsgTypes> msg;
    msg.header.id = type;
    // i want to cast the data to the right type and i want the value not the pointer
    switch (convert_type) {
        case rtype::SqlSendable::TYPE::DISPLAY_SETTINGS:
            display_settings = static_cast<display_settings_t>(*(display_settings_t *)data);
            msg << display_settings;
            break;
        case rtype::SqlSendable::TYPE::AUDIO_SETTINGS:
            audio_settings = static_cast<audio_settings_t>(*(audio_settings_t *)data);
            msg << audio_settings;
            break;
        case rtype::SqlSendable::TYPE::BINDS:
            binds = static_cast<binds_t>(*(binds_t *)data);
            msg << binds;
            break;
        case rtype::SqlSendable::TYPE::MOUSE_SETTINGS:
            mouse_settings = static_cast<mouse_settings_t>(*(mouse_settings_t *)data);
            msg << mouse_settings;
            break;
        case rtype::SqlSendable::TYPE::PROFILE:
            profile = static_cast<profile_t>(*(profile_t *)data);
            msg << profile;
            break;
        default:
            break;
    }
    client->Send(msg);
}

void rtype::TcpMainServer::SendFriendsInfos(players_t &player, std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, int lobby_id)
{
    // on va parcourir la liste d'amis puis on va regarder si ils sont connectes
    std::string query;
    std::vector<std::map<std::string, std::string>> query_result;
    std::vector<int> isConnected;
    std::vector<profile_t> friends;
    int isFriendConnected = 0;

    for (auto it = player.friends.begin(); it != player.friends.end(); it++) {
        // recuperer son username et level profile_t
        query = "SELECT * FROM profile WHERE user_id = " + std::to_string(*it) + ";";
        query_result = this->_dbManager->query(query);
        if (query_result.size() != 1) {
            std::cout << "euh ya un pb la le pote du gars existe pas" << std::endl;
            continue;
        }
        profile_t profile;
        this->_sqlSendable->call_conversion_function(query_result[0], &profile, rtype::SqlSendable::TYPE::PROFILE);
        friends.push_back(profile);

        // verifier si il est connecte
        isFriendConnected = 0;
        for (auto lobby = this->_lobbies.begin(); lobby != this->_lobbies.end(); lobby++) {
            for (auto playerIn = lobby->second.getPlayers().begin(); playerIn != lobby->second.getPlayers().end(); playerIn++) {
                if (playerIn->id == *it) {
                    isConnected.push_back(1);
                    isFriendConnected = 1;
                    // maintenant on doit le prevenir lui qu'on vient de se connecter
                    rtype::net::message<MainServerMsgTypes> msgforFriend;
                    msgforFriend.header.id = MainServerMsgTypes::NewFriendConnected;
                    msgforFriend << player.id;
                    // ajouter au message l'id et le lobby_id du joueur qui vient de se connecter
                    this->MessageClient(playerIn->client, msgforFriend);
                    break;
                }
            }
            if (isFriendConnected)
                break;
        }

        if (!isFriendConnected)
            isConnected.push_back(0);
    }

    rtype::net::message<MainServerMsgTypes> msg;
    msg.header.id = MainServerMsgTypes::FriendsInfos;

    for (int i = 0; i < player.friends.size(); i++) {
        std::cout << "friend: " << friends[i].username << std::endl;
        msg << friends[i];
        msg << isConnected[i];
    }

    msg << player.friends.size();
    client->Send(msg);
}

//* /////////////////////////// *//

//* here we have all the function used to updater the settings in the db

void rtype::TcpMainServer::DisplaySettings(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg)
{
    display_settings_t display_settings;

    msg >> display_settings;

    std::string query = "UPDATE display_settings SET ";
    query += "vsync = " + std::to_string(display_settings.vsync) + ", ";
    query += "fov = " + std::to_string(display_settings.fov) + ", ";
    query += "brightness = " + std::to_string(display_settings.brightness) + ", ";
    query += "gamma = " + std::to_string(display_settings.gamma) + ", ";
    query += "contrast = " + std::to_string(display_settings.contrast) + ", ";
    query += "saturation = " + std::to_string(display_settings.saturation) + ", ";
    query += "sharpness = " + std::to_string(display_settings.sharpness) + ", ";
    query += "resolution = \"" + std::string(display_settings.resolution) + "\", ";
    query += "framerate = \"" + std::string(display_settings.framerate) + "\", ";
    query += "colorblind = \"" + std::string(display_settings.colorblind) + "\" ";
    query += "WHERE user_id = " + std::to_string(display_settings.user_id);

    try {
        this->_dbManager->update(query);
    } catch(std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void rtype::TcpMainServer::AudioSettings(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg)
{
    audio_settings_t audio_settings;

    msg >> audio_settings;

    std::string query = "UPDATE audio_settings SET ";
    query += "master = " + std::to_string(audio_settings.master) + ", ";
    query += "music = " + std::to_string(audio_settings.music) + ", ";
    query += "effects = " + std::to_string(audio_settings.effects) + ", ";
    query += "voice = " + std::to_string(audio_settings.voice) + ", ";
    query += "voice_chat = " + std::to_string(audio_settings.voice_chat) + ", ";
    query += "voice_chat_push_to_talk = " + std::to_string(audio_settings.voice_chat_push_to_talk) + " ";
    query += "WHERE user_id = " + std::to_string(audio_settings.user_id);

    try {
        this->_dbManager->update(query);
    } catch(std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void rtype::TcpMainServer::Binds(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg)
{

    binds_t binds;

    msg >> binds;

    std::string query = "UPDATE binds SET ";
    query += "forward = " + std::to_string(binds.forward) + ", ";
    query += "backward = " + std::to_string(binds.backward) + ", ";
    query += "`left` = " + std::to_string(binds.left) + ", ";
    query += "`right` = " + std::to_string(binds.right) + ", ";
    query += "shoot = " + std::to_string(binds.shoot) + ", ";
    query += "scope = " + std::to_string(binds.scope) + ", ";
    query += "reload = " + std::to_string(binds.reload) + ", ";
    query += "run = " + std::to_string(binds.run) + ", ";
    query += "swap = " + std::to_string(binds.swap) + ", ";
    query += "jump = " + std::to_string(binds.jump) + ", ";
    query += "pause = " + std::to_string(binds.pause) + ", ";
    query += "chat = " + std::to_string(binds.chat) + ", ";
    query += "score = " + std::to_string(binds.score) + ", ";
    query += "map = " + std::to_string(binds.map) + ", ";
    query += "mute = " + std::to_string(binds.mute) + ", ";
    query += "quit = " + std::to_string(binds.quit) + " ";
    query += "WHERE user_id = " + std::to_string(binds.user_id);

    try {
        this->_dbManager->update(query);
    } catch(std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void rtype::TcpMainServer::MouseSettings(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg)
{
    mouse_settings_t mouse_settings;

    msg >> mouse_settings;

    std::string query = "UPDATE mouse_settings SET ";
    query += "sensitivity = " + std::to_string(mouse_settings.sensitivity) + ", ";
    query += "invert_y = " + std::to_string(mouse_settings.invert_y) + ", ";
    query += "invert_x = " + std::to_string(mouse_settings.invert_x) + ", ";
    query += "mouse_acceleration = " + std::to_string(mouse_settings.mouse_acceleration) + ", ";
    query += "mouse_smoothing = " + std::to_string(mouse_settings.mouse_smoothing) + ", ";
    query += "mouse_filtering = " + std::to_string(mouse_settings.mouse_filtering) + ", ";
    query += "mouse_acceleration_x = " + std::to_string(mouse_settings.mouse_acceleration_x) + ", ";
    query += "mouse_acceleration_y = " + std::to_string(mouse_settings.mouse_acceleration_y) + ", ";
    query += "mouse_smoothing_x = " + std::to_string(mouse_settings.mouse_smoothing_x) + ", ";
    query += "mouse_smoothing_y = " + std::to_string(mouse_settings.mouse_smoothing_y) + ", ";
    query += "mouse_filtering_x = " + std::to_string(mouse_settings.mouse_filtering_x) + ", ";
    query += "mouse_filtering_y = " + std::to_string(mouse_settings.mouse_filtering_y) + " ";
    query += "WHERE user_id = " + std::to_string(mouse_settings.user_id);

    try {
        this->_dbManager->update(query);
    } catch(std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void rtype::TcpMainServer::ProfileInfos(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg)
{
    profile_t profile;

    msg >> profile;

    std::string query = "UPDATE profile SET ";
    query += "name = \"" + std::string(profile.username) + "\", ";
    query += "level = " + std::to_string(profile.level) + " ";
    query += "WHERE user_id = " + std::to_string(profile.user_id);

    try {
        this->_dbManager->update(query);
    } catch(std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

//* /////////////////////////// *//