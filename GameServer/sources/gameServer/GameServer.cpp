/*
** EPITECH PROJECT, 2023
** RType
** File description:
** GameServer
*/

#include "GameServer.hpp"

bool rtype::CustomUdpGameServer::OnClientConnect(std::shared_ptr<rtype::net::AConnection<GameServerMsgTypes>> client)
{
    std::cout << "New client connected" << std::endl;
    if (this->_canAcceptClients == 1) {
        std::cout << "Client accepted" << std::endl;
        rtype::net::message<GameServerMsgTypes> resp;
        resp.header.id = GameServerMsgTypes::ClientConnected;
        resp << this->gameLogic.mapToLoad;
        client->Send(resp);
        // this->_mtx.lock();
        // this->_canAcceptClients = 0;
        // this->_mtx.unlock();
        return true;
    } else {
        std::cout << "Client refused" << std::endl;
        return false;
    }
}

void rtype::CustomUdpGameServer::OnClientDisconnect(std::shared_ptr<rtype::net::AConnection<GameServerMsgTypes>> client)
{
    std::cout << "Client disconnected" << std::endl;
}

void rtype::CustomUdpGameServer::PlayerJump(std::shared_ptr<rtype::net::AConnection<GameServerMsgTypes>> client, rtype::net::message<GameServerMsgTypes> msg)
{
    // std::cout << "PLayer jump" << std::endl;
    int id = 0;
    msg >> id;
    rtype::net::message<GameServerMsgTypes> resp;
    resp.header.id = GameServerMsgTypes::OtherPlayerJump;
    resp << id;
    this->sendToOtherPlayers(client, resp);
    this->gameLogic.HandleJump(id);
}

void rtype::CustomUdpGameServer::ConnectToGameServer(std::shared_ptr<rtype::net::AConnection<GameServerMsgTypes>> client, rtype::net::message<GameServerMsgTypes> msg)
{
    std::cout << "Connect to game server" << std::endl;
    int id = 0;
    msg >> id;
    std::shared_ptr<PlayerLogic> player = this->gameLogic.getPlayer(id);
    if (player == nullptr)
        return;
    int teamId = this->gameLogic.getMyTeamId(id);
    if (teamId == 0)
        return;
    player->client = client;
}

void rtype::CustomUdpGameServer::PlayerMove(std::shared_ptr<rtype::net::AConnection<GameServerMsgTypes>> client, rtype::net::message<GameServerMsgTypes> msg)
{
    Vector3 xyz;
    msg >> xyz;
    rtype::net::message<GameServerMsgTypes> resp;
    resp.header.id = GameServerMsgTypes::PlayerMove;
    rtype::net::message<GameServerMsgTypes> resp2;
    resp2.header.id = GameServerMsgTypes::OtherPlayerMove;

    std::shared_ptr<PlayerLogic> player = this->gameLogic.getPlayer(client);
    if (player == nullptr)
        return;
    int teamId = this->gameLogic.getMyTeamId(client);
    if (teamId == 0)
        return;
    int id = this->gameLogic.getMyId(client);

    Vector3 oldPosition = player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetWorldPosition();

    player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->MoveD(xyz.x);
    player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->MoveH(-xyz.y);

    Vector3 newPosition = player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetWorldPosition();
    Vector3 diff = Vector3Subtract(newPosition, oldPosition);
    if (Vector3Equals(diff, Vector3{0,0,0}) == false) {
        player->hitbox.min = Vector3Add(player->hitbox.min, diff);
        player->hitbox.max = Vector3Add(player->hitbox.max, diff);
    }

    bool isColliding = false;
    for (int i = 0; i < this->gameLogic.mapBoundingBox.size(); i++) {
        if (CheckCollisionBoxes(player->hitbox, this->gameLogic.mapBoundingBox[i]) == true) {
            player->hitbox.min = Vector3Subtract(player->hitbox.min, diff);
            player->hitbox.max = Vector3Subtract(player->hitbox.max, diff);
            player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->SetPosition(oldPosition);
            isColliding = true;
        }
    }

    Camera camera = player->game->getComponent<ECS::Ecs3D::RayLibCameraComponent>()->camera;
    camera.position = player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->GetWorldPosition();
    camera.target = Vector3Transform(Vector3{0,0,1}, player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->GetWorldMatrix());
    player->game->getComponent<ECS::Ecs3D::RayLibCameraComponent>()->camera = camera; 

    Vector3 position = player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetWorldPosition();

    resp << position;
    resp2 << position << id << teamId;
    if (player->client != nullptr)
        player->client->Send(resp);
    this->sendToOtherPlayers(client, resp2);
}

void rtype::CustomUdpGameServer::PlayerLook(std::shared_ptr<rtype::net::AConnection<GameServerMsgTypes>> client, rtype::net::message<GameServerMsgTypes> msg)
{
    Vector3 xyz;
    msg >> xyz;
    rtype::net::message<GameServerMsgTypes> resp;
    resp.header.id = GameServerMsgTypes::PlayerLook;
    rtype::net::message<GameServerMsgTypes> resp2;
    resp2.header.id = GameServerMsgTypes::OtherPlayerLook;

    std::shared_ptr<PlayerLogic> player = this->gameLogic.getPlayer(client);
    if (player == nullptr)
        return;
    int teamId = this->gameLogic.getMyTeamId(client);
    if (teamId == 0)
        return;
    int id = this->gameLogic.getMyId(client);

    player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->RotateV(-xyz.x);
    player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->RotateH(-xyz.y);

    Camera camera = player->game->getComponent<ECS::Ecs3D::RayLibCameraComponent>()->camera;
    camera.position = player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->GetWorldPosition();
    camera.target = Vector3Transform(Vector3{0,0,1}, player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->GetWorldMatrix());
    player->game->getComponent<ECS::Ecs3D::RayLibCameraComponent>()->camera = camera;

    Quaternion basePlayerRotation = player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetOrientation();
    Quaternion headPlayerRotation = player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->GetOrientation();
    resp << basePlayerRotation << headPlayerRotation;
    resp2 << basePlayerRotation << headPlayerRotation << id << teamId;
    if (player->client != nullptr)
        player->client->Send(resp);
    this->sendToOtherPlayers(client, resp2);
}

void rtype::CustomUdpGameServer::PlayerShoot(std::shared_ptr<rtype::net::AConnection<GameServerMsgTypes>> client, rtype::net::message<GameServerMsgTypes> msg)
{
    std::shared_ptr<PlayerLogic> player = this->gameLogic.getPlayer(client);
    if (player == nullptr || player->game->getComponent<ECS::Ecs3D::Health>()->health <= 0)
        return;
    int teamId = this->gameLogic.getMyTeamId(client);
    if (teamId == 0)
        return;
    int id = this->gameLogic.getMyId(client);
    rtype::net::message<GameServerMsgTypes> whoshoot;
    whoshoot.header.id = GameServerMsgTypes::OtherPlayerShoot;
    whoshoot << id;
    whoshoot << teamId;
    this->sendToOtherPlayers(client, whoshoot);
    player->history.shots += 1;
    Camera camera = player->game->getComponent<ECS::Ecs3D::RayLibCameraComponent>()->camera;
    Vector3 position = camera.position;
    Vector3 direction = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
    Ray ray = {position, direction};
    std::map<int, std::shared_ptr<PlayerLogic>> enemyTeam = this->gameLogic.getEnnemyTeam(client);
    std::tuple<int, std::shared_ptr<PlayerLogic>, RayCollision, bool> playerHit = std::make_tuple(0, nullptr, RayCollision(), false);
    for (auto &player : enemyTeam) {
        if (player.second->game->getComponent<ECS::Ecs3D::Health>()->health <= 0)
            continue;
        Matrix baseTransform = player.second->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetWorldMatrix();
        Matrix headTransform = player.second->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->GetWorldMatrix();
        RayCollision baseCollision = GetRayCollisionMesh(ray, this->gameLogic.CharacterModel.meshes[0], baseTransform);
        RayCollision headCollision = GetRayCollisionMesh(ray, this->gameLogic.HeadCharacterModel.meshes[0], headTransform);
        if (headCollision.hit == true && baseCollision.hit == true) {
            if (headCollision.distance < baseCollision.distance) {
                if (std::get<1>(playerHit) == nullptr || std::get<2>(playerHit).distance > headCollision.distance)
                    playerHit = std::make_tuple(player.first, player.second, headCollision, true);
            } else {
                if (std::get<1>(playerHit) == nullptr || std::get<2>(playerHit).distance > baseCollision.distance)
                    playerHit = std::make_tuple(player.first, player.second, baseCollision, false);
            }
        } else if (headCollision.hit == true && baseCollision.hit == false) {
            if (std::get<1>(playerHit) == nullptr || std::get<2>(playerHit).distance > headCollision.distance)
                playerHit = std::make_tuple(player.first, player.second, headCollision, true);
        } else if (headCollision.hit == false && baseCollision.hit == true) {
            if (std::get<1>(playerHit) == nullptr || std::get<2>(playerHit).distance > baseCollision.distance)
                playerHit = std::make_tuple(player.first, player.second, baseCollision, false);
        }
    }
    if (std::get<1>(playerHit) == nullptr) {
        std::cout << "No player hit" << std::endl;
        return;
    }
    bool wallIntersect = false;
    for (auto &wall : this->gameLogic.mapBoundingBox) {
        RayCollision wallCollision = GetRayCollisionBox(ray, wall.second);
        if (wallCollision.hit == true && wallCollision.distance < std::get<2>(playerHit).distance) {
            wallIntersect = true;
            break;
        }
    }
    if (wallIntersect == true) {
        std::cout << "Wall intersect" << std::endl;
        return;
    }
    player->history.hits += 1;
    int damage = std::get<3>(playerHit) == true ? 50 : 20;
    std::get<3>(playerHit) == true ? player->history.headshots += 1 : 0;
    int oldHealth = std::get<1>(playerHit)->game->getComponent<ECS::Ecs3D::Health>()->health;
    std::get<1>(playerHit)->game->getComponent<ECS::Ecs3D::Health>()->health -= damage;
    if (oldHealth > 0 && std::get<1>(playerHit)->game->getComponent<ECS::Ecs3D::Health>()->health <= 0) {
        std::cout << "Player " << id << " killed player " << std::get<0>(playerHit) << std::endl;
        std::get<1>(playerHit)->assists.erase(std::remove(std::get<1>(playerHit)->assists.begin(), std::get<1>(playerHit)->assists.end(), id), std::get<1>(playerHit)->assists.end());
        rtype::net::message<GameServerMsgTypes> resp;
        resp.header.id = GameServerMsgTypes::UpdateScoreboard;
        resp << id << std::get<0>(playerHit);
        for (auto &playerAssists : std::get<1>(playerHit)->assists)
            resp << playerAssists;
        resp << std::get<1>(playerHit)->assists.size();
        this->MessageAllClients(resp);
        std::get<1>(playerHit)->assists.clear();
        std::get<1>(playerHit)->data.deaths += 1;
        player->data.kills += 1;
        for (auto &player : this->gameLogic.getMyTeam(client)) {
            for (auto &playerAssists : std::get<1>(playerHit)->assists)
                if (player.first == playerAssists)
                    player.second->data.assists += 1;
        }
    } else
        std::get<1>(playerHit)->assists.push_back(id);
    rtype::net::message<GameServerMsgTypes> resp;
    resp.header.id = GameServerMsgTypes::PlayerHitted;
    bool isHeadshot = std::get<3>(playerHit) == true ? true : false;
    resp << isHeadshot;
    client->Send(resp);
    rtype::net::message<GameServerMsgTypes> resp2;
    resp2.header.id = GameServerMsgTypes::SomeoneGotHitted;
    resp2 << std::get<0>(playerHit) << damage;
    std::map<int, std::shared_ptr<PlayerLogic>> myTeam = this->gameLogic.getMyTeam(client);
    this->MessageAllClients(resp2);
}
