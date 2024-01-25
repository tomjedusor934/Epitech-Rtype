/*
** EPITECH PROJECT, 2023
** ecs_rType
** File description:
** buttonmenu
*/

#pragma once
#include "interface.hpp"

namespace rtype::ECS::Ecs3D {
    class GifAnim : public IComponent {
        public:
            unsigned int nextFrameDataOffset = 0;
            int currentAnimFrame = 0;
            int frameDelay = 8;
            int frameCounter = 0;

            // Constructor
            GifAnim() = default;
    };
}