/*
** EPITECH PROJECT, 2023
** ecs_rType
** File description:
** buttonmenu
*/

#pragma once
#include "interface.hpp"

namespace rtype::ECS::Ecs3D {
    class Model3d : public IComponent {
        public:

            std::string id;
            Model model;
            int animsCount;
            unsigned int animCurrentFrame;
            unsigned int animIndex;
            ModelAnimation *modelAnimations;
            Vector3 scale;
            Vector3 rotationAxis;
            float rotationAngle;
            float animationSpeed;
            float animationTime;

            // Constructor
            Model3d(std::string id, Model model, int animsCount, unsigned int animCurrentFrame, unsigned int animIndex, ModelAnimation *modelAnimations, Vector3 scale, Vector3 rotationAxis, float rotationAngle, float animationSpeed)
                : id(id), model(model), animsCount(animsCount), animCurrentFrame(animCurrentFrame), animIndex(animIndex), modelAnimations(modelAnimations), scale(scale), rotationAxis(rotationAxis), rotationAngle(rotationAngle), animationSpeed(animationSpeed) {
                animationTime = 0;
            }

            Model3d(std::string id, Model model, Vector3 startPos) : id(id), model(model) {
                animsCount = 0;
                animCurrentFrame = 0;
                animIndex = 0;
                modelAnimations = NULL;
                scale = {1.0f, 1.0f, 1.0f};
                rotationAxis = {0.0f, 0.0f, 0.0f};
                rotationAngle = 0.0f;
                animationSpeed = 1.0f;
                animationTime = 0;
                model.transform = MatrixTranslate(startPos.x, startPos.y, startPos.z);
            }

            Model3d(std::string id, Model model, Vector3 startPos, Vector3 scale) : id(id), model(model), scale(scale) {
                animsCount = 0;
                animCurrentFrame = 0;
                animIndex = 0;
                modelAnimations = NULL;
                rotationAxis = {0.0f, 0.0f, 0.0f};
                rotationAngle = 0.0f;
                animationSpeed = 1.0f;
                animationTime = 0;
                model.transform = MatrixTranslate(startPos.x, startPos.y, startPos.z);
            }

            void applyRotation(Vector3 angle) {
                model.transform = MatrixRotateXYZ(angle);
            }
    };
}