/*
** EPITECH PROJECT, 2023
** ecs_rType
** File description:
** postion
*/

#pragma once
#include "interface.hpp"

namespace rtype::ECS::Ecs3D {
    class PositionComponent3d : public IComponent {
        public:
            Vector3 position;
            std::string id;

            // Constructor
            PositionComponent3d(Vector3 initialPosition, std::string id)
                : position(initialPosition), id(id) {}

            // Setters
            void setPosition(Vector3 newPosition) {
                position = newPosition;
            }

            // Infos
            void printPosition() {
                std::cout << "Position: " << position.x << " " << position.y << " " << position.z << std::endl;
            }
    };
}