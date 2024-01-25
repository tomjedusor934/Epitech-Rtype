/*
** EPITECH PROJECT, 2023
** RType
** File description:
** relationship
*/

#include "ServerToClient.hpp"

/*
    JoinFriend, //* (join a friend in a lobby)
    LeaveFriend, //* (leave a friend in a lobby)
    AddFriend, //* (add a friend)
    RemoveFriend, //* (remove a friend)
    AcceptFriend, //* (accept a friend)
    RefuseFriend, //* (refuse a friend)
*/

void rtype::TcpMainServer::JoinFriend(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg)
{
    int friendId = 0;
    int myId = 0;
    int myLobbyId = 0;

    msg >> friendId >> myId >> myLobbyId;

    // 1 on va verifier si le client est dans le lobby
    // 2 il va falloir trouver le lobby du friend
    // 3 si le lobby du friend est plein on va lui envoyer un message d'erreur
    // 4 sinon il faut le supprimer de son lobby (si ce dernier se trouve vide on le supprime)
    // 5 et on l'ajoute dans le lobby du friend
    // 6 on va envoyer un message a tous les clients du lobby du friend pour leur dire qu'un nouveau joueur a rejoint le lobby et on va envoyer un message au client pour lui dire qu'il a rejoint le lobby

    //* 1
    if (this->_lobbies[myLobbyId].isClientInLobby(myId) == false)
    {
        std::cerr << "Client is not in the lobby" << std::endl;
        return;
    }

    //* 2
    int friendLobbyId = -1;
    for (auto &lobby : this->_lobbies)
    {
        if (lobby.second.isClientInLobby(friendId))
        {
            friendLobbyId = lobby.first;
            break;
        }
    }

    //* 3
    if (friendLobbyId == -1)
    {
        std::cerr << "Friend is not in a lobby" << std::endl;
        return;
    }
    if (this->_lobbies[friendLobbyId].getNbPlayers() == this->_lobbies[friendLobbyId].getMaxPlayers())
    {
        std::cerr << "Friend lobby is full" << std::endl;
        return;
    }



    //* 6(1)
    rtype::net::message<MainServerMsgTypes> msgforFriend;
    msgforFriend.header.id = MainServerMsgTypes::LobbyInfos;
    profile_t profile;
    std::vector<std::map<std::string, std::string>> result;
    std::string query = "SELECT * FROM profile WHERE user_id = " + std::to_string(myId) + ";";
    result = this->_dbManager->query(query);
    if (result.size() != 1)
    {
        std::cerr << "Id is not valid" << std::endl;
        return;
    }

    // convertir le result en profile_t
    this->_sqlSendable->call_conversion_function(result[0], &profile, rtype::SqlSendable::TYPE::PROFILE);

    // mettre le profile_t dans le message
    // 1 is the nb of player infos and 0 is if ts ready or not
    msgforFriend << profile << 0 << 1 << friendLobbyId;

    std::cout << msgforFriend << std::endl;
    this->_lobbies[friendLobbyId].notifyPlayers(msgforFriend);

    //* 6(2)
    rtype::net::message<MainServerMsgTypes> msgforClient;
    msgforClient.header.id = MainServerMsgTypes::LobbyInfos;
    // std::vector<profile_t> playersInfos;
    for (players_t &player : this->_lobbies[friendLobbyId].getPlayers()) {
            profile_t playerInfos;
            query = "SELECT * FROM profile WHERE user_id = " + std::to_string(player.id) + ";";
            result = this->_dbManager->query(query);
            if (result.size() != 1)
            {
                std::cerr << "Id is not valid" << std::endl;
                continue;
            }
            // convertir le result en profile_t
            this->_sqlSendable->call_conversion_function(result[0], &playerInfos, rtype::SqlSendable::TYPE::PROFILE);
            msgforClient << playerInfos << player.isReady;
    }
    msgforClient << (int)this->_lobbies[friendLobbyId].getPlayers().size() << friendLobbyId;
    this->_lobbies[myLobbyId].notifyPlayer(msgforClient, myId);

    //* 4/5
    this->_lobbies[friendLobbyId].addPlayer(this->_lobbies[myLobbyId].getPlayer(myId));
    this->_lobbies[myLobbyId].removePlayer(myId);
    if (this->_lobbies[myLobbyId].getNbPlayers() == 0)
        this->_lobbies.erase(myLobbyId);
    else {
        //? prevenir les joeurs de l'ancien lobby que le joueur a quitté le lobby
        msgforFriend.clear();
        msgforFriend.header.id = MainServerMsgTypes::LeaveLobby;
        msgforFriend << myId << 1;
        this->_lobbies[myLobbyId].notifyPlayers(msgforFriend);

        //? il faut que je me dise a moi que ces joueur ne sont plus dans mon lobby
        msgforClient.clear();
        msgforClient.header.id = MainServerMsgTypes::LeaveLobby;
        for (auto &player : this->_lobbies[myLobbyId].getPlayers()) {
            std::cout << "player id: " << player.id << " player username " << player.name << std::endl;
            msgforClient << player.id;
        }
        msgforClient << (int)this->_lobbies[myLobbyId].getPlayers().size();
        this->_lobbies[friendLobbyId].notifyPlayer(msgforClient, myId);
    }
}

void rtype::TcpMainServer::LeaveLobby(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg)
{
    int myId = 0;
    int myLobbyId = 0;

    msg >> myId >> myLobbyId;
    std::cout << "myId: " << myId << " myLobbyId: " << myLobbyId << std::endl;

    // 1 on va verifier si le client est dans le lobby
    // 2 on va creer un nouveau lobby pour le client et l'y ajouter
    // 3 on va supprimer le client de son ancien lobby
    // 4 si son ancien lobby est vide on le supprime
    // 5 on va envoyer un message a tous les clients du lobby du client pour leur dire qu'un joueur a quitté le lobby et on va envoyer un message au client pour lui dire qu'il a quitté le lobby

    //* 1
    if (this->_lobbies[myLobbyId].isClientInLobby(myId) == false)
    {
        std::cerr << "Client is not in the lobby" << std::endl;
        return;
    }

    //* 2 on recupere la derniere clef du map et on l'incremente de 1
    int newLobbyId = this->_lobbies.rbegin()->first + 1;
    this->_lobbies.insert(std::pair<int, rtype::Lobby>(newLobbyId, rtype::Lobby(newLobbyId)));
    this->_lobbies[newLobbyId].addPlayer(this->_lobbies[myLobbyId].getPlayer(myId));

    //* 3/4
    this->_lobbies[myLobbyId].removePlayer(myId);
    if (this->_lobbies[myLobbyId].getNbPlayers() == 0)
        this->_lobbies.erase(myLobbyId);
    else {
        //* 5 rpevenir les autres qu'on vient de quitter le lobby
        msg.clear();
        MainServerMsgTypes::LeaveLobby;
        msg << myId << 1;
        this->_lobbies[myLobbyId].notifyPlayers(msg);
    }
    // il faut prevenir le gars de reset son lobby
    msg.clear();
    msg.header.id = MainServerMsgTypes::Alone;
    msg << newLobbyId;
    this->_lobbies[newLobbyId].notifyPlayer(msg, myId);
}

void rtype::TcpMainServer::AddFriend(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg)
{
    // in the db the table friendships looks like this:
    /*
    `id` int(11) NOT NULL AUTO_INCREMENT,
    `user_id_1` int(11) NOT NULL,
    `user_id_2` int(11) NOT NULL,
    `status` enum('pending', 'accepted', 'declined') NOT NULL DEFAULT 'pending',
    */
    // so we just need to insert uid1 and uid2
    // but fisrt we need to check if the friendship already exists or if the user is trying to add himself or if id 1 and id 2 are valid
    int uid1 = 0;
    char username[21];

    msg >> uid1 >> username;

    std::cout << "uid1: " << uid1 << " username: " << username << std::endl;

    // 1 on va verifier si l'id 1 est correcte valides
    // 2 on va essayer de get l'id 2 avec le username
        // si on trouve pas on envoit un message d'erreur
    // 3 on va verifier si l'id 1 et l'id 2 sont differents
    // 4 on va verifier si l'id 1 et l'id 2 ne sont pas deja amis (present dans la table peut importe le status)


    //*1/2
    std::vector<std::map<std::string, std::string>> result;
    std::string query = "SELECT * FROM profile WHERE user_id = " + std::to_string(uid1) + ";";
    result = this->_dbManager->query(query);
    if (result.size() != 1)
    {
        std::cerr << "Id is not valid" << std::endl;
        return;
    }

    query = "SELECT * FROM profile WHERE name = '" + std::string(username) + "';";
    result = this->_dbManager->query(query);
    if (result.size() != 1)
    {
        std::cerr << "Username is not valid" << std::endl;
        return;
    }
    int uid2 = std::stoi(result[0]["user_id"]);

    //*
    if (uid1 == uid2)
    {
        std::cerr << "You can't add yourself" << std::endl;
        return;
    }

    query = "SELECT * FROM users WHERE id = " + std::to_string(uid1) + " OR id = " + std::to_string(uid2) + ";";
    result = this->_dbManager->query(query);
    if (result.size() != 2)
    {
        std::cerr << "One of the id is not valid" << std::endl;
        return;
    }

    //* 3
    query = "SELECT * FROM friendships WHERE (user_id_1 = " + std::to_string(uid1) + " AND user_id_2 = " + std::to_string(uid2) + ") OR (user_id_1 = " + std::to_string(uid2) + " AND user_id_2 = " + std::to_string(uid1) + ");";
    result = this->_dbManager->query(query);
    if (result.size() != 0)
    {
        std::cerr << "Friendship already exists" << std::endl;
        return;
    }

    //* 4 on va inserer la nouvelle amitie dans la table friendships
    query = "INSERT INTO friendships (user_id_1, user_id_2) VALUES (" + std::to_string(uid1) + ", " + std::to_string(uid2) + ");";
    this->_dbManager->insert(query);

    // prevenir qu'on a recu une demande d'ami
    rtype::net::message<MainServerMsgTypes> msgforFriend;
    msgforFriend.header.id = MainServerMsgTypes::PendingFiendRequest;
    // il faut que je recupere le profile_t du gars que j'ajoute
    profile_t profile;
    query = "SELECT * FROM profile WHERE user_id = " + std::to_string(uid1) + ";";
    result = this->_dbManager->query(query);
    if (result.size() != 1)
    {
        std::cerr << "Id is not valid" << std::endl;
        return;
    }
    this->_sqlSendable->call_conversion_function(result[0], &profile, rtype::SqlSendable::TYPE::PROFILE);
    msgforFriend << profile << 1;
    //je dois trouver le client qui a l'id uid2
    for (auto &lobby : this->_lobbies)
    {
        if (lobby.second.isClientInLobby(uid2))
        {
            lobby.second.notifyPlayer(msgforFriend, uid2);
            break;
        }
    }
}

void rtype::TcpMainServer::RemoveFriend(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg)
{
    int uid1; // you
    int uid2; // friend

    msg >> uid1 >> uid2;

    // 1 on va verifier si l'id 1 et l'id 2 sont differents
    // 2 on va verifier si l'id 1 et l'id 2 sont amis (present dans la table peut importe le status)
    // 3 on va supprimer l'amitie de la table friendships

    //* 1
    if (uid1 == uid2)
    {
        std::cerr << "You can't remove yourself" << std::endl;
        return;
    }

    //* 2
    std::vector<std::map<std::string, std::string>> result;
    std::string query = "SELECT * FROM friendships WHERE (user_id_1 = " + std::to_string(uid1) + " AND user_id_2 = " + std::to_string(uid2) + ") OR (user_id_1 = " + std::to_string(uid2) + " AND user_id_2 = " + std::to_string(uid1) + ");";
    result = this->_dbManager->query(query);
    if (result.size() != 1)
    {
        std::cerr << "Friendship doesn't exist" << std::endl;
        return;
    }

    //* 3
    query = "DELETE FROM friendships WHERE (user_id_1 = " + std::to_string(uid1) + " AND user_id_2 = " + std::to_string(uid2) + ") OR (user_id_1 = " + std::to_string(uid2) + " AND user_id_2 = " + std::to_string(uid1) + ");";
    this->_dbManager->remove(query);
}

void rtype::TcpMainServer::AcceptFriend(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg)
{
    int uid1; // you
    int uid2; // friend
    std::string query;
    std::vector<std::map<std::string, std::string>> result;

    msg >> uid1 >> uid2;

    if (this->PrepareRequest(uid1, uid2, "accepted"))
        return;

    // il faut prevenir le 2 qu'ils on un nouvel ami et si il est en ligne ou pas
    // 1 on va voir si le client qui a demande est en ligne
    int isOnline = 0;
    profile_t user_profile;
    for (auto &lobby : this->_lobbies)
    {
        for (auto &player : lobby.second.getPlayers())
            std::cout << "player id: " << player.id << " player username " << player.name << std::endl;
        if (lobby.second.isClientInLobby(uid2))
        {
            // il est en ligne
            // on va lui envoyer un message pour lui dire qu'il a un nouvel ami
            msg.clear();
            msg.header.id = MainServerMsgTypes::FriendsInfos;
            query = "SELECT * FROM profile WHERE user_id = " + std::to_string(uid1) + ";";
            result = this->_dbManager->query(query);
            if (result.size() != 1) {
                std::cerr << "Id is not valid" << std::endl;
                return;
            }
            this->_sqlSendable->call_conversion_function(result[0], &user_profile, rtype::SqlSendable::TYPE::PROFILE);
            msg << user_profile << 1 << (size_t)1;
            lobby.second.notifyPlayer(msg, uid2);
            isOnline = 1;
            break;
        }
    }
    msg.clear();
    msg.header.id = MainServerMsgTypes::FriendsInfos;
    query = "SELECT * FROM profile WHERE user_id = " + std::to_string(uid2) + ";";
    result = this->_dbManager->query(query);
    if (result.size() != 1) {
        std::cerr << "Id is not valid" << std::endl;
        return;
    }
    this->_sqlSendable->call_conversion_function(result[0], &user_profile, rtype::SqlSendable::TYPE::PROFILE);
    std::cout << "user_profile: " << user_profile.username << std::endl;
    msg << user_profile << isOnline << (size_t)1;
    client->Send(msg);

}

void rtype::TcpMainServer::RefuseFriend(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg)
{
    int uid1; // you
    int uid2; // friend

    msg >> uid1 >> uid2;

    if (this->PrepareRequest(uid1, uid2, "declined"))
        return;
}

int rtype::TcpMainServer::PrepareRequest(int uid1, int uid2, std::string state)
{
    // 1 on va verifier si l'id 1 et l'id 2 sont differents
    // 2 on va verifier si l'id 1 et l'id 2 sont amis (present dans la table avec le status pending)
    // 3 on va update la table friendships pour mettre le status a declined

    //* 1
    if (uid1 == uid2)
    {
        std::cerr << "You can't add yourself" << std::endl;
        return 1;
    }

    //* 2
    std::vector<std::map<std::string, std::string>> result;
    std::string query = "SELECT * FROM friendships WHERE (user_id_1 = " + std::to_string(uid1) + " AND user_id_2 = " + std::to_string(uid2) + ") OR (user_id_1 = " + std::to_string(uid2) + " AND user_id_2 = " + std::to_string(uid1) + ");";
    result = this->_dbManager->query(query);
    if (result.size() != 1)
    {
        std::cerr << "Friendship doesn't exist" << std::endl;
        return 1;
    }
    if (result[0]["status"] != "pending")
    {
        std::cerr << "Friendship is not pending" << std::endl;
        return 1;
    }

    //* 3
    query = "UPDATE friendships SET status = '" + state + "' WHERE (user_id_1 = " + std::to_string(uid1) + " AND user_id_2 = " + std::to_string(uid2) + ") OR (user_id_1 = " + std::to_string(uid2) + " AND user_id_2 = " + std::to_string(uid1) + ");";
    this->_dbManager->update(query);
    return 0;
}

void rtype::TcpMainServer::SendPendingFriendRequest(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, std::string &userId)
{
    std::string query = "SELECT user_id_1 FROM friendships WHERE user_id_2 = " + userId + " AND status = 'pending';";
    std::vector<std::vector<std::string>> query_result;

    query_result = this->_dbManager->customQuery(query);

    if (query_result.size() == 0)
        return;

    int pendingUsdId;
    std::vector<std::map<std::string, std::string>> result;


    // on a donc un certain nombre de demande d'ami en attente il faut donc les envoyer au client
    // pour ca il va faloir recuperer les infos du profile de chaque user
    rtype::net::message<MainServerMsgTypes> msg;
    msg.header.id = MainServerMsgTypes::PendingFiendRequest;

    for (auto it = query_result.begin(); it != query_result.end(); it++) {
        pendingUsdId = std::stoi((*it)[0]);
        query = "SELECT * FROM profile WHERE user_id = " + std::to_string(pendingUsdId) + ";";
        result = this->_dbManager->query(query);
        if (result.size() != 1)
        {
            std::cerr << "Id is not valid" << std::endl;
            continue;
        }

        profile_t profile;
        // convertir le result en profile_t
        this->_sqlSendable->call_conversion_function(result[0], &profile, rtype::SqlSendable::TYPE::PROFILE);

        // mettre le profile_t dans le message
        msg << profile;
    }
    msg << (int)query_result.size();
    client->Send(msg);
}