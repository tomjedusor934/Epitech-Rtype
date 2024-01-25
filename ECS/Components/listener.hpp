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
            class ListenerComponent : public IComponent {
                public:
                    std::string id;
                    sf::Listener listener;

                    // Constructor
                    ListenerComponent(std::string id)
                        : id(id) {}
            };
        }
    }
}