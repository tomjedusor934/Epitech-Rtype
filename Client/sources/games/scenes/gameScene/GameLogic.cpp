/*
** EPITECH PROJECT, 2024
** RType
** File description:
** GameLogic
*/

#include "GameLogic.hpp"

rtype::GameLogic::GameLogic()
{
    std::strcpy(this->playerInfo.username, "anonymous");
    this->playerInfo.kills = 0;
    this->playerInfo.deaths = 0;
    this->playerInfo.score = 0;
    this->playerInfo.team = 0;
    this->playerInfo.assists = 0;
}

rtype::GameLogic::~GameLogic()
{
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

int rtype::GameLogic::getMyTeamId(int id)
{
    if (this->teamOne.find(id) != this->teamOne.end()) {
        return 1;
    } else if (this->teamTwo.find(id) != this->teamTwo.end()) {
        return 2;
    }
    return 0;
}

void rtype::GameLogic::UpdateLightValues(Shader shader, Light light)
{
    // Send to shader light enabled state and type
    SetShaderValue(shader, light.enabledLoc, &light.enabled, SHADER_UNIFORM_INT);
    SetShaderValue(shader, light.typeLoc, &light.type, SHADER_UNIFORM_INT);

    // Send to shader light position values
    float position[3] = { light.position.x, light.position.y, light.position.z };
    SetShaderValue(shader, light.positionLoc, position, SHADER_UNIFORM_VEC3);

    // Send to shader light target position values
    float target[3] = { light.target.x, light.target.y, light.target.z };
    SetShaderValue(shader, light.targetLoc, target, SHADER_UNIFORM_VEC3);

    // Send to shader light color values
    float color[4] = { (float)light.color.r/(float)255, (float)light.color.g/(float)255, 
                       (float)light.color.b/(float)255, (float)light.color.a/(float)255 };
    SetShaderValue(shader, light.colorLoc, color, SHADER_UNIFORM_VEC4);
}

Light rtype::GameLogic::CreateLight(int type, Vector3 position, Vector3 target, Color color, Shader shader)
{
    Light light = { 0 };

    if (this->lightsCount < MAX_LIGHTS)
    {
        light.enabled = true;
        light.type = type;
        light.position = position;
        light.target = target;
        light.color = color;

        // NOTE: Lighting shader naming must be the provided ones
        light.enabledLoc = GetShaderLocation(shader, TextFormat("lights[%i].enabled", this->lightsCount));
        light.typeLoc = GetShaderLocation(shader, TextFormat("lights[%i].type", this->lightsCount));
        light.positionLoc = GetShaderLocation(shader, TextFormat("lights[%i].position", this->lightsCount));
        light.targetLoc = GetShaderLocation(shader, TextFormat("lights[%i].target", this->lightsCount));
        light.colorLoc = GetShaderLocation(shader, TextFormat("lights[%i].color", this->lightsCount));

        UpdateLightValues(shader, light);
        
        this->lightsCount++;
    }

    return light;
}

void rtype::GameLogic::addPlayer(std::string name, int id, int team, Vector3 spawnPositon, Quaternion spawnRotation, player_info_t data)
{
    ECS::Ecs3D::IEntity player(name);
    player.addComponent<ECS::Ecs3D::RayLibCameraComponent>("camera");
    player.addComponent<ECS::Ecs3D::Health>("health", 100);
    player.addComponent<ECS::Ecs3D::AmmoComponent>("ammo", 8);
    player.addComponent<ECS::Ecs3D::ReloadingComponent>("reloading", 10.0f);
    player.addComponent<ECS::Ecs3D::VelocityComponent>();
    player.addComponent<ECS::Ecs3D::GravityComponent>(-60.0f, "gravity");
    player.addComponent<ECS::Ecs3D::ListenerComponent>("listener");
    player.addComponent<ECS::Ecs3D::ObjectTransform>();
    player.getComponent<ECS::Ecs3D::ObjectTransform>()->SetPosition(spawnPositon);
    player.getComponent<ECS::Ecs3D::ObjectTransform>()->SetOrientation(spawnRotation);
    player.getComponent<ECS::Ecs3D::ObjectTransform>()->AddChild();
    player.getComponent<ECS::Ecs3D::ObjectTransform>()->MoveV(3);
    player.getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->MoveV(1.5f);
    player.getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->AddChild();
    player.getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->GetChildren()[0]->MoveV(-0.5f);
    player.getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->GetChildren()[0]->MoveH(-0.5f);

    PlayerLogic playerLogic;
    playerLogic.game = std::make_shared<ECS::Ecs3D::IEntity>(player);
    playerLogic.hitbox = GetModelBoundingBox(this->CharacterBodyModel);
    playerLogic.hitbox.min = Vector3Add(playerLogic.hitbox.min, spawnPositon);
    playerLogic.hitbox.max = Vector3Add(playerLogic.hitbox.max, spawnPositon);
    playerLogic.hitbox.min = Vector3Add(playerLogic.hitbox.min, {0, 3, 0});
    playerLogic.hitbox.max = Vector3Add(playerLogic.hitbox.max, {0, 4, 0});
    playerLogic.spawnPosition = spawnPositon;
    playerLogic.spawnRotation = spawnRotation;
    playerLogic.data = data;
    
    if (team == 1) {
        this->teamOne[id] = std::make_shared<PlayerLogic>(playerLogic);
    } else if (team == 2) {
        this->teamTwo[id] = std::make_shared<PlayerLogic>(playerLogic);
    }
}

int rtype::GameLogic::countLineFile(std::string path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier " << path << std::endl;
        return -1;
    }

    int lineCount = 0;
    std::string line;
    while (std::getline(file, line)) {
        lineCount++;
    }

    file.close();
    return lineCount;
}

void rtype::GameLogic::updateTimeBetweenShoot(float deltatime)
{
    if (this->timebetweenShoot <= 0.0) {
        this->timebetweenShoot = 0.0;
    }

    if (this->timebetweenShoot > 0.0) {
        this->timebetweenShoot -= deltatime * 10;
    }
}

void rtype::GameLogic::animationShootGun()
{
    if (this->timeAnimation <= 0) {
        this->isAnimated = false;
        this->timeAnimation = 0;
        this->gunOffset.y = 0;
    }

    if (this->isAnimated == true) {
        if (this->timeAnimation >= 0.25) {
            this->gunOffset.z -= 0.2f;
        } else if (this->timeAnimation <= 1.5 && this->timeAnimation >= 0) {
            this->gunOffset.z += 0.2;
        }
        this->timeAnimation -= 0.05f;
    }
}

void rtype::GameLogic::parseMap(std::string map)
{
    std::cout << "TA MERE LA PUTE" << std::endl;
    // std::string path = this->pathToMaps += map;
    std::string path = "./maps/1.txt";
    std::ifstream file(path);

    this->shader = LoadShader(TextFormat("Media/shaders/glsl%i/lighting.vs", GLSL_VERSION),
                               TextFormat("Media/shaders/glsl%i/lighting.fs", GLSL_VERSION));
    // Get some required shader locations
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
    int ambientLoc = GetShaderLocation(shader, "ambient");

    float ambientValues[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
    SetShaderValue(shader, ambientLoc, ambientValues, SHADER_UNIFORM_VEC4);

    Texture2D texture = LoadTexture("Media/guardian.png");

    this->GunModel = LoadModel("./Media/models/Guardian.obj");
    this->GunModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    this->GunModel.materials[0].shader = this->shader;

    this->GunModel.transform = MatrixMultiply(this->GunModel.transform, MatrixRotateY(270 * DEG2RAD));
    this->CharacterHeadModel = LoadModelFromMesh(GenMeshSphere(0.5f, 16, 16));
    this->CharacterBodyModel = LoadModelFromMesh(GenMeshCube(1, 2, 1));
    this->CharacterHeadModel.materials[0].shader = this->shader;
    this->CharacterBodyModel.materials[0].shader = this->shader;

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
            plane.addComponent<ECS::Ecs3D::ColorComponent>(name, color);

            std::cout << "texture = " << texture << std::endl;

            if (texture != "") {
                Image image = LoadImage(texture.c_str());
                plane.addComponent<ECS::Ecs3D::Texture2d>(image, name);
            }
            plane.addComponent<ECS::Ecs3D::Model3d>(name, LoadModelFromMesh(GenMeshPlane(widht, lenght, sizex, sizey)), Vector3{0, 0, 0});

            if (plane.getComponent<ECS::Ecs3D::Texture2d>() != nullptr) {
                plane.getComponent<ECS::Ecs3D::Model3d>()->model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = plane.getComponent<ECS::Ecs3D::Texture2d>()->texture;
            }

            for (int i = 0; i < plane.getComponent<ECS::Ecs3D::Model3d>()->model.meshCount; i++) {
                this->mapBoundingBox[nbBdBox] = GetMeshBoundingBox(plane.getComponent<ECS::Ecs3D::Model3d>()->model.meshes[i]);
                nbBdBox++;
            }

            this->map.insert(std::pair<int, std::shared_ptr<ECS::Ecs3D::IEntity>>(this->map.size(), std::make_shared<ECS::Ecs3D::IEntity>(plane)));


        } else if (line == "CUBE:") {
            std::getline(file, line);

            std::string name;
            float posx, posy, posz, sizex, sizey, sizez;
            std::string r, g, b, a;
            std::string texture = "";

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

            std::getline(file, line);

            if (line.compare(0, 10, "texture = ") == 0) {
                texture = line.substr(line.find("=") + 2, line.find("\n") - line.find("=") - 1);
            }

            Color color = Color{static_cast<unsigned char>(std::stoi(r)),
                static_cast<unsigned char>(std::stoi(g)),
                static_cast<unsigned char>(std::stoi(b)),
                static_cast<unsigned char>(std::stoi(a))};

            ECS::Ecs3D::IEntity cube(name);
            cube.addComponent<ECS::Ecs3D::PositionComponent3d>(Vector3{posx, posy, posz}, name);
            cube.addComponent<ECS::Ecs3D::ColorComponent>(name, color);

            if (texture != "") {
                Image image = LoadImage(texture.c_str());
                cube.addComponent<ECS::Ecs3D::Texture2d>(image, name);
            }

            cube.addComponent<ECS::Ecs3D::Model3d>(name, LoadModelFromMesh(GenMeshCube(sizex, sizey, sizez)), Vector3{posx, posy, posz});

            if (cube.getComponent<ECS::Ecs3D::Texture2d>() != nullptr) {
                cube.getComponent<ECS::Ecs3D::Model3d>()->model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = cube.getComponent<ECS::Ecs3D::Texture2d>()->texture;
            }


            for (int i = 0; i < cube.getComponent<ECS::Ecs3D::Model3d>()->model.meshCount; i++) {
                BoundingBox box = GetMeshBoundingBox(cube.getComponent<ECS::Ecs3D::Model3d>()->model.meshes[i]);
                box.min = Vector3Add(box.min, Vector3{posx, posy, posz});
                box.max = Vector3Add(box.max, Vector3{posx, posy, posz});
                this->mapBoundingBox[nbBdBox] = box;

                nbBdBox++;
            }
            this->map.insert(std::pair<int, std::shared_ptr<ECS::Ecs3D::IEntity>>(this->map.size(), std::make_shared<ECS::Ecs3D::IEntity>(cube)));

        } else if (line == "LIGHT:") {
            std::getline(file, line);

            std::string name;
            int type;
            float posx, posy, posz;
            std::string r, g, b, a;

            name = line.substr(line.find("=") + 1, line.find(",") - line.find("=") - 1);
            std::getline(file, line);
            type = std::stoi(line.substr(line.find("=") + 1, line.find(",") - line.find("=") - 1));
            std::getline(file, line);
            posx = std::stof(line.substr(line.find("=") + 1, line.find(",") - line.find("=") - 1));
            std::getline(file, line);
            posy = std::stof(line.substr(line.find("=") + 1, line.find(",") - line.find("=") - 1));
            std::getline(file, line);
            posz = std::stof(line.substr(line.find("=") + 1, line.find(",") - line.find("=") - 1));
            std::getline(file, line);
            r = line.substr(line.find("=") + 1, line.find(",") - line.find("=") - 1);
            g = line.substr(line.find(",") + 1, line.find(",", line.find(",") + 1) - line.find(",") - 1);
            b = line.substr(line.find(",", line.find(",") + 1) + 1, line.find(",", line.find(",", line.find(",") + 1) + 1) - line.find(",", line.find(",") + 1) - 1);
            a = line.substr(line.find(",", line.find(",", line.find(",") + 1) + 1) + 1, line.find("\n") - line.find(",", line.find(",", line.find(",") + 1) + 1) - 1);

            Color color = Color{static_cast<unsigned char>(std::stoi(r)),
                static_cast<unsigned char>(std::stoi(g)),
                static_cast<unsigned char>(std::stoi(b)),
                static_cast<unsigned char>(std::stoi(a))};

            Light parsedLight;
            addLightInMap(parsedLight, color, Vector3{posx, posy, posz}, type, this->nb_light);
            this->nb_light++;
        } else if (line == "MODEL:") {
            std::getline(file, line);

            std::string name;
            float posx, posy, posz;
            float sizeX, sizeY, sizeZ;
            Color color;
            std::string pathModel = "";
            std::string pathTexture = "";

            name = line.substr(line.find("=") + 1, line.find(",") - line.find("=") - 1);
            std::getline(file, line);
            posx = std::stof(line.substr(line.find("=") + 1, line.find(",") - line.find("=") - 1));
            std::getline(file, line);
            posy = std::stof(line.substr(line.find("=") + 1, line.find(",") - line.find("=") - 1));
            std::getline(file, line);
            posz = std::stof(line.substr(line.find("=") + 1, line.find(",") - line.find("=") - 1));
            std::getline(file, line);
            sizeX = std::stof(line.substr(line.find("=") + 1, line.find(",") - line.find("=") - 1));
            std::getline(file, line);
            sizeY = std::stof(line.substr(line.find("=") + 1, line.find(",") - line.find("=") - 1));
            std::getline(file, line);
            sizeZ = std::stof(line.substr(line.find("=") + 1, line.find(",") - line.find("=") - 1));
            std::getline(file, line);
            color = Color{static_cast<unsigned char>(std::stoi(line.substr(line.find("=") + 1, line.find(",") - line.find("=") - 1))),
                static_cast<unsigned char>(std::stoi(line.substr(line.find("=") + 1, line.find(",") - line.find("=") - 1))),
                static_cast<unsigned char>(std::stoi(line.substr(line.find("=") + 1, line.find(",") - line.find("=") - 1))),
                static_cast<unsigned char>(std::stoi(line.substr(line.find("=") + 1, line.find(",") - line.find("=") - 1)))};

            std::getline(file, line);
            pathModel = line.substr(line.find("=") + 2, line.find("\n") - line.find("=") - 1);
            std::getline(file, line);
            pathTexture = line.substr(line.find("=") + 2, line.find("\n") - line.find("=") - 1);

            ECS::Ecs3D::IEntity model(name);
            model.addComponent<ECS::Ecs3D::PositionComponent3d>(Vector3{posx, posy, posz}, name);
            model.addComponent<ECS::Ecs3D::ColorComponent>(name, color);

            if (pathTexture != "") {
                Image image = LoadImage(pathTexture.c_str());
                model.addComponent<ECS::Ecs3D::Texture2d>(image, name);
            }

            model.addComponent<ECS::Ecs3D::Model3d>(name, LoadModel(pathModel.c_str()), Vector3{posx, posy, posz}, Vector3{sizeX, sizeY, sizeZ});
            model.getComponent<ECS::Ecs3D::Model3d>()->model.transform = MatrixScale(sizeX, sizeY, sizeZ);
            
            if (model.getComponent<ECS::Ecs3D::Texture2d>() != nullptr) {
                model.getComponent<ECS::Ecs3D::Model3d>()->model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = model.getComponent<ECS::Ecs3D::Texture2d>()->texture;
            }

            for (int i = 0; i < model.getComponent<ECS::Ecs3D::Model3d>()->model.meshCount; i++) {
                BoundingBox box = GetModelBoundingBox(model.getComponent<ECS::Ecs3D::Model3d>()->model);
                this->mapBoundingBox[nbBdBox] = box;
                nbBdBox++;
            }
            // model.getComponent<ECS::Ecs3D::PositionComponent3d>()->position = Vector3{posx, posy / 2, posz};
            this->map.insert(std::pair<int, std::shared_ptr<ECS::Ecs3D::IEntity>>(this->map.size(), std::make_shared<ECS::Ecs3D::IEntity>(model)));
        }
    }

    for (int i = 0; i < this->map.size(); i++) {
        if (this->map[i]->getComponent<ECS::Ecs3D::Model3d>() != nullptr) {
            this->map[i]->getComponent<ECS::Ecs3D::Model3d>()->model.materials[0].shader = this->shader;
            if (this->map[i]->getComponent<ECS::Ecs3D::Texture2d>() != nullptr) {
                this->map[i]->getComponent<ECS::Ecs3D::Model3d>()->model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = this->map[i]->getComponent<ECS::Ecs3D::Texture2d>()->texture;
            }
        }
    }


    Image enemyAliveImage = LoadImage("Media/ennemi_chara.png");
    this->enemyAlive = LoadTextureFromImage(enemyAliveImage);
    UnloadImage(enemyAliveImage);

    Image enemyDeadImage = LoadImage("Media/ennemi_chara_die.png");
    this->enemyDead = LoadTextureFromImage(enemyDeadImage);
    UnloadImage(enemyDeadImage);

    Image allyAliveImage = LoadImage("Media/player_chara.png");
    this->alliesAlive = LoadTextureFromImage(allyAliveImage);
    UnloadImage(allyAliveImage);

    Image allyDeadImage = LoadImage("Media/player_chara_die.png");
    this->alliesDead = LoadTextureFromImage(allyDeadImage);
    UnloadImage(allyDeadImage);
}

void rtype::GameLogic::addLightInMap(Light light, Color color, Vector3 position, int type, int id)
{
    this->lights[id][0] = CreateLight(type, position, Vector3Zero(), color, this->shader);
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

std::map<int, std::shared_ptr<rtype::PlayerLogic>> rtype::GameLogic::getMyTeam(int id)
{
    if (this->teamOne.find(id) != this->teamOne.end()) {
        return this->teamOne;
    } else if (this->teamTwo.find(id) != this->teamTwo.end()) {
        return this->teamTwo;
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

void rtype::GameLogic::updateGravity(float deltatime, int id)
{
    std::shared_ptr<PlayerLogic> player = this->getPlayer(id);
    
    if (player == nullptr)
        return;

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

void rtype::GameLogic::jump(int id)
{
    std::shared_ptr<PlayerLogic> player = this->getPlayer(id);

    if (player == nullptr)
        return;

    if (player->game->getComponent<ECS::Ecs3D::GravityComponent>()->enableGravity == false) {
        player->game->getComponent<ECS::Ecs3D::GravityComponent>()->enableGravity = true;
        player->game->getComponent<ECS::Ecs3D::VelocityComponent>()->velocity = 25.0f;
    }
}

void rtype::GameLogic::reloadWeapon(int id)
{
    std::shared_ptr<PlayerLogic> player = this->getPlayer(id);

    if (player == nullptr)
        return;

    player->game->getComponent<ECS::Ecs3D::ReloadingComponent>()->shouldShoot = false;
    player->game->getComponent<ECS::Ecs3D::ReloadingComponent>()->time = 5.0f;
}

void rtype::GameLogic::updateReloading(float deltatime, int id)
{
    std::shared_ptr<PlayerLogic> player = this->getPlayer(id);

    if (player == nullptr)
        return;

    if (player->game->getComponent<ECS::Ecs3D::ReloadingComponent>()->time > 0.0f && player->game->getComponent<ECS::Ecs3D::ReloadingComponent>()->shouldShoot == false) {
        player->game->getComponent<ECS::Ecs3D::ReloadingComponent>()->time -= deltatime * 5;
    } else if (player->game->getComponent<ECS::Ecs3D::ReloadingComponent>()->time <= 0.0f && player->game->getComponent<ECS::Ecs3D::ReloadingComponent>()->shouldShoot == false) {
        player->game->getComponent<ECS::Ecs3D::ReloadingComponent>()->shouldShoot = true;
        player->game->getComponent<ECS::Ecs3D::ReloadingComponent>()->time = 10.0f;
        player->game->getComponent<ECS::Ecs3D::AmmoComponent>()->nb_ammo = 8;
    }
}

void rtype::GameLogic::updateListener(int id)
{
    std::shared_ptr<PlayerLogic> player = this->getPlayer(id);

    if (player == nullptr)
        return;
    
    Vector3 position = player->game->getComponent<ECS::Ecs3D::RayLibCameraComponent>()->camera.position;
    Vector3 target = player->game->getComponent<ECS::Ecs3D::RayLibCameraComponent>()->camera.target;

    // player->game->getComponent<ECS::Ecs3D::ListenerComponent>()->listener.setPosition(position.x, position.y, position.z);
    // player->game->getComponent<ECS::Ecs3D::ListenerComponent>()->listener.setDirection(target.x, target.y, target.z);
}

void rtype::GameLogic::updateMySoundPos(int id)
{
    std::shared_ptr<PlayerLogic> player = this->getPlayer(id);

    if (player == nullptr)
        return;

    Vector3 position = player->game->getComponent<ECS::Ecs3D::RayLibCameraComponent>()->camera.position;

    // // si le son footStep est joué alors on update sa position
    // if (this->footStepSound.getStatus() == sf::Sound::Playing) {
    //     this->footStepSound.setPosition(position.x, position.y, position.z);
    // }

    // if (this->shootSound.getStatus() == sf::Sound::Playing) {
    //     this->shootSound.setPosition(position.x, position.y, position.z);
    // }
}
