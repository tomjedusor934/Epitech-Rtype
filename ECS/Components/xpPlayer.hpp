/*
** EPITECH PROJECT, 2023
** ecs_rType
** File description:
** postion
*/

#pragma once
#include "interface.hpp"

namespace rtype::ECS::Ecs3D {
    class XpPlayer : public IComponent {
        public:
            std::string id;
            float xp;

            // Constructor
            XpPlayer(std::string id, float xp)
                : id(id), xp(xp) {}
    };
}