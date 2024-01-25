/*
** EPITECH PROJECT, 2024
** RType
** File description:
** GameLogic
*/

#include "GameLogic.hpp"

rtype::GameLogic::GameLogic()
{
    this->HeadCharacterModel = LoadModelFromMesh(GenMeshSphere(0.5f, 16, 16));
    this->CharacterModel = LoadModelFromMesh(GenMeshCube(1, 2, 1));
}

rtype::GameLogic::~GameLogic()
{
}

void rtype::GameLogic::addPlayer(std::string name, int id, int team, player_info_t data, Vector3 spawnPosition, Quaternion spawnRotation)
{
    ECS::Ecs3D::IEntity player(name);
    player.addComponent<ECS::Ecs3D::Health>("health", 100);
    player.addComponent<ECS::Ecs3D::VelocityComponent>();
    player.addComponent<ECS::Ecs3D::GravityComponent>(-60.0f, "gravity");
    player.addComponent<ECS::Ecs3D::ObjectTransform>();
    player.getComponent<ECS::Ecs3D::ObjectTransform>()->SetPosition(spawnPosition);
    player.getComponent<ECS::Ecs3D::ObjectTransform>()->SetOrientation(spawnRotation);
    player.getComponent<ECS::Ecs3D::ObjectTransform>()->AddChild();
    player.getComponent<ECS::Ecs3D::ObjectTransform>()->MoveV(3);
    player.getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->MoveV(1.5f);
    player.getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->AddChild();
    player.getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->GetChildren()[0]->MoveV(-0.5f);
    player.getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->GetChildren()[0]->MoveH(-0.5f);
    player.addComponent<ECS::Ecs3D::RayLibCameraComponent>("camera");

    PlayerLogic playerLogic;
    playerLogic.spawnPosition = spawnPosition;
    playerLogic.spawnRotation = spawnRotation;
    playerLogic.data = data;
    playerLogic.client = nullptr;
    playerLogic.game = std::make_shared<ECS::Ecs3D::IEntity>(player);
    playerLogic.hitbox = GetModelBoundingBox(this->CharacterModel);
    playerLogic.hitbox.min = Vector3Add(playerLogic.hitbox.min, spawnPosition);
    playerLogic.hitbox.max = Vector3Add(playerLogic.hitbox.max, spawnPosition);
    playerLogic.hitbox.min = Vector3Add(playerLogic.hitbox.min, {0, 3, 0});
    playerLogic.hitbox.max = Vector3Add(playerLogic.hitbox.max, {0, 4, 0});
    playerLogic.history.user_id = id;
    playerLogic.history.team = team;
    if (team == 1) {
        this->teamOne[id] = std::make_shared<PlayerLogic>(playerLogic);
    } else if (team == 2) {
        this->teamTwo[id] = std::make_shared<PlayerLogic>(playerLogic);
    }
}

std::shared_ptr<rtype::PlayerLogic> rtype::GameLogic::getPlayer(int id)
{
    if (this->teamOne.find(id) != this->teamOne.end()) {
        return this->teamOne[id];
    } else if (this->teamTwo.find(id) != this->teamTwo.end()) {
        return this->teamTwo[id];
    }
    return nullptr;
}

std::shared_ptr<rtype::PlayerLogic> rtype::GameLogic::getPlayer(std::shared_ptr<rtype::net::AConnection<GameServerMsgTypes>> client)
{
    for (const auto &player : this->teamOne) {
        if (player.second->client == client) {
            return player.second;
        }
    }
    for (const auto &player : this->teamTwo) {
        if (player.second->client == client) {
            return player.second;
        }
    }
    return nullptr;
}

std::map<int, std::shared_ptr<rtype::PlayerLogic>> rtype::GameLogic::getMyTeam(int id)
{
    if (this->teamOne.find(id) != this->teamOne.end()) {
        return this->teamOne;
    } else if (this->teamTwo.find(id) != this->teamTwo.end()) {
        return this->teamTwo;
    }
    return std::map<int, std::shared_ptr<PlayerLogic>>();
}

std::map<int, std::shared_ptr<rtype::PlayerLogic>> rtype::GameLogic::getMyTeam(std::shared_ptr<rtype::net::AConnection<GameServerMsgTypes>> client)
{
    for (const auto &player : this->teamOne) {
        if (player.second->client == client) {
            return this->teamOne;
        }
    }
    for (const auto &player : this->teamTwo) {
        if (player.second->client == client) {
            return this->teamTwo;
        }
    }
    return std::map<int, std::shared_ptr<PlayerLogic>>();
}

std::map<int, std::shared_ptr<rtype::PlayerLogic>> rtype::GameLogic::getEnnemyTeam(int id)
{
    if (this->teamOne.find(id) != this->teamOne.end()) {
        return this->teamTwo;
    } else if (this->teamTwo.find(id) != this->teamTwo.end()) {
        return this->teamOne;
    }
    return std::map<int, std::shared_ptr<PlayerLogic>>();
}

std::map<int, std::shared_ptr<rtype::PlayerLogic>> rtype::GameLogic::getEnnemyTeam(std::shared_ptr<rtype::net::AConnection<GameServerMsgTypes>> client)
{
    for (const auto &player : this->teamOne) {
        if (player.second->client == client) {
            return this->teamTwo;
        }
    }
    for (const auto &player : this->teamTwo) {
        if (player.second->client == client) {
            return this->teamOne;
        }
    }
    return std::map<int, std::shared_ptr<PlayerLogic>>();
}

int rtype::GameLogic::getMyTeamId(int id)
{
    if (this->teamOne.find(id) != this->teamOne.end()) {
        return 1;
    } else if (this->teamTwo.find(id) != this->teamTwo.end()) {
        return 2;
    }
    return 0;
}

int rtype::GameLogic::getMyTeamId(std::shared_ptr<rtype::net::AConnection<GameServerMsgTypes>> client)
{
    for (const auto &player : this->teamOne) {
        if (player.second->client == client) {
            return 1;
        }
    }
    for (const auto &player : this->teamTwo) {
        if (player.second->client == client) {
            return 2;
        }
    }
    return 0;
}

int rtype::GameLogic::getEnnemyTeamId(int id)
{
    if (this->teamOne.find(id) != this->teamOne.end()) {
        return 2;
    } else if (this->teamTwo.find(id) != this->teamTwo.end()) {
        return 1;
    }
    return 0;
}

int rtype::GameLogic::getEnnemyTeamId(std::shared_ptr<rtype::net::AConnection<GameServerMsgTypes>> client)
{
    for (const auto &player : this->teamOne) {
        if (player.second->client == client) {
            return 2;
        }
    }
    for (const auto &player : this->teamTwo) {
        if (player.second->client == client) {
            return 1;
        }
    }
    return 0;
}

int rtype::GameLogic::getMyId(std::shared_ptr<rtype::net::AConnection<GameServerMsgTypes>> client)
{
    for (const auto &player : this->teamOne) {
        if (player.second->client == client) {
            return player.first;
        }
    }
    for (const auto &player : this->teamTwo) {
        if (player.second->client == client) {
            return player.first;
        }
    }
    return 0;
}

bool rtype::GameLogic::isTeamOneReady()
{
    for (const auto &player : this->teamOne) {
        if (player.second->client == nullptr) {
            return false;
        }
    }
    return true;
}

bool rtype::GameLogic::isTeamTwoReady()
{
    for (const auto &player : this->teamTwo) {
        if (player.second->client == nullptr) {
            return false;
        }
    }
    return true;
}

void rtype::GameLogic::parseMap(std::string map)
{
    // std::string path = this->pathToMaps += map;
    std::string path = "./maps/1.txt";

    std::ifstream file(path);

    std::string line;
    int nbBdBox = 0;

    while (std::getline(file, line)) {
        if (line == "PLANE:") {
            std::getline(file, line);

            std::string name;
            float widht, lenght, sizex, sizey;
            Color color;
            std::string texture = "";

            name = line.substr(line.find("=") + 1, line.find(",") - line.find("=") - 1);
            std::getline(file, line);
            widht = std::stof(line.substr(line.find("=") + 1, line.find(",") - line.find("=") - 1));
            std::getline(file, line);
            lenght = std::stof(line.substr(line.find("=") + 1, line.find(",") - line.find("=") - 1));
            std::getline(file, line);
            sizex = std::stof(line.substr(line.find("=") + 1, line.find(",") - line.find("=") - 1));
            std::getline(file, line);
            sizey = std::stof(line.substr(line.find("=") + 1, line.find(",") - line.find("=") - 1));
            std::getline(file, line);
            color = Color{static_cast<unsigned char>(std::stoi(line.substr(line.find("=") + 1, line.find(",") - line.find("=") - 1))),
                static_cast<unsigned char>(std::stoi(line.substr(line.find("=") + 1, line.find(",") - line.find("=") - 1))),
                static_cast<unsigned char>(std::stoi(line.substr(line.find("=") + 1, line.find(",") - line.find("=") - 1))),
                static_cast<unsigned char>(std::stoi(line.substr(line.find("=") + 1, line.find(",") - line.find("=") - 1)))};
            std::getline(file, line);

            if (line.compare(0, 10, "texture = ") == 0) {
                texture = line.substr(line.find("=") + 2, line.find("\n") - line.find("=") - 1);
            }

            ECS::Ecs3D::IEntity plane(name);
            plane.addComponent<ECS::Ecs3D::PositionComponent3d>(Vector3{0, 0, 0}, name);
            plane.addComponent<ECS::Ecs3D::Model3d>(name, LoadModelFromMesh(GenMeshPlane(widht, lenght, sizex, sizey)), Vector3{0, 0, 0});

            this->mapBoundingBox[nbBdBox] = GetModelBoundingBox(plane.getComponent<ECS::Ecs3D::Model3d>()->model); 
            this->mapBoundingBox[nbBdBox].min = Vector3Add(this->mapBoundingBox[nbBdBox].min, Vector3{0, 0, 0});
            this->mapBoundingBox[nbBdBox].max = Vector3Add(this->mapBoundingBox[nbBdBox].max, Vector3{0, 0, 0});
            nbBdBox++;

            this->map.insert(std::pair<int, std::shared_ptr<ECS::Ecs3D::IEntity>>(this->map.size(), std::make_shared<ECS::Ecs3D::IEntity>(plane)));
        } else if (line == "CUBE:") {
            std::getline(file, line);

            std::string name;
            float posx, posy, posz, sizex, sizey, sizez;
            std::string r, g, b, a;

            name = line.substr(line.find("=") + 1, line.find(",") - line.find("=") - 1);
            std::getline(file, line);
            posx = std::stof(line.substr(line.find("=") + 1, line.find(",") - line.find("=") - 1));
            std::getline(file, line);
            posy = std::stof(line.substr(line.find("=") + 1, line.find(",") - line.find("=") - 1));
            std::getline(file, line);
            posz = std::stof(line.substr(line.find("=") + 1, line.find(",") - line.find("=") - 1));
            std::getline(file, line);
            sizex = std::stof(line.substr(line.find("=") + 1, line.find(",") - line.find("=") - 1));
            std::getline(file, line);
            sizey = std::stof(line.substr(line.find("=") + 1, line.find(",") - line.find("=") - 1));
            std::getline(file, line);
            sizez = std::stof(line.substr(line.find("=") + 1, line.find(",") - line.find("=") - 1));
            std::getline(file, line);
            r = line.substr(line.find("=") + 1, line.find(",") - line.find("=") - 1);
            g = line.substr(line.find(",") + 1, line.find(",", line.find(",") + 1) - line.find(",") - 1);
            b = line.substr(line.find(",", line.find(",") + 1) + 1, line.find(",", line.find(",", line.find(",") + 1) + 1) - line.find(",", line.find(",") + 1) - 1);
            a = line.substr(line.find(",", line.find(",", line.find(",") + 1) + 1) + 1, line.find("\n") - line.find(",", line.find(",", line.find(",") + 1) + 1) - 1);

            Color color = Color{static_cast<unsigned char>(std::stoi(r)),
                static_cast<unsigned char>(std::stoi(g)),
                static_cast<unsigned char>(std::stoi(b)),
                static_cast<unsigned char>(std::stoi(a))};

            ECS::Ecs3D::IEntity cube(name);
            cube.addComponent<ECS::Ecs3D::Model3d>(name, LoadModelFromMesh(GenMeshCube(sizex, sizey, sizez)), Vector3{posx, posy, posz});
            this->mapBoundingBox[nbBdBox] = GetModelBoundingBox(cube.getComponent<ECS::Ecs3D::Model3d>()->model);
            this->mapBoundingBox[nbBdBox].min = Vector3Add(this->mapBoundingBox[nbBdBox].min, Vector3{posx, posy, posz});
            this->mapBoundingBox[nbBdBox].max = Vector3Add(this->mapBoundingBox[nbBdBox].max, Vector3{posx, posy, posz});
            nbBdBox++;
            this->map.insert(std::pair<int, std::shared_ptr<ECS::Ecs3D::IEntity>>(this->map.size(), std::make_shared<ECS::Ecs3D::IEntity>(cube)));
        }
    }
}

void rtype::GameLogic::HandleJump(int id)
{
    std::shared_ptr<PlayerLogic> player = this->getPlayer(id);

    if (player == nullptr)
        return;

    if (player->game->getComponent<ECS::Ecs3D::GravityComponent>()->enableGravity == false) {
        player->game->getComponent<ECS::Ecs3D::GravityComponent>()->enableGravity = true;
        player->game->getComponent<ECS::Ecs3D::VelocityComponent>()->velocity = 25.0f;
    }
}

void rtype::GameLogic::updateGravity(float deltatime, std::shared_ptr<PlayerLogic> player)
{
    Vector3 xyz = {0, 0, 0};

    xyz.z += player->game->getComponent<ECS::Ecs3D::VelocityComponent>()->velocity * deltatime;
    Vector3 oldPos = player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetWorldPosition();
    player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->MoveV(xyz.z);
    Vector3 newPos = player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetWorldPosition();
    Vector3 diff = Vector3Subtract(newPos, oldPos);
    player->hitbox.min = Vector3Add(player->hitbox.min, diff);
    player->hitbox.max = Vector3Add(player->hitbox.max, diff);

    for (int i = 0; i < this->mapBoundingBox.size(); i++) {
        if (CheckCollisionBoxes(player->hitbox, this->mapBoundingBox[i])) {
            if (oldPos.y < 1.0f) {
                oldPos.y += 0.2f;
                diff = Vector3Subtract(newPos, oldPos);
            }
            player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->SetPosition(oldPos);
            player->hitbox.min = Vector3Subtract(player->hitbox.min, diff);
            player->hitbox.max = Vector3Subtract(player->hitbox.max, diff);
            player->game->getComponent<ECS::Ecs3D::VelocityComponent>()->velocity = 0.0f;
            player->game->getComponent<ECS::Ecs3D::GravityComponent>()->enableGravity = false;
            return;
        }
    }

    Camera camera = player->game->getComponent<ECS::Ecs3D::RayLibCameraComponent>()->camera;
    camera.position = player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->GetWorldPosition();
    camera.target = Vector3Transform(Vector3{0,0,1}, player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->GetWorldMatrix());
    player->game->getComponent<ECS::Ecs3D::GravityComponent>()->enableGravity = true;
    player->game->getComponent<ECS::Ecs3D::RayLibCameraComponent>()->camera = camera;
    player->game->getComponent<ECS::Ecs3D::VelocityComponent>()->velocity += player->game->getComponent<ECS::Ecs3D::GravityComponent>()->gravity * deltatime;
}

bool rtype::GameLogic::teamOneAlive()
{
    for (const auto &player : this->teamOne) {
        if (player.second->game->getComponent<ECS::Ecs3D::Health>()->health > 0) {
            return true;
        }
    }
    return false;
}

bool rtype::GameLogic::teamTwoAlive()
{
    for (const auto &player : this->teamTwo) {
        if (player.second->game->getComponent<ECS::Ecs3D::Health>()->health > 0) {
            return true;
        }
    }
    return false;
}

void rtype::GameLogic::checkRound()
{
    if (this->teamOneAlive() == false) {
        this->scoreTeamTwo++;
        this->resetRound(2);
    } else if (this->teamTwoAlive() == false) {
        this->scoreTeamOne++;
        this->resetRound(1);
    }
}

void rtype::GameLogic::resetRound(int winningTeam)
{
    for (const auto& team : {this->teamOne, this->teamTwo}) {
        for (const auto& player : team) {
            auto& game = player.second->game;
            player.second->assists.clear();
            game->getComponent<ECS::Ecs3D::Health>()->health = 100;
            game->getComponent<ECS::Ecs3D::ObjectTransform>()->SetPosition(player.second->spawnPosition);
            game->getComponent<ECS::Ecs3D::ObjectTransform>()->SetOrientation(player.second->spawnRotation);
            game->getComponent<ECS::Ecs3D::ObjectTransform>()->MoveV(3);
            game->getComponent<ECS::Ecs3D::VelocityComponent>()->velocity = 0.0f;
            game->getComponent<ECS::Ecs3D::GravityComponent>()->enableGravity = true;
            player.second->hitbox = GetModelBoundingBox(this->CharacterModel);
            player.second->hitbox.min = Vector3Add(player.second->hitbox.min, player.second->spawnPosition);
            player.second->hitbox.max = Vector3Add(player.second->hitbox.max, player.second->spawnPosition);
            player.second->hitbox.min = Vector3Add(player.second->hitbox.min, {0, 3, 0});
            player.second->hitbox.max = Vector3Add(player.second->hitbox.max, {0, 4, 0});
            rtype::net::message<GameServerMsgTypes> resp;
            resp.header.id = GameServerMsgTypes::NextRound;
            resp << winningTeam;
            player.second->client->Send(resp);
        }
    }
}

bool rtype::GameLogic::endGame()
{
    if (this->scoreTeamOne != ROUND_TO_WIN && this->scoreTeamTwo != ROUND_TO_WIN) {
        return false;
    }
    for (const auto& team : {this->teamOne, this->teamTwo}) {
        for (const auto& player : team) {
            player.second->history.kills = player.second->data.kills;
            player.second->history.deaths = player.second->data.deaths;
            player.second->history.assists = player.second->data.assists;
            // player.second->history.accuracy = player.second->history.hits / player.second->history.shots * 100;
            player.second->history.accuracy = 0;
        }
    }
    this->gameHistory.winner = this->scoreTeamOne == ROUND_TO_WIN ? 1 : 2;
    this->scoreTeamOne = 0;
    this->scoreTeamTwo = 0;
    this->mapToLoad = -1;
    for (const auto& map : this->map)
        UnloadModel(map.second->getComponent<ECS::Ecs3D::Model3d>()->model);
    this->map.clear();
    this->mapBoundingBox.clear();
    return true;
}
