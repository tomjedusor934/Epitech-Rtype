/*
** EPITECH PROJECT, 2024
** RType
** File description:
** IScene
*/

#pragma once
#include "common.hpp"
#include "entity.hpp"
#include <memory>


namespace rtype::ECS::Ecs3D {
    /**
     * @brief Interface for a 3D scene in the ECS system.
     */
    class IScene {
        public:

            /**
             * @brief Update the scene.
             */
            virtual void Update() = 0;

            /**
             * @brief Draw the scene.
             */
            virtual void Draw() = 0;

            /**
             * @brief Add an entity to the scene.
             * @param entity The entity to add.
             */
            virtual void addEntity(std::shared_ptr<IEntity> entity) = 0;

            /**
             * @brief Remove an entity from the scene.
             * @param entity The entity to remove.
             */
            virtual void removeEntity(std::shared_ptr<IEntity> entity) = 0;

            /**
             * @brief Get all entities in the scene.
             * @return A map of entities, where the key is the entity's name tag and the value is the entity itself.
             */
            virtual std::map<std::string, std::shared_ptr<IEntity>> getEntities() = 0;

            /**
             * @brief Get an entity from the scene by its name tag.
             * @param nameTag The name tag of the entity.
             * @return The entity with the specified name tag, or nullptr if not found.
             */
            virtual std::shared_ptr<IEntity> getEntity(std::string nameTag) = 0;

            /**
             * @brief Virtual destructor.
             */
            virtual ~IScene() = default;

        protected:
        private:
    };
};