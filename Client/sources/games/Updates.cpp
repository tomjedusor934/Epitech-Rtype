/*
** EPITECH PROJECT, 2023
** RType
** File description:
** Updates
*/

#include "../../includes/MainInclusdes.hpp"

namespace rtype::ECS::Ecs3D {
    void updateCameraPosition(Camera2D *camera, Vector2 playerPosition)
    {
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        camera->target.x = playerPosition.x - screenWidth / 2;
        camera->target.y = playerPosition.y - screenHeight / 2;
    }
}