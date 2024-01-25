/*
** EPITECH PROJECT, 2023
** ecs_rType
** File description:
** hitboxgravity
*/

#pragma once
#include "position3d.hpp"

namespace rtype::ECS::Ecs3D {
    class HitboxGravityComponent : public IComponent {
        public:
            // drawcubewires
            PositionComponent3d* positionComponent;
            float width;
            float height;
            float length;
            Color color;
            std::string id;

            // Constructor
            HitboxGravityComponent(PositionComponent3d* positionComponent, float width, float height, float length, Color color, std::string id)
                : positionComponent(positionComponent), width(width), height(height), length(length), color(color), id(id) {}

            // Setters
            void setHitboxGravity(float newWidth, float newHeight, float newLength, Color newColor) {
                width = newWidth;
                height = newHeight;
                length = newLength;
                color = newColor;
            }

            // Infos
            void printHitboxGravity() {
                std::cout << "HitboxGravity: " << width << " " << height << " " << length << std::endl;
            }
    };
}