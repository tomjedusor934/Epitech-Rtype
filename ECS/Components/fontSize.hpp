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
            class FontSize : public IComponent {
                public:
                    std::string id;
                    float size;

                    // Constructor
                    FontSize(std::string id, float size)
                        : id(id), size(size) {}
            };
        }
    }
}