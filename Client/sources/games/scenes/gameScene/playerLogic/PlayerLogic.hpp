/*
** EPITECH PROJECT, 2024
** RType
** File description:
** PlayerLogic
*/

#pragma once
#include "../../../../../includes/CommonIncludes.hpp"

namespace rtype {
    class PlayerLogic {
        public:
            PlayerLogic() {};
            ~PlayerLogic() {};

            std::shared_ptr<ECS::Ecs3D::IEntity> game;
            player_info_t data = {0};
            BoundingBox hitbox = {0};
            Vector3 spawnPosition = {0};
            Quaternion spawnRotation = {0};
    };
}