/*
** EPITECH PROJECT, 2023
** ecs_rType
** File description:
** orientation
*/

#pragma once
#include "interface.hpp"

namespace rtype::ECS::Ecs3D {
    class OrientationComponent : public IComponent {
        public:
            Vector3 orientation;
            std::string id;

            // Constructor
            OrientationComponent(Vector3 initialOrientation, std::string id)
                : orientation(initialOrientation), id(id) {}

            // Setters
            void setOrientation(Vector3 newOrientation) {
                orientation = newOrientation;
            }

            // Infos
            void printOrientation() {
                std::cout << "Orientation: " << orientation.x << " " << orientation.y << " " << orientation.z << std::endl;
            }
    };
}