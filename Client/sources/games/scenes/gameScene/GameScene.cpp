/*
** EPITECH PROJECT, 2024
** RType
** File description:
** GameScene
*/

#include "GameScene.hpp"

Vector3 rtype::GameScene::HandleMovement(std::unique_ptr<rtype::GameManager> &clientToGame, float deltatime)
{
    Vector3 xyz = {0, 0, 0};
    if (IsWindowFocused() == false)
        return {0, 0, 0};
    if (IsKeyDown(this->_binds.forward)) {
        xyz.x = 10.0f * deltatime;
        if (this->_soundManager.myFootStepSound.getStatus() != sf::Sound::Playing) {
            this->_soundManager.myFootStepSound.play();
        }
    } else if (IsKeyDown(this->_binds.backward)) {
        xyz.x = -10.0f * deltatime;
        if (this->_soundManager.myFootStepSound.getStatus() != sf::Sound::Playing) {
            this->_soundManager.myFootStepSound.play();
        }
    } else {
        xyz.x = 0;
    }

    if (IsKeyDown(this->_binds.left)) {
        xyz.y = -10.0f * deltatime;
        if (this->_soundManager.myFootStepSound.getStatus() != sf::Sound::Playing) {
            this->_soundManager.myFootStepSound.play();
        }
    } else if (IsKeyDown(this->_binds.right)) {
        xyz.y = 10.0f * deltatime;
        if (this->_soundManager.myFootStepSound.getStatus() != sf::Sound::Playing) {
            this->_soundManager.myFootStepSound.play();
        }
    } else {
        xyz.y = 0;
    }

    if (IsKeyDown(this->_binds.jump)) {
        this->_gameLogic.jump(this->_id);
        this->jump = true;
    }
    return xyz;
}

Vector3 rtype::GameScene::CheckCollisionAndMove(Vector3 desiredMovement, float deltatime)
{
    std::shared_ptr<PlayerLogic> player = this->_gameLogic.getPlayer(this->_id);
    if (player == nullptr)
        return {0, 0, 0};

    Vector3 oldPosition = player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetWorldPosition();
    player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->MoveD(desiredMovement.x);
    player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->MoveH(-desiredMovement.y);
    Vector3 newPosition = player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetWorldPosition();
    Vector3 diff = Vector3Subtract(newPosition, oldPosition);
    if (Vector3Equals(diff, {0, 0, 0}) == false) {
        player->hitbox.min = Vector3Add(player->hitbox.min, diff);
        player->hitbox.max = Vector3Add(player->hitbox.max, diff);
    }
    for (int i = 0; i < this->_gameLogic.mapBoundingBox.size(); i++) {
        if (CheckCollisionBoxes(player->hitbox, this->_gameLogic.mapBoundingBox[i]) == true) {
            std::cout << "Collision" << std::endl;
            player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->SetPosition(oldPosition);
        }
    }
    std::cout << "No Collision" << std::endl;
    return desiredMovement;
}

Vector3 rtype::GameScene::HandleRotation(std::unique_ptr<rtype::GameManager> &clientToGame, float deltatime)
{
    Vector3 rota = {0, 0, 0};
    Vector2 mouseDelta = GetMouseDelta();

    if (IsWindowFocused() == false)
        return {0, 0, 0};
    if (mouseDelta.y > 0.0f) {
        rota.y = mouseDelta.y * (_mouseSettings.sensitivity / 1000.0f);
    } else if (mouseDelta.y < 0.0f) {
        rota.y = mouseDelta.y * (_mouseSettings.sensitivity / 1000.0f);
    } else {
        rota.y = 0;
    }

    if (mouseDelta.x > 0.0f) {
        rota.x = mouseDelta.x * (_mouseSettings.sensitivity / 1000.0f);
    } else if (mouseDelta.x < 0.0f) {
        rota.x = mouseDelta.x * (_mouseSettings.sensitivity / 1000.0f);
    } else {
        rota.x = 0;
    }
    return rota;
}

void rtype::GameScene::HandleShoot(std::unique_ptr<rtype::GameManager> &clientToGame)
{
    if (IsMouseButtonPressed(this->_binds.scope)) {
        this->_gameLogic.isZoom = true;
        this->_gameLogic.getPlayer(this->_id)->game->getComponent<ECS::Ecs3D::RayLibCameraComponent>()->camera.fovy = 35.0f;
        this->_gameLogic.gunOffset = {0.5, 0, 1.3};
    }

    if (IsMouseButtonReleased(this->_binds.scope)) {
        this->_gameLogic.isZoom = false;
        this->_gameLogic.getPlayer(this->_id)->game->getComponent<ECS::Ecs3D::RayLibCameraComponent>()->camera.fovy = 60.0f;
        this->_gameLogic.gunOffset = {0, 0, 1};
    }

    if (this->_gameLogic.timebetweenShoot == 0.0f && IsMouseButtonPressed(this->_binds.shoot) && this->_gameLogic.getPlayer(this->_id)->game->getComponent<ECS::Ecs3D::ReloadingComponent>()->shouldShoot == true && this->shoot == false && this->_gameLogic.getPlayer(this->_id)->game->getComponent<ECS::Ecs3D::AmmoComponent>()->nb_ammo > 0) {
        this->_gameLogic.getPlayer(this->_id)->game->getComponent<ECS::Ecs3D::AmmoComponent>()->nb_ammo -= 1;
        this->shoot = true;
        // this->_soundManager.myShootSound.setPosition(this->_gameLogic.getPlayer(this->_id)->game->getComponent<ECS::Ecs3D::RayLibCameraComponent>()->camera.position.x, this->_gameLogic.getPlayer(this->_id)->game->getComponent<ECS::Ecs3D::RayLibCameraComponent>()->camera.position.y, this->_gameLogic.getPlayer(this->_id)->game->getComponent<ECS::Ecs3D::RayLibCameraComponent>()->camera.position.z);
        this->_soundManager.myShootSound.play();

        this->_gameLogic.isAnimated = true;
        this->_gameLogic.timeAnimation = 0.5f;
        this->_gameLogic.timebetweenShoot = 3.0f;
    }

    if (IsKeyReleased(this->_binds.reload) && this->_gameLogic.getPlayer(this->_id)->game->getComponent<ECS::Ecs3D::AmmoComponent>()->nb_ammo < 8) {
        std::cout << "reload" << std::endl;
        this->_gameLogic.reloadWeapon(this->_id);
    }
}


rtype::GameScene::~GameScene()
{
}

void rtype::GameScene::drawHealthPlayer(std::shared_ptr<PlayerLogic> player, int id)
{
    std::string floatString = std::to_string(static_cast<int>(player->game->getComponent<ECS::Ecs3D::Health>()->health));
    Color myBlack = {0, 0, 0, 150};
    DrawRectangle(0, 950, 380, 80, myBlack);

    DrawText(floatString.c_str(), 30, 967, 40, WHITE);
    DrawRectangle(100, 970, player->game->getComponent<ECS::Ecs3D::Health>()->health * 2.5, 30, WHITE);
}

void rtype::GameScene::drawAmmoPlayer(std::shared_ptr<PlayerLogic> player, int id)
{
    std::string intToString = std::to_string(player->game->getComponent<ECS::Ecs3D::AmmoComponent>()->nb_ammo) + " / 8";
    Color myBlack = {0, 0, 0, 150};
    DrawRectangle(1740, 950, 200, 80, myBlack);

    DrawText(intToString.c_str(), 1770, 967, 40, WHITE);


    if (player->game->getComponent<ECS::Ecs3D::ReloadingComponent>()->shouldShoot == false) {
        DrawRectangle(GetScreenWidth() / 2 - 50, GetScreenHeight() / 2 - 50, this->_gameLogic.getPlayer(this->_id)->game->getComponent<ECS::Ecs3D::ReloadingComponent>()->time * 10, 10, WHITE);
    }
}

void rtype::GameScene::drawStatsGame()
{
    if (IsKeyDown(this->_binds.score)) {
        Color myBlack = {0, 0, 0, 100};
        Color myBlack2 = {80, 80, 80, 200};
        Color myGray = {150, 150, 150, 255};

        DrawRectangle(GetScreenWidth() / 2 - 500, GetScreenHeight() / 2 - 300, 1200, 600, myBlack);
        DrawRectangle(GetScreenWidth() / 2 - 500, GetScreenHeight() / 2 - 20, 1200, 80, myBlack2);

        DrawText("NAME", GetScreenWidth() / 2 - 400, GetScreenHeight() / 2 - 280, 30, WHITE);
        DrawText("KILL", GetScreenWidth() / 2 + 150, GetScreenHeight() / 2 - 280, 30, WHITE);
        DrawText("DEATH", GetScreenWidth() / 2 + 300, GetScreenHeight() / 2 - 280, 30, WHITE);
        DrawText("ASSIST", GetScreenWidth() / 2 + 500, GetScreenHeight() / 2 - 280, 30, WHITE);

        std::map<int, std::shared_ptr<PlayerLogic>> myTeam = this->_gameLogic.getMyTeam(this->_id);
        int j = 0;
        for (const auto& player : myTeam) {
            DrawLine(GetScreenWidth() / 2 - 500, GetScreenHeight() / 2 - 210 + j * 50, GetScreenWidth() / 2 + 700, GetScreenHeight() / 2 - 210 + j * 50, myBlack2);
            DrawText(player.second->data.username, GetScreenWidth() / 2 - 400, GetScreenHeight() / 2 - 200 + j * 50, 25, WHITE);
            DrawText(static_cast<std::string>(std::to_string(player.second->data.kills)).c_str(), GetScreenWidth() / 2 + 180, GetScreenHeight() / 2 - 200 + j * 50, 25, WHITE);
            DrawText(static_cast<std::string>(std::to_string(player.second->data.deaths)).c_str(), GetScreenWidth() / 2 + 340, GetScreenHeight() / 2 - 200 + j * 50, 25, WHITE);
            DrawText(static_cast<std::string>(std::to_string(player.second->data.assists)).c_str(), GetScreenWidth() / 2 + 550, GetScreenHeight() / 2 - 200 + j * 50, 25, WHITE);
            DrawLine(GetScreenWidth() / 2 - 500, GetScreenHeight() / 2 - 160 + j * 50, GetScreenWidth() / 2 + 700, GetScreenHeight() / 2 - 160 + j * 50, myBlack2);
            j++;
        }

        std::map<int, std::shared_ptr<PlayerLogic>> enemyTeam = this->_gameLogic.getEnnemyTeam(this->_id);
        j = 0;
        for (const auto& player : enemyTeam) {
            DrawLine(GetScreenWidth() / 2 - 500, GetScreenHeight() / 2 + 100 + j * 50, GetScreenWidth() / 2 + 700, GetScreenHeight() / 2 + 100 + j * 50, myBlack2);
            DrawText(player.second->data.username, GetScreenWidth() / 2 - 400, GetScreenHeight() / 2 + 110 + j * 50, 25, WHITE);
            DrawText(static_cast<std::string>(std::to_string(player.second->data.kills)).c_str(), GetScreenWidth() / 2 + 180, GetScreenHeight() / 2 + 110 + j * 50, 25, WHITE);
            DrawText(static_cast<std::string>(std::to_string(player.second->data.deaths)).c_str(), GetScreenWidth() / 2 + 340, GetScreenHeight() / 2 + 110 + j * 50, 25, WHITE);
            DrawText(static_cast<std::string>(std::to_string(player.second->data.assists)).c_str(), GetScreenWidth() / 2 + 550, GetScreenHeight() / 2 + 110 + j * 50, 25, WHITE);
            DrawLine(GetScreenWidth() / 2 - 500, GetScreenHeight() / 2 + 150 + j * 50, GetScreenWidth() / 2 + 700, GetScreenHeight() / 2 + 150 + j * 50, myBlack2);
            j++;
        }

        for (int i = 0; i < 9; i++) {
            Rectangle rec = {static_cast<float>(GetScreenWidth()) / 2 - 320 + i * 100, static_cast<float>(GetScreenHeight()) / 2 - 5, 10, 10};
            DrawRectangleRounded(rec, 1, 0, myGray);
        }

        for (int i = 0; i < 9; i++) {
            Rectangle rec = {static_cast<float>(GetScreenWidth()) / 2 - 320 + i * 100, static_cast<float>(GetScreenHeight()) / 2 + 35, 10, 10};
            DrawRectangleRounded(rec, 1, 0, myGray);
        }


        int myTeamId = this->_gameLogic.getMyTeamId(this->_id);
        int enemyTeamId = this->_gameLogic.getEnnemyTeamId(this->_id);

        int i = 0;
        for (auto &score : this->_gameLogic.winTeam) {
            if (score == 1 && myTeamId == 1) {
                Rectangle rec = {static_cast<float>(GetScreenWidth()) / 2 - 325 + i * 100, static_cast<float>(GetScreenHeight()) / 2 - 5, 20, 20};
                DrawRectangleRounded(rec, 1, 0, GREEN);
            } else if (score == 1 && myTeamId == 2) {
                Rectangle rec = {static_cast<float>(GetScreenWidth()) / 2 - 325 + i * 100, static_cast<float>(GetScreenHeight()) / 2 + 30, 20, 20};
                DrawRectangleRounded(rec, 1, 0, RED);
            } else if (score == 2 && enemyTeamId == 1) {
                Rectangle rec = {static_cast<float>(GetScreenWidth()) / 2 - 325 + i * 100, static_cast<float>(GetScreenHeight()) / 2 - 5, 20, 20};
                DrawRectangleRounded(rec, 1, 0, GREEN);
            } else if (score == 2 && enemyTeamId == 2) {
                Rectangle rec = {static_cast<float>(GetScreenWidth()) / 2 - 325 + i * 100, static_cast<float>(GetScreenHeight()) / 2 + 30, 20, 20};
                DrawRectangleRounded(rec, 1, 0, RED);
            }
            i++;
        }
    }
}

void rtype::GameScene::drawPlayerAth()
{
    std::map<int, std::shared_ptr<PlayerLogic>> myTeam = this->_gameLogic.getMyTeam(this->_id);
    std::map<int, std::shared_ptr<PlayerLogic>> enemyTeam = this->_gameLogic.getEnnemyTeam(this->_id);
    Color myBlack = {0, 0, 0, 100};
    DrawRectangle(GetScreenWidth() / 2 - 100, 0, 200, 80, myBlack);

    DrawText(FormatFloatToString(this->_elapsedTimer).c_str(), GetScreenWidth() / 2 - 50, 20, 40, WHITE);


    int myTeamId = this->_gameLogic.getMyTeamId(this->_id);
    int enemyTeamId = this->_gameLogic.getEnnemyTeamId(this->_id);

    if (myTeamId == 1) {
        DrawText(static_cast<std::string>(std::to_string(this->_gameLogic.scoreTeamOne)).c_str(), GetScreenWidth() / 2 - 200, 20, 40, WHITE);
        DrawText(static_cast<std::string>(std::to_string(this->_gameLogic.scoreTeamTwo)).c_str(), GetScreenWidth() / 2 + 150, 20, 40, WHITE);
    } else {
        DrawText(static_cast<std::string>(std::to_string(this->_gameLogic.scoreTeamOne)).c_str(), GetScreenWidth() / 2 + 150, 20, 40, WHITE);
        DrawText(static_cast<std::string>(std::to_string(this->_gameLogic.scoreTeamTwo)).c_str(), GetScreenWidth() / 2 - 200, 20, 40, WHITE);
    }

    int i = 0;
    for (auto &player : myTeam) {
        if (player.second->game->getComponent<ECS::Ecs3D::Health>()->health > 0) {
            DrawTextureEx(this->_gameLogic.alliesAlive, {static_cast<float>(GetScreenWidth()) / 2 - 340 - i * 110, 0}, 0, 0.2, WHITE);
        } else {
            DrawTextureEx(this->_gameLogic.alliesDead, {static_cast<float>(GetScreenWidth()) / 2 - 340 - i * 110, 0}, 0, 0.2, WHITE);
        }
        i++;
    }

    int y = 0;
    for (auto &player : enemyTeam) {
        if (player.second->game->getComponent<ECS::Ecs3D::Health>()->health > 0) {
            DrawTextureEx(this->_gameLogic.enemyAlive, {static_cast<float>(GetScreenWidth()) / 2 + 220 + y * 110, 0}, 0, 0.2, WHITE);
        } else {
            DrawTextureEx(this->_gameLogic.enemyDead, {static_cast<float>(GetScreenWidth()) / 2 + 220 + y * 110, 0}, 0, 0.2, WHITE);
        }
        y++;
    }
}

void rtype::GameScene::draw()
{
    std::shared_ptr<PlayerLogic> player = this->_gameLogic.getPlayer(this->_id);
    if (player == nullptr)
        return;
    Camera camera = player->game->getComponent<ECS::Ecs3D::RayLibCameraComponent>()->camera;

    BeginDrawing();
        ClearBackground(SKYBLUE);
        BeginMode3D(camera);
            for (int i = 0; i < this->_gameLogic.map.size(); i++) {
                DrawModel(this->_gameLogic.map[i]->getComponent<ECS::Ecs3D::Model3d>()->model, this->_gameLogic.map[i]->getComponent<ECS::Ecs3D::PositionComponent3d>()->position, this->_gameLogic.map[i]->getComponent<ECS::Ecs3D::Model3d>()->scale.x, this->_gameLogic.map[i]->getComponent<ECS::Ecs3D::ColorComponent>()->color);

                // DrawModelEx(this->_gameLogic.map[i]->getComponent<ECS::Ecs3D::Model3d>()->model, this->_gameLogic.map[i]->getComponent<ECS::Ecs3D::PositionComponent3d>()->position, this->_gameLogic.map[i]->getComponent<ECS::Ecs3D::Model3d>()->rotationAxis, this->_gameLogic.map[i]->getComponent<ECS::Ecs3D::Model3d>()->rotationAngle, this->_gameLogic.map[i]->getComponent<ECS::Ecs3D::Model3d>()->scale, this->_gameLogic.map[i]->getComponent<ECS::Ecs3D::ColorComponent>()->color);
            }

            for (int i = 0; i < this->_gameLogic.mapBoundingBox.size(); i++) {
                Vector3 position = Vector3Add(this->_gameLogic.mapBoundingBox[i].min, this->_gameLogic.mapBoundingBox[i].max);
                position = Vector3Scale(position, 0.5f);
                float widht = this->_gameLogic.mapBoundingBox[i].max.x - this->_gameLogic.mapBoundingBox[i].min.x;
                float height = this->_gameLogic.mapBoundingBox[i].max.y - this->_gameLogic.mapBoundingBox[i].min.y;
                float lenght = this->_gameLogic.mapBoundingBox[i].max.z - this->_gameLogic.mapBoundingBox[i].min.z;

                // DrawCubeWires(position, widht, height, lenght, GREEN);
            }

            std::map<int, std::shared_ptr<PlayerLogic>> myTeam = this->_gameLogic.getMyTeam(this->_id);
            std::map<int, std::shared_ptr<PlayerLogic>> enemyTeam = this->_gameLogic.getEnnemyTeam(this->_id);

            for (auto &player : myTeam) {
                if (player.second->game->getComponent<ECS::Ecs3D::Health>()->health <= 0)
            continue;
                Color headColor = player.first == this->_id ? BLUE : GREEN;
                Color weaponColor = player.first == this->_id ? MAGENTA : YELLOW;
                player.second->game->getComponent<ECS::Ecs3D::ObjectTransform>()->PushMatrix();
                DrawModel(this->_gameLogic.CharacterBodyModel, Vector3Zero(), 1.0f, WHITE);
                player.second->game->getComponent<ECS::Ecs3D::ObjectTransform>()->PopMatrix();

                player.second->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->PushMatrix();
                DrawModel(this->_gameLogic.CharacterHeadModel, Vector3Zero(), 1.0f, headColor);
                player.second->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->PopMatrix();

                player.second->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->GetChildren()[0]->PushMatrix();
                DrawModel(this->_gameLogic.GunModel, this->_gameLogic.gunOffset, 1.0f, WHITE);
                player.second->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->GetChildren()[0]->PopMatrix();
                DrawBoundingBox(player.second->hitbox, LIME);
            }

            for (auto &player : enemyTeam) {
                if (player.second->game->getComponent<ECS::Ecs3D::Health>()->health <= 0)
            continue;
        player.second->game->getComponent<ECS::Ecs3D::ObjectTransform>()->PushMatrix();
                DrawModel(this->_gameLogic.CharacterBodyModel, Vector3Zero(), 1.0f, WHITE);
                player.second->game->getComponent<ECS::Ecs3D::ObjectTransform>()->PopMatrix();

                player.second->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->PushMatrix();
                DrawModel(this->_gameLogic.CharacterHeadModel, Vector3Zero(), 1.0f, RED);
                player.second->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->PopMatrix();

                player.second->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->GetChildren()[0]->PushMatrix();
                DrawModel(this->_gameLogic.GunModel, {0, 0, 1}, 1.0f, WHITE);
                player.second->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->GetChildren()[0]->PopMatrix();
                DrawBoundingBox(player.second->hitbox, LIME);
            }


        EndMode3D();

        DrawText(TextFormat("Me Position: (%06.3f, %06.3f, %06.3f)", camera.position.x, camera.position.y, camera.position.z), 15, 15, 10, BLACK);
        DrawText(TextFormat("Me Target: (%06.3f, %06.3f, %06.3f)", camera.target.x, camera.target.y, camera.target.z), 15, 30, 10, BLACK);
        DrawText(TextFormat("Me Up: (%06.3f, %06.3f, %06.3f)", camera.up.x, camera.up.y, camera.up.z), 15, 45, 10, BLACK);

        DrawLine(GetScreenWidth() / 2 - 10, GetScreenHeight() / 2, GetScreenWidth() / 2 + 10, GetScreenHeight() / 2, ORANGE);
        DrawLine(GetScreenWidth() / 2, GetScreenHeight() / 2 - 10, GetScreenWidth() / 2, GetScreenHeight() / 2 + 10, ORANGE);
        if (this->_gameLogic.hitMarker > 0) {
            //Draw a hitmarker
            DrawLine(GetScreenWidth() / 2 - 20, GetScreenHeight() / 2 - 20, GetScreenWidth() / 2 + 20, GetScreenHeight() / 2 + 20, BLUE);
            DrawLine(GetScreenWidth() / 2 - 20, GetScreenHeight() / 2 + 20, GetScreenWidth() / 2 + 20, GetScreenHeight() / 2 - 20, BLUE);
            this->_gameLogic.hitMarker--;
        }

        DrawFPS(1800, 10);
        drawHealthPlayer(player, this->_id);
        drawAmmoPlayer(player, this->_id);
        drawStatsGame();
        drawPlayerAth();

        if (this->_pause == true) {
            auto entitySettings = this->getEntity("SettingsGame");
            auto entityQuit = this->getEntity("Quit");

            auto font = this->getEntity("Font")->getComponent<rtype::ECS::Ecs3D::FontComponent>()->font;
            auto buttonSettings = entitySettings->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button;
            auto buttonQuit = entityQuit->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button;

            DrawRectangleRounded({(float)GetScreenWidth() / 2 - 100, (float)GetScreenHeight() / 2 - 100, 200.0f, 200.0f}, 0.2f, 0, BLACK);
            DrawTextEx(font, "Settings", {buttonSettings.x, buttonSettings.y}, entitySettings->getComponent<rtype::ECS::Ecs3D::FontSize>()->size, 0, WHITE);
            DrawTextEx(font, "Quit", {buttonQuit.x, buttonQuit.y}, entityQuit->getComponent<rtype::ECS::Ecs3D::FontSize>()->size, 0, WHITE);
        }

    EndDrawing();
}

std::string rtype::GameScene::FormatFloatToString(float timer)
{
    int seconds = (int)timer % 60;
    int min = (int)timer / 60;
    int hour = min / 24;
    std::string secondsStr = ((seconds < 10) ? ("0" + std::to_string(seconds)) : std::to_string(seconds));

    if (hour > 0)
        return std::to_string(hour) + ":" + std::to_string(min) + ":" + secondsStr;
    else if (min > 0)
        return std::to_string(min) + ":" + secondsStr;
    else
        return secondsStr;
}

void rtype::GameScene::update(float deltatime, float getTime)
{
    if (IsKeyPressed(this->_binds.quit))
        this->_pause = !this->_pause;

    if (this->_pause == true) {
        if (IsCursorHidden())
            EnableCursor();
        auto entitySettings = this->getEntity("SettingsGame");
        auto entityQuit = this->getEntity("Quit");

        rtype::ECS::Ecs3D::systemButtonUpdatePlace(this->_scene, {entitySettings, entityQuit}, _Scene::GAME);
        if (IsMouseButtonPressed(this->_binds.shoot)) {
            auto button = entityQuit->getComponent<rtype::ECS::Ecs3D::ButtonMenu>();
            if (CheckCollisionPointRec(GetMousePosition(), button->button)) {
                if (button->id == "Quit")
                    this->_shouldExit = true;
            }
        }
    } else {
        if (!IsCursorHidden())
            DisableCursor();
    }

    if (this->_soundManager.menuSound.getStatus() == sf::Sound::Playing)
        this->_soundManager.menuSound.stop();

    float currentTime = GetTime();
    this->_elapsedTimer += (currentTime - getTime) * deltatime * 100000;

    float cameraPos[3] = { this->_gameLogic.getPlayer(this->_id)->game->getComponent<ECS::Ecs3D::RayLibCameraComponent>()->camera.position.x, this->_gameLogic.getPlayer(this->_id)->game->getComponent<ECS::Ecs3D::RayLibCameraComponent>()->camera.position.y, this->_gameLogic.getPlayer(this->_id)->game->getComponent<ECS::Ecs3D::RayLibCameraComponent>()->camera.position.z };
    SetShaderValue(this->_gameLogic.shader, this->_gameLogic.shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);

    std::shared_ptr<PlayerLogic> player = this->_gameLogic.getPlayer(this->_id);
    if (player == nullptr)
        return;
    Vector3 xyz = HandleMovement(this->_clientToGame, deltatime);
    Vector3 rotation = HandleRotation(this->_clientToGame, deltatime);
    this->HandleShoot(this->_clientToGame);

    Vector3 oldPos = player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetWorldPosition();

    player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->RotateV(-rotation.x);
    player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->RotateH(-rotation.y);

    player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->MoveD(xyz.x);
    player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->MoveH(-xyz.y);

    Vector3 newPos = player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetWorldPosition();
    Vector3 diff = Vector3Subtract(newPos, oldPos);
    if (Vector3Equals(diff, {0, 0, 0}) == false) {
        player->hitbox.min = Vector3Add(player->hitbox.min, diff);
        player->hitbox.max = Vector3Add(player->hitbox.max, diff);
    }
    bool isColliding = false;
    for (int i = 0; i < this->_gameLogic.mapBoundingBox.size(); i++) {
        if (CheckCollisionBoxes(player->hitbox, this->_gameLogic.mapBoundingBox[i]) == true) {
            player->hitbox.min = Vector3Subtract(player->hitbox.min, diff);
            player->hitbox.max = Vector3Subtract(player->hitbox.max, diff);
            player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->SetPosition(oldPos);
            isColliding = true;
        }
    }
    if (isColliding == false) {
        this->move = Vector3Add(this->move, xyz);
        this->rota = Vector3Add(this->rota, rotation);
    }

    Camera camera = player->game->getComponent<ECS::Ecs3D::RayLibCameraComponent>()->camera;
    camera.position = player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->GetWorldPosition();
    camera.target = Vector3Transform(Vector3{0,0,1}, player->game->getComponent<ECS::Ecs3D::ObjectTransform>()->GetChildren()[0]->GetWorldMatrix());
    player->game->getComponent<ECS::Ecs3D::RayLibCameraComponent>()->camera = camera;
    this->_gameLogic.updateReloading(deltatime, this->_id);
    this->_gameLogic.animationShootGun();
    this->_gameLogic.updateTimeBetweenShoot(deltatime);

    Vector3 playerPos = player->game->getComponent<ECS::Ecs3D::RayLibCameraComponent>()->camera.position;
    Vector3 playerDir = player->game->getComponent<ECS::Ecs3D::RayLibCameraComponent>()->camera.target;
    this->_soundManager.updateListener(playerPos, playerDir);

    this->_gameLogic.updateMySoundPos(this->_id);

    if (getTime - this->lastUpdate >= 1 / TICK_RATE) {
        this->shouldUpdate = true;
        this->lastUpdate = getTime;
    } else {
        this->shouldUpdate = false;
    }
    this->updateTick(deltatime);
}

void rtype::GameScene::updateTick(float deltatime)
{
    if (this->shouldUpdate == false)
        return;
    for (auto &player : this->_gameLogic.teamOne)
        this->_gameLogic.updateGravity(deltatime, player.first);
    for (auto &player : this->_gameLogic.teamTwo)
        this->_gameLogic.updateGravity(deltatime, player.first);
    if (Vector3Equals(this->move, {0, 0, 0}) == false) {
        rtype::net::message<GameServerMsgTypes> msg;
        msg.header.id = GameServerMsgTypes::PlayerMove;
        msg << this->move;
        this->_clientToGame->Send(msg);
        this->move = {0, 0, 0};
    }
    if (Vector3Equals(this->rota, {0, 0, 0}) == false) {
        rtype::net::message<GameServerMsgTypes> msg;
        msg.header.id = GameServerMsgTypes::PlayerLook;
        msg << this->rota;
        this->_clientToGame->Send(msg);
        this->rota = {0, 0, 0};
    }
    if (this->shoot == true) {
        rtype::net::message<GameServerMsgTypes> msg;
        msg.header.id = GameServerMsgTypes::PlayerShoot;
        this->_clientToGame->Send(msg);
        this->shoot = false;
    }
    if (this->jump == true) {
        rtype::net::message<GameServerMsgTypes> msg;
        msg.header.id = GameServerMsgTypes::PlayerJump;
        msg << this->_id;
        this->_clientToGame->Send(msg);
        this->jump = false;
    }
    this->shouldUpdate = false;
}
