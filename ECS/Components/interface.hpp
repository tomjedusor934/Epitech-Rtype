/*
** EPITECH PROJECT, 2023
** ecs_rType
** File description:
** components
*/

#pragma once
#include "../common.hpp"

namespace rtype::ECS::Ecs3D {
    class IComponent {
        public:
            virtual ~IComponent() = default;
    };
}