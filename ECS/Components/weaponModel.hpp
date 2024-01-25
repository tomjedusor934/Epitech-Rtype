/*
** EPITECH PROJECT, 2023
** ecs_rType
** File description:
** buttonmenu
*/

#pragma once
#include "interface.hpp"

namespace rtype::ECS::Ecs3D {
    class WeaponModel : public IComponent {
        public:

            std::string id;
            Model model;
            Vector3 scale;
            Vector3 rotationAxis;
            float rotationAngle;

            // Constructor
            WeaponModel(std::string id, Model model, Vector3 scale, Vector3 rotationAxis, float rotationAngle)
                : id(id), model(model), scale(scale), rotationAxis(rotationAxis), rotationAngle(rotationAngle) {
            }

            WeaponModel(std::string id, Model model, Vector3 startPos) : id(id), model(model) {
                scale = {0.5f, 0.5f, 0.5f};
                rotationAxis = {0.0f, 0.0f, 0.0f};
                rotationAngle = 0.0f;
                model.transform = MatrixTranslate(startPos.x, startPos.y, startPos.z);
            }

            WeaponModel(std::string id, Model model, Vector3 startPos, Vector3 rotationAxis) : id(id), model(model) {
                scale = {0.5f, 0.5f, 0.5f};
                rotationAngle = 0.0f;
                model.transform = MatrixTranslate(startPos.x, startPos.y, startPos.z);
            }
    };
}