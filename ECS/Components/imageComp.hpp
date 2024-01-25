/*
** EPITECH PROJECT, 2023
** ecs_rType
** File description:
** buttonmenu
*/

#pragma once
#include "interface.hpp"

namespace rtype::ECS::Ecs3D {
    class ImageComp : public IComponent {
        public:
            Image image;
            std::string id;

            // Constructor
            ImageComp(std::string filePath, std::string id) {
                image = LoadImage(filePath.c_str());
                this->id = id;
            }
    };
}