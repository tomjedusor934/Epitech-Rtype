/*
** EPITECH PROJECT, 2023
** ecs_rType
** File description:
** buttonmenu
*/

#pragma once
#include "interface.hpp"

namespace rtype::ECS::Ecs3D {
    class ButtonMenu : public IComponent {
        public:
            Rectangle button;
            std::string id;
            bool isHover = false;
            Vector2 initialPosition;
            Texture2D texture;

            // Constructor
            ButtonMenu(Rectangle button, std::string id)
                : button(button), id(id) {
                    initialPosition = {button.x, button.y};
            }


            // Setters
            void setButton(Rectangle newButton) {
                button = newButton;
            }

            // Infos
            void printButton() {
                std::cout << "Button: " << button.x << " " << button.y << " " << button.width << " " << button.height << std::endl;
            }
    };
}