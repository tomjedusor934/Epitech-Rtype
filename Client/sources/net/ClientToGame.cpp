/*
** EPITECH PROJECT, 2023
** RType
** File description:
** ClientToGame
*/

#include "ClientToGame.hpp"

void rtype::GameManager::ClientConnected(rtype::net::message<GameServerMsgTypes> &msg)
{
    this->_scene = _Scene::LOADING;
    int mapToLoad = 0;
    msg >> mapToLoad;
    this->_gameLogic.parseMap(std::to_string(mapToLoad) + ".txt");
}

void rtype::GameManager::SpawnPosition(rtype::net::message<GameServerMsgTypes> &msg)
{
    int id = 0;
    int team = 0;
    Quaternion spawnRotation = {0};
    Vector3 spawnPosition = {0};
    player_info_t data = {0};
    msg >> id;
    msg >> team;
    msg >> spawnRotation;
    msg >> spawnPosition;
    msg >> data;
    this->_scene = _Scene::GAME;
    DisableCursor();
    this->_gameLogic.addPlayer("me", id, team, spawnPosition, spawnRotation, data);
}

void rtype::GameManager::OtherSpawnPosition(rtype::net::message<GameServerMsgTypes> &msg)
{
    int id = 0;
    int team = 0;
    Quaternion spawnRotation = {0};
    Vector3 spawnPosition = {0};
    player_info_t data = {0};
    msg >> id;
    msg >> team;
    msg >> spawnRotation;
    msg >> spawnPosition;
    msg >> data;
    this->_gameLogic.addPlayer("other", id, team, spawnPosition, spawnRotation, data);
}

void rtype::GameManager::OtherPlayerJump(rtype::net::message<GameServerMsgTypes> &msg)
{
    int id = 0;
    msg >> id;
    std::shared_ptr<PlayerLogic> player = this->_gameLogic.getPlayer(id);
    if (player == nullptr)
        return;
    this->_gameLogic.jump(id);
}

void rtype::GameManager::PlayerMove(rtype::net::message<GameServerMsgTypes> &msg)
{
    std::shared_ptr<PlayerLogic> player = this->_gameLogic.getPlayer(this->_id);
    if (player == nullptr)
        return;
    Vector3 newPosition = {0};
    msg >> newPosition;
    Vector3 oldPosition = player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetWorldPosition();
    player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->SetPositionX(newPosition.x);
    player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->SetPositionY(newPosition.y);
    player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->SetPositionZ(newPosition.z);
    Vector3 diff = Vector3Subtract(newPosition, oldPosition);
    player->hitbox.min = Vector3Add(player->hitbox.min, diff);
    player->hitbox.max = Vector3Add(player->hitbox.max, diff);
    Camera camera = player->game->getComponent<ECS::Ecs3D::RayLibCameraComponent>()->camera;
    camera.position = player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->GetWorldPosition();
    camera.target = Vector3Transform(Vector3{0,0,1}, player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->GetWorldMatrix());
    player->game->getComponent<ECS::Ecs3D::RayLibCameraComponent>()->camera = camera;
}

void rtype::GameManager::OtherPlayerMove(rtype::net::message<GameServerMsgTypes> &msg)
{
    int team = 0;
    int id = 0;
    Vector3 newPosition = {0};
    msg >> team;
    msg >> id;
    msg >> newPosition;
    if (id == this->_id)
        return;
    std::shared_ptr<PlayerLogic> player = this->_gameLogic.getPlayer(id);
    if (player == nullptr)
        return;
    Vector3 oldPosition = player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetWorldPosition();
    player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->SetPositionX(newPosition.x);
    player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->SetPositionY(newPosition.y);
    player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->SetPositionZ(newPosition.z);
    Vector3 diff = Vector3Subtract(newPosition, oldPosition);
    player->hitbox.min = Vector3Add(player->hitbox.min, diff);
    player->hitbox.max = Vector3Add(player->hitbox.max, diff);

    Camera camera = player->game->getComponent<ECS::Ecs3D::RayLibCameraComponent>()->camera;
    camera.position = player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->GetWorldPosition();
    camera.target = Vector3Transform(Vector3{0,0,1}, player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->GetWorldMatrix());
    player->game->getComponent<ECS::Ecs3D::RayLibCameraComponent>()->camera = camera; 
    this->_soundManager.footStepSound.setPosition(camera.position.x, camera.position.y, camera.position.z);
    // play footstep sound if the sound is not already playing
    if (this->_soundManager.footStepSound.getStatus() != sf::Sound::Playing) {
        this->_soundManager.footStepSound.play();
    }
}

void rtype::GameManager::PlayerLook(rtype::net::message<GameServerMsgTypes> &msg)
{
    std::shared_ptr<PlayerLogic> player = this->_gameLogic.getPlayer(this->_id);
    if (player == nullptr)
        return;
    Quaternion headRotation = {0};
    Quaternion bodyRotation = {0};
    msg >> headRotation;
    msg >> bodyRotation;
    player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->SetOrientation(bodyRotation);
    player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->SetOrientation(headRotation);

    Camera camera = player->game->getComponent<ECS::Ecs3D::RayLibCameraComponent>()->camera;
    camera.position = player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->GetWorldPosition();
    camera.target = Vector3Transform(Vector3{0,0,1}, player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->GetWorldMatrix());
    player->game->getComponent<ECS::Ecs3D::RayLibCameraComponent>()->camera = camera;
}

void rtype::GameManager::OtherPlayerLook(rtype::net::message<GameServerMsgTypes> &msg)
{
    int team = 0;
    int id = 0;
    Quaternion headRotation = {0};
    Quaternion bodyRotation = {0};
    msg >> team;
    msg >> id;
    msg >> headRotation;
    msg >> bodyRotation;
    if (id == this->_id)
        return;
    std::shared_ptr<PlayerLogic> player = this->_gameLogic.getPlayer(id);
    if (player == nullptr)
        return;
    player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->SetOrientation(bodyRotation);
    player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->SetOrientation(headRotation);
    Camera camera = player->game->getComponent<ECS::Ecs3D::RayLibCameraComponent>()->camera;
    camera.position = player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->GetWorldPosition();
    camera.target = Vector3Transform(Vector3{0,0,1}, player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->GetWorldMatrix());
    player->game->getComponent<ECS::Ecs3D::RayLibCameraComponent>()->camera = camera;
}

void rtype::GameManager::PlayerHitted(rtype::net::message<GameServerMsgTypes> &msg)
{
    std::cout << "HITTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT" << std::endl;
    this->_gameLogic.hitMarker = 100;
    bool isHeadShot = true;
    msg >> isHeadShot;
    if (isHeadShot)
        this->_soundManager.headShotSound.play();
    else
        this->_soundManager.hitSound.play();
}

void rtype::GameManager::SomeoneGotHitted(rtype::net::message<GameServerMsgTypes> &msg)
{
    int id = 0;
    int damageReceived = 0;
    msg >> damageReceived;
    msg >> id;
    std::shared_ptr<PlayerLogic> player = this->_gameLogic.getPlayer(id);
    if (player == nullptr)
        return;
    player->game->getComponent<ECS::Ecs3D::Health>()->health -= damageReceived;
    std::cout << "QUELQU'UN A MAAAAAAAAAAALLLLLLLLLLLLL" << std::endl;
}

void rtype::GameManager::OtherPlayerShoot(rtype::net::message<GameServerMsgTypes> &msg)
{
    int id = 0;
    int team = 0;
    msg >> team;
    msg >> id;
    std::shared_ptr<PlayerLogic> player = this->_gameLogic.getPlayer(id);
    if (player == nullptr)
        return;
    Camera camera = player->game->getComponent<ECS::Ecs3D::RayLibCameraComponent>()->camera;
    Vector3 position = camera.position;
    this->_soundManager.shootSound.setPosition(position.x, position.y, position.z);

    this->_soundManager.shootSound.play();
}

void rtype::GameManager::NextRound(rtype::net::message<GameServerMsgTypes> &msg)
{
    int team = 0;
    msg >> team;
    if (team == 1) {
        this->_gameLogic.scoreTeamOne += 1;
        this->_gameLogic.winTeam.push_back(1);
    } else {
        this->_gameLogic.scoreTeamTwo += 1;
        this->_gameLogic.winTeam.push_back(2);
    }

    for (const auto& team : {this->_gameLogic.teamOne, this->_gameLogic.teamTwo}) {
        for (auto &player : team) {
            player.second->game->getComponent<ECS::Ecs3D::Health>()->health = 100;
            player.second->game->getComponent<ECS::Ecs3D::ObjectTransform>()->SetPosition(player.second->spawnPosition);
            player.second->game->getComponent<ECS::Ecs3D::ObjectTransform>()->SetOrientation(player.second->spawnRotation);
            player.second->game->getComponent<ECS::Ecs3D::ObjectTransform>()->MoveV(3);
            player.second->game->getComponent<ECS::Ecs3D::AmmoComponent>()->nb_ammo = 8;
            player.second->hitbox = GetModelBoundingBox(this->_gameLogic.CharacterBodyModel);
            player.second->hitbox.min = Vector3Add(player.second->hitbox.min, player.second->spawnPosition);
            player.second->hitbox.max = Vector3Add(player.second->hitbox.max, player.second->spawnPosition);
            player.second->hitbox.min = Vector3Add(player.second->hitbox.min, {0, 3, 0});
            player.second->hitbox.max = Vector3Add(player.second->hitbox.max, {0, 4, 0});
        }
    }
}

void rtype::GameManager::UpdateScoreboard(rtype::net::message<GameServerMsgTypes> &msg)
{
    size_t nbAssists = 0;
    msg >> nbAssists;
    for (size_t i = 0; i < nbAssists; ++i) {
        int id = 0;
        msg >> id;
        std::shared_ptr<PlayerLogic> player = this->_gameLogic.getPlayer(id);
        if (player == nullptr)
            return;
        player->data.assists += 1;
    }
    int deathId = 0;
    msg >> deathId;
    std::shared_ptr<PlayerLogic> player = this->_gameLogic.getPlayer(deathId);
    if (player == nullptr)
        return;
    player->data.deaths += 1;
    int killerId = 0;
    msg >> killerId;
    player = this->_gameLogic.getPlayer(killerId);
    if (player == nullptr)
        return;
    player->data.kills += 1;
}

void rtype::GameManager::GameEnded(rtype::net::message<GameServerMsgTypes> &msg)
{
    int winningTeam = 0;
    msg >> winningTeam;
    this->_scene = _Scene::RESULT;
    EnableCursor();
}
