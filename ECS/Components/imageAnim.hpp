/*
** EPITECH PROJECT, 2023
** ecs_rType
** File description:
** buttonmenu
*/

#pragma once
#include "interface.hpp"

namespace rtype::ECS::Ecs3D {
    class ImageAnim : public IComponent {
        public:
            Image image;
            std::string id;
            int animFrames;

            // Constructor
            ImageAnim(std::string filePath, std::string id, int animFrames) {
                this->animFrames = animFrames;
                image = LoadImageAnim(filePath.c_str(), &animFrames);
                this->id = id;
            }
    };
}