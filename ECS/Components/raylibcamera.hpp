/*
** EPITECH PROJECT, 2023
** ecs_rType
** File description:
** raylibcamera
*/

#pragma once
#include "interface.hpp"

namespace rtype::ECS::Ecs3D {
    class RayLibCameraComponent : public IComponent {
        public:
            // id of the component
            std::string id;

            // Camera from raylib
            Camera camera;

            // Constructor
            RayLibCameraComponent(
                std::string id,
                Vector3 position,
                Vector3 target,
                Vector3 up,
                float fovy,
                int projection
            )
            : id(id) {
                camera.position = position;
                camera.target = target;
                camera.up = up;
                camera.fovy = fovy;
                camera.projection = projection;
            }

            RayLibCameraComponent(std::string id)
            : id(id) {
                camera.position = {0};
                camera.target = {0};
                camera.up = {0, 1, 0};
                camera.fovy = 60;
                camera.projection = CAMERA_PERSPECTIVE;
            }

            RayLibCameraComponent(std::string, Camera newCamera)
            : camera(newCamera) {}


            // Setters
            void setCamera(Camera newCamera) {
                camera = newCamera;
            }

            void updateCamera(Vector3 position, Vector3 target, float zoom) {
                UpdateCameraPro(&camera, position, target, zoom);
            }

            // Infos
            void printCamera() {
                std::cout << "Camera: " << std::endl;
                std::cout << "Position: " << camera.position.x << ", " << camera.position.y << ", " << camera.position.z << std::endl;
                std::cout << "Target: " << camera.target.x << ", " << camera.target.y << ", " << camera.target.z << std::endl;
                std::cout << "Up: " << camera.up.x << ", " << camera.up.y << ", " << camera.up.z << std::endl;
                std::cout << "Fovy: " << camera.fovy << std::endl;
                std::cout << "Projection: " << camera.projection << std::endl;
            }
    };
}