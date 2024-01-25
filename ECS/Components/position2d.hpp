/*
** EPITECH PROJECT, 2023
** ecs_rType
** File description:
** postion
*/

#pragma once
#include "interface.hpp"

namespace rtype::ECS::Ecs3D {
    class PositionComponent2d : public IComponent {
        public:
            Vector2 position;
            std::string id;

            // Constructor
            PositionComponent2d(Vector2 initialPosition, std::string id)
                : position(initialPosition), id(id) {}

            PositionComponent2d()
            {
                position = {0, 0};
                id = "default";
            }

            // Setters
            void setPosition(Vector2 newPosition) {
                position = newPosition;
            }

            // Infos
            void printPosition() {
                std::cout << "Position: " << position.x << " " << position.y << " " << std::endl;
            }
    };
}