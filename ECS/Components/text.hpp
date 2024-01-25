/*
** EPITECH PROJECT, 2023
** RType
** File description:
** text
*/

#pragma once
#include "interface.hpp"

namespace rtype::ECS::Ecs3D {
    class Text : public IComponent {
        public:
            std::string id;
            Font font;
            std::string text;
            Vector2 position;
            float fontSize;
            float spacing;
            Color tint;

            // Constructor
            Text(std::string id, Font font, std::string text, Vector2 position, float fontSize, float spacing, Color tint
            ) : id(id), font(font), text(text), position(position), fontSize(fontSize), spacing(spacing), tint(tint) {}
    };
}