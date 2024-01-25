/*
** EPITECH PROJECT, 2023
** ecs_rType
** File description:
** postion
*/

#pragma once
#include "interface.hpp"
#include "position2d.hpp"

namespace rtype {
    namespace ECS {
        namespace Ecs3D {
            class ColorComponent : public IComponent {
                public:
                    std::string id;
                    Color color;

                    // Constructor
                    ColorComponent(std::string id, Color color)
                        : id(id), color(color) {}
            };
        }
    }
}