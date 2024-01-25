/*
** EPITECH PROJECT, 2024
** RType
** File description:
** ISettings
*/

#pragma once
    #include "../../../../includes/CommonIncludes.hpp"

namespace rtype {
    class ISettings : public ECS::Ecs3D::AScene {
        public:
            virtual ~ISettings() = default;
            virtual void draw() = 0;
            virtual void update(float deltatime, float getTime) = 0;
            void drawTextInRectangle(std::initializer_list<std::string> texts, Rectangle rectangle, Font font, Color color, float fontSize);
            std::map<std::string, std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>>> createBindInBox(std::initializer_list<std::string> texts, std::initializer_list<std::string> values, Rectangle rectangle, Font font, Color textColor, float fontSize);
            int WhichBindisClikcked(std::map<std::string, std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>>> map);
            void UpdateBinding(std::map<std::string, std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>>> &map, int &rectVisible);
            void UpdateCoche(std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> coches, std::vector<bool> &enabled);
            void DrawCoche(std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> coches, std::vector<bool> enabled);
            void UpdateJaugeBar(std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> bars, std::vector<float> &level);
            void DrawJaugeBar(std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> bars, std::vector<float> level);

        protected:
        private:
    };
};
