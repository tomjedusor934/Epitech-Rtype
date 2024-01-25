/*
** EPITECH PROJECT, 2024
** RType
** File description:
** GameLogic
*/

#pragma once
#include "playerLogic/PlayerLogic.hpp"

namespace rtype {
    /**
     * @brief The GameLogic class represents the game logic of the RType game server.
     * It manages players, teams, game state, and game mechanics.
     */
    class GameLogic {
        public:
            /**
             * @brief Constructs a GameLogic object.
             */
            GameLogic();

            /**
             * @brief Destroys the GameLogic object.
             */
            ~GameLogic();

            /**
             * @brief Adds a player to the game.
             * @param name The name of the player.
             * @param id The ID of the player.
             * @param team The team of the player.
             * @param data The player information.
             * @param spawnPosition The spawn position of the player.
             * @param spawnRotation The spawn rotation of the player.
             */
            void addPlayer(std::string name, int id, int team, player_info_t data, Vector3 spawnPosition, Quaternion spawnRotation);

            /**
             * @brief Gets a player by ID.
             * @param id The ID of the player.
             * @return A shared pointer to the PlayerLogic object.
             */
            std::shared_ptr<PlayerLogic> getPlayer(int id);

            /**
             * @brief Gets a player by client connection.
             * @param client The client connection.
             * @return A shared pointer to the PlayerLogic object.
             */
            std::shared_ptr<PlayerLogic> getPlayer(std::shared_ptr<rtype::net::AConnection<GameServerMsgTypes>> client);

            /**
             * @brief Gets all players in the same team as the specified player.
             * @param id The ID of the player.
             * @return A map of player IDs to shared pointers of PlayerLogic objects.
             */
            std::map<int, std::shared_ptr<PlayerLogic>> getMyTeam(int id);

            /**
             * @brief Gets all players in the same team as the specified player.
             * @param client The client connection.
             * @return A map of player IDs to shared pointers of PlayerLogic objects.
             */
            std::map<int, std::shared_ptr<PlayerLogic>> getMyTeam(std::shared_ptr<rtype::net::AConnection<GameServerMsgTypes>> client);

            /**
             * @brief Gets all players in the opposing team of the specified player.
             * @param id The ID of the player.
             * @return A map of player IDs to shared pointers of PlayerLogic objects.
             */
            std::map<int, std::shared_ptr<PlayerLogic>> getEnnemyTeam(int id);

            /**
             * @brief Gets all players in the opposing team of the specified player.
             * @param client The client connection.
             * @return A map of player IDs to shared pointers of PlayerLogic objects.
             */
            std::map<int, std::shared_ptr<PlayerLogic>> getEnnemyTeam(std::shared_ptr<rtype::net::AConnection<GameServerMsgTypes>> client);

            /**
             * @brief Gets the team ID of the specified player.
             * @param id The ID of the player.
             * @return The team ID.
             */
            int getMyTeamId(int id);

            /**
             * @brief Gets the team ID of the specified player.
             * @param client The client connection.
             * @return The team ID.
             */
            int getMyTeamId(std::shared_ptr<rtype::net::AConnection<GameServerMsgTypes>> client);

            /**
             * @brief Gets the team ID of the opposing team of the specified player.
             * @param id The ID of the player.
             * @return The team ID.
             */
            int getEnnemyTeamId(int id);

            /**
             * @brief Gets the team ID of the opposing team of the specified player.
             * @param client The client connection.
             * @return The team ID.
             */
            int getEnnemyTeamId(std::shared_ptr<rtype::net::AConnection<GameServerMsgTypes>> client);

            /**
             * @brief Gets the ID of the specified player.
             * @param client The client connection.
             * @return The player ID.
             */
            int getMyId(std::shared_ptr<rtype::net::AConnection<GameServerMsgTypes>> client);

            /**
             * @brief Checks if team one is ready.
             * @return True if team one is ready, false otherwise.
             */
            bool isTeamOneReady();

            /**
             * @brief Checks if team two is ready.
             * @return True if team two is ready, false otherwise.
             */
            bool isTeamTwoReady();

            /**
             * @brief Parses the map data.
             * @param map The map data.
             */
            void parseMap(std::string map);

            /**
             * @brief Handles the jump action for the specified player.
             * @param id The ID of the player.
             */
            void HandleJump(int id);

            /**
             * @brief Updates the gravity for the specified player.
             * @param deltatime The time since the last update.
             * @param player The player to update.
             */
            void updateGravity(float deltatime, std::shared_ptr<PlayerLogic> player);

            /**
             * @brief Checks if team one has any alive players.
             * @return True if team one has alive players, false otherwise.
             */
            bool teamOneAlive();

            /**
             * @brief Checks if team two has any alive players.
             * @return True if team two has alive players, false otherwise.
             */
            bool teamTwoAlive();

            /**
             * @brief Checks if the round is over.
             */
            void checkRound();

            /**
             * @brief Resets the round for the specified team.
             * @param team The team to reset.
             */
            void resetRound(int team);

            /**
             * @brief Checks if the game has ended.
             * @return True if the game has ended, false otherwise.
             */
            bool endGame();

            std::map<int, std::shared_ptr<PlayerLogic>> teamOne; /**< The players in team one. */
            std::map<int, std::shared_ptr<PlayerLogic>> teamTwo; /**< The players in team two. */

            int mapToLoad = -1; /**< The ID of the map to load. */
            float deltatime = 0; /**< The time since the last update. */
            std::string pathToMaps = "./maps/"; /**< The path to the maps directory. */
            std::map<int, std::shared_ptr<ECS::Ecs3D::IEntity>> map; /**< The map entities. */
            std::map<int, BoundingBox> mapBoundingBox; /**< The bounding boxes of the map entities. */

            int scoreTeamOne = 0; /**< The score of team one. */
            int scoreTeamTwo = 0; /**< The score of team two. */

            Model CharacterModel = { 0 }; /**< The character model. */
            Model HeadCharacterModel = { 0 }; /**< The head character model. */

            game_history_t gameHistory = {0}; /**< The game history. */

    };
}
