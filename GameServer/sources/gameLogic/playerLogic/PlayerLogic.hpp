/*
** EPITECH PROJECT, 2024
** RType
** File description:
** PlayerLogic
*/

#pragma once
#include "../../../includes/CommonIncludes.hpp"

namespace rtype {
    /**
     * @brief The PlayerLogic class represents the logic for a player in the game.
     * 
     * It contains information about the player, such as their client connection,
     * game entity, spawn position, rotation, hitbox, assists, and game history.
     */
    class PlayerLogic {
        public:
            /**
             * @brief Constructs a PlayerLogic object with the given data, client connection, and game entity.
             * 
             * @param data The player information.
             * @param client The client connection.
             * @param entity The game entity.
             */
            PlayerLogic(player_info_t data, std::shared_ptr<rtype::net::AConnection<GameServerMsgTypes>> client, std::shared_ptr<ECS::Ecs3D::IEntity> entity) : data(data), client(client), game(entity) {
            };

            /**
             * @brief Default constructor for PlayerLogic.
             */
            PlayerLogic() {};

            /**
             * @brief Default destructor for PlayerLogic.
             */
            ~PlayerLogic() = default;

            std::shared_ptr<rtype::net::AConnection<GameServerMsgTypes>> client; /**< The client connection. */
            std::shared_ptr<ECS::Ecs3D::IEntity> game; /**< The game entity. */
            Vector3 spawnPosition = {0}; /**< The spawn position. */
            Quaternion spawnRotation = {0}; /**< The spawn rotation. */
            player_info_t data = {0}; /**< The player information. */
            BoundingBox hitbox = {0}; /**< The hitbox. */
            std::vector<int> assists; /**< The list of assists. */
            game_history_player_t history = {0}; /**< The game history. */
    };
}