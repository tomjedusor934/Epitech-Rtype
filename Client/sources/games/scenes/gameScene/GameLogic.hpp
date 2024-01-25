/*
** EPITECH PROJECT, 2024
** RType
** File description:
** GameLogic
*/

#pragma once
#include "../../../../includes/CommonIncludes.hpp"
#include "playerLogic/PlayerLogic.hpp"

namespace rtype {
    class GameLogic {
        public:
            GameLogic();
            ~GameLogic();

            void addPlayer(std::string name, int id, int team, Vector3 spawnPositon, Quaternion spawnRotation, player_info_t data);
            void parseMap(std::string map);
            int countLineFile(std::string path);

            std::shared_ptr<PlayerLogic> getPlayer(int id);
            std::map<int, std::shared_ptr<PlayerLogic>> getMyTeam(int id);
            std::map<int, std::shared_ptr<PlayerLogic>> getEnnemyTeam(int id);
            int getMyTeamId(int id);
            int getEnnemyTeamId(int id);

            void updateGravity(float deltatime, int id);
            void jump(int id);
            void reloadWeapon(int id);
            void updateReloading(float deltatime, int id);
            void updateListener(int id);

            void updateTimeBetweenShoot(float deltatime);
            void animationShootGun();
            bool isAnimated = false;
            float timeAnimation = 0.0f;

            float timebetweenShoot = 0.0f;
            
            void updateMySoundPos(int id);

            Light CreateLight(int type, Vector3 position, Vector3 target, Color color, Shader shader);

            void UpdateLightValues(Shader shader, Light light);

            std::map<int, std::shared_ptr<PlayerLogic>> teamOne;
            std::map<int, std::shared_ptr<PlayerLogic>> teamTwo;

            std::string pathToMaps = "./maps/";

            std::map<int, std::shared_ptr<ECS::Ecs3D::IEntity>> map;

            std::map<int, BoundingBox> mapBoundingBox;

            int hitMarker = 0;
            int lightsCount = 0;

            int nb_light = 0;

            Shader shader;

            void addLightInMap(Light light, Color color, Vector3 position, int type, int id);

            std::map<int, Light[MAX_LIGHTS]> lights;

            // Light lights[MAX_LIGHTS] = { 0 };


            // sf::SoundBuffer bufferFootStep;
            // sf::Sound footStepSound;

            // sf::SoundBuffer bufferShoot;
            // sf::Sound shootSound;

            Model GunModel = { 0 };
            Vector3 gunOffset = { 0, 0, 1 };
            float timeResetPosGun = 0;
            bool isZoom = false;

            Model CharacterHeadModel = { 0 };
            Model CharacterBodyModel = { 0 };
            player_info_t playerInfo = { 0 };

            Texture2D enemyAlive;
            Texture2D enemyDead;
            Texture2D alliesAlive;
            Texture2D alliesDead;

            std::vector<int> winTeam;

            int scoreTeamOne = 0;
            int scoreTeamTwo = 0;
    };
}
