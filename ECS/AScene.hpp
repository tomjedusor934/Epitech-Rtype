/*
** TALWEB PROJECT, 2023
** ecs_rType
** File description:
** ecs
*/

#pragma once
#include "common.hpp"
#include "entity.hpp"
#include "IScene.hpp"

namespace rtype::ECS::Ecs3D {
    /**
     * @brief Abstract base class for scenes in the ECS system.
     */
    class AScene {
    public:
        /**
         * @brief Default destructor.
         */
        virtual ~AScene() = default;

        /**
         * @brief Pure virtual function to draw the scene.
         */
        virtual void draw() = 0;

        /**
         * @brief Pure virtual function to update the scene.
         * @param deltatime The time elapsed since the last update.
         * @param getTime The current time.
         */
        virtual void update(float deltatime, float getTime) = 0;

        /**
         * @brief Function to add an entity to the scene.
         * @param entity The entity to be added.
         */
        void addEntity(std::shared_ptr<IEntity> entity) {
            entities.insert(std::pair<std::string, std::shared_ptr<IEntity>>(entity->getName(), entity));
        }

        /**
         * @brief Function to add multiple entities to the scene.
         * @param entities The vector of entities to be added.
         */
        void addEntity(std::vector<std::shared_ptr<IEntity>> entities) {
            for (auto &entity : entities) {
                addEntity(entity);
            }
        }

        /**
         * @brief Function to remove an entity from the scene.
         * @param entity The entity to be removed.
         */
        void removeEntity(std::shared_ptr<IEntity> entity) {
            entities.erase(entity->getName());
        }

        /**
         * @brief Function to get all entities in the scene.
         * @return A map of entities, where the key is the entity's name and the value is the entity itself.
         */
        std::map<std::string, std::shared_ptr<IEntity>> getEntities() {
            return entities;
        }

        /**
         * @brief Function to get a specific entity from the scene.
         * @param nameTag The name tag of the entity.
         * @return The entity with the specified name tag, or nullptr if not found.
         */
        std::shared_ptr<IEntity> getEntity(std::string nameTag) {
            if (entities.find(nameTag) != entities.end()) {
                return entities[nameTag];
            } else {
                return nullptr;
            }
        }

        /**
         * @brief Function to edit an entity in the scene.
         * @param nameTag The name tag of the entity to be edited.
         * @param value The new value of the entity.
         */
        void editEntity(std::string nameTag, std::shared_ptr<IEntity> value) {
            if (entities.find(nameTag) != entities.end()) {
                entities[nameTag] = value;
            }
        }

    protected:
        std::map<std::string, std::shared_ptr<IEntity>> entities; /**< The map of entities in the scene. */
    };
}