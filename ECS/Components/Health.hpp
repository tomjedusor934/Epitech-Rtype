/*
** EPITECH PROJECT, 2023
** ecs_rType
** File description:
** postion
*/

#pragma once
#include "interface.hpp"
#include "position2d.hpp"

namespace rtype::ECS::Ecs3D {
    class Health : public IComponent {
        public:
            std::string id;
            float health;

            // Constructor
            Health(std::string id, float health)
                : id(id), health(health) {}
    };
}