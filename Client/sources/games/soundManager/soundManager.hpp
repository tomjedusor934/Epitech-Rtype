/*
** EPITECH PROJECT, 2024
** RType
** File description:
** soundManager
*/

#pragma once

#include "../../../../Includes/Includes.hpp"

namespace rtype {
    class SoundManager {
        public:
            SoundManager();
            ~SoundManager();

            // functs
            void updateListener(Vector3 position, Vector3 direction);
            void updateVolume(std::string type, float volume);


            sf::Listener listener;

            sf::SoundBuffer bufferFootStep;
            sf::Sound footStepSound;
            sf::SoundBuffer bufferMyFootStep;
            sf::Sound myFootStepSound;

            sf::SoundBuffer bufferShoot;
            sf::Sound shootSound;
            sf::SoundBuffer bufferMyShoot;
            sf::Sound myShootSound;

            sf::SoundBuffer bufferHit;
            sf::Sound hitSound;

            sf::SoundBuffer bufferHeadShot;
            sf::Sound headShotSound;

            sf::SoundBuffer bufferMenu;
            sf::Sound menuSound;

        protected:
        private:
    };
}