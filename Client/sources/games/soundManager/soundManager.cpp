/*
** EPITECH PROJECT, 2024
** RType
** File description:
** soundManager
*/

#include "./soundManager.hpp"

rtype::SoundManager::SoundManager()
{
    this->bufferFootStep.loadFromFile("Media/footstep_mono.ogg");
    this->footStepSound.setBuffer(this->bufferFootStep);
    this->footStepSound.setMinDistance(10.0f);
    this->footStepSound.setAttenuation(1.5f);
    this->footStepSound.setVolume(50.0f);
    this->footStepSound.setPitch(1.0f);

    this->bufferMyFootStep.loadFromFile("Media/footstep.ogg");
    this->myFootStepSound.setBuffer(this->bufferMyFootStep);
    this->myFootStepSound.setVolume(100.0f);
    this->myFootStepSound.setPitch(1.0f);

    this->bufferShoot.loadFromFile("Media/guardian.ogg");
    this->shootSound.setBuffer(this->bufferShoot);
    this->shootSound.setMinDistance(10.0f);
    this->shootSound.setAttenuation(1.5f);
    this->shootSound.setVolume(50.0f);
    this->shootSound.setPitch(1.0f);

    this->bufferMyShoot.loadFromFile("Media/MyGuardian.ogg");
    this->myShootSound.setBuffer(this->bufferMyShoot);
    this->myShootSound.setVolume(100.0f);
    this->myShootSound.setPitch(1.0f);

    this->bufferHit.loadFromFile("Media/hitmarker.ogg");
    this->hitSound.setBuffer(this->bufferHit);
    this->hitSound.setVolume(100.0f);
    this->hitSound.setPitch(1.0f);

    this->bufferHeadShot.loadFromFile("Media/headshot.ogg");
    this->headShotSound.setBuffer(this->bufferHeadShot);
    this->headShotSound.setVolume(100.0f);
    this->headShotSound.setPitch(1.0f);


    this->bufferMenu.loadFromFile("Media/lobby.ogg");
    this->menuSound.setBuffer(this->bufferMenu);
}

rtype::SoundManager::~SoundManager()
{
}

void rtype::SoundManager::updateListener(Vector3 position, Vector3 direction)
{
    this->listener.setPosition(position.x, position.y, position.z);
    this->listener.setDirection(direction.x, direction.y, direction.z);
}

void rtype::SoundManager::updateVolume(std::string type, float volume)
{
    if (type == "MasterVolume") {
    } else if (type == "Effects") {
        this->myShootSound.setVolume(volume);
        this->shootSound.setVolume(volume);
        this->footStepSound.setVolume(volume);
        this->myFootStepSound.setVolume(volume);
        this->hitSound.setVolume(volume);
        this->headShotSound.setVolume(volume);
    } else if (type == "Music") {
        this->menuSound.setVolume(volume);
    }
}