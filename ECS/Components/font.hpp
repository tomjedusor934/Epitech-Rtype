/*
** EPITECH PROJECT, 2024
** RType
** File description:
** font
*/

#pragma once
#include "interface.hpp"

namespace rtype {
    namespace ECS {
        namespace Ecs3D {
            class FontComponent : public IComponent {
                public:
                    std::string id;
                    float size;
                    std::string filepath;
                    Font font;

                    // Constructor
                    FontComponent(std::string id, float size, std::string filepath)
                        : id(id), size(size), filepath(filepath) {
                            font = LoadFontEx(filepath.c_str(), size, 0, 250);
                        }
            };
        }
    }
}