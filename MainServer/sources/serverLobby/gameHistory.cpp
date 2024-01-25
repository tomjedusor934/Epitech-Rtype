/*
** EPITECH PROJECT, 2024
** RType
** File description:
** gameHistory
*/

#include "ServerToClient.hpp"

void rtype::TcpMainServer::GameHistory(std::shared_ptr<rtype::net::AConnection<MainServerMsgTypes>> &client, rtype::net::message<MainServerMsgTypes> msg)
{
    int userId;
    msg >> userId;

    // dans un premier temps je dois chercher toutes les parties dans lesquelles l'utilisateur a joué
    /*
    typedef struct game_history_s {
        int id;
        int duration;
        char start_time[20];
        char end_time[20];
        char game_mode[20];
        char map[20];
        int player1_id;
        int player2_id;
        int player3_id;
        int player4_id;
        int player5_id;
        int player6_id;
        int player7_id;
        int player8_id;
        int player9_id;
        int player10_id;
    } game_history_t;
    */
   std::cout << "GameHistory" << std::endl;
    std::vector<std::vector<std::string>> result;
    std::string player1 = "player1_id = " + std::to_string(userId);
    std::string player2 = "player2_id = " + std::to_string(userId);
    std::string player3 = "player3_id = " + std::to_string(userId);
    std::string player4 = "player4_id = " + std::to_string(userId);
    std::string player5 = "player5_id = " + std::to_string(userId);
    std::string player6 = "player6_id = " + std::to_string(userId);
    std::string player7 = "player7_id = " + std::to_string(userId);
    std::string player8 = "player8_id = " + std::to_string(userId);
    std::string player9 = "player9_id = " + std::to_string(userId);
    std::string player10 = "player10_id = " + std::to_string(userId);
    std::string query = "SELECT id, winner FROM game_history WHERE ";
    query += player1 + " OR " + player2 + " OR " + player3 + " OR ";
    query += player4 + " OR " + player5 + " OR " + player6 + " OR ";
    query += player7 + " OR " + player8 + " OR " + player9 + " OR " + player10 + ";";
    result = this->_dbManager->customQuery(query);
    if (result.size() == 0) {
        std::cerr << "Iiiiid is not valid" << std::endl;
        return;
    }

    std::vector<int> game_history_id;
    std::vector<int> winner;
    for (auto &it : result) {
        game_history_id.push_back(std::stoi(it[0]));
        winner.push_back(std::stoi(it[1]));
    }
    rtype::net::message<MainServerMsgTypes> msgGameHistoryPlayer;
    msgGameHistoryPlayer.header.id = MainServerMsgTypes::GameHistory;
    std::vector<std::map<std::string, std::string>> result2;

    for (int i = 0; i < game_history_id.size(); i++) {
        game_history_player_t game_history_player;
        std::string query2 = "SELECT * FROM game_history_player WHERE game_history_id = " + std::to_string(game_history_id[i]) + " AND user_id = " + std::to_string(userId) + ";";
        result2 = this->_dbManager->query(query2);
        if (result2.size() == 0) {
            std::cerr << "Id is not valid" << std::endl;
            return;
        }
        this->_sqlSendable->call_conversion_function(result2[0], &game_history_player, rtype::SqlSendable::TYPE::GAME_HISTORY_PLAYER);
        msgGameHistoryPlayer << game_history_player << (std::stoi(result2[0]["team"]) == winner[i] ? 1 : 0);
    }
    msgGameHistoryPlayer << (int)game_history_id.size();
    client->Send(msgGameHistoryPlayer);
}