/*
** EPITECH PROJECT, 2024
** RType
** File description:
** resultScene
*/


#include "resultScene.hpp"

rtype::ResultScene::~ResultScene()
{
}

void rtype::ResultScene::resetGame()
{
    this->_gameLogic.teamOne.clear();
    this->_gameLogic.teamTwo.clear();
    this->_gameLogic.lights.clear();
    this->_gameLogic.winTeam.clear();
    this->_gameLogic.scoreTeamOne = 0;
    this->_gameLogic.scoreTeamTwo = 0;
    this->_scene = _Scene::LOBBY;
}

void rtype::ResultScene::draw()
{
    BeginDrawing();
        ClearBackground(LIGHTGRAY);
        Texture2D texture = LoadTexture("./Media/back_launcher.png");
        DrawTextureEx(texture, {0, 0}, 0, 1.8, WHITE);

        if (CheckCollisionPointRec(GetMousePosition(), {10, 50, 250, 50})) {
            DrawRectangle(10, 50, 250, 50, GRAY);
            DrawText("Back to lobby", 15, 55, 30, WHITE);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                this->resetGame();
            }
        } else {
            DrawRectangle(10, 50, 250, 50, DARKGRAY);
            DrawText("Back to lobby", 15, 55, 30, WHITE);
        }


        Color myBlack = {0, 0, 0, 100};
        Color myBlack2 = {80, 80, 80, 200};
        Color myGray = {150, 150, 150, 255};

        DrawRectangle(GetScreenWidth() / 2 - 500, GetScreenHeight() / 2 - 300, 1200, 600, myBlack);
        DrawRectangle(GetScreenWidth() / 2 - 500, GetScreenHeight() / 2 - 20, 1200, 80, myBlack2);

        DrawText("NAME", GetScreenWidth() / 2 - 400, GetScreenHeight() / 2 - 280, 30, WHITE);
        DrawText("KILL", GetScreenWidth() / 2 + 150, GetScreenHeight() / 2 - 280, 30, WHITE);
        DrawText("DEATH", GetScreenWidth() / 2 + 300, GetScreenHeight() / 2 - 280, 30, WHITE);
        DrawText("ASSIST", GetScreenWidth() / 2 + 500, GetScreenHeight() / 2 - 280, 30, WHITE);

        std::map<int, std::shared_ptr<PlayerLogic>> myTeam = this->_gameLogic.getMyTeam(this->_me);
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

        std::map<int, std::shared_ptr<PlayerLogic>> enemyTeam = this->_gameLogic.getEnnemyTeam(this->_me);
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


        int myTeamId = this->_gameLogic.getMyTeamId(this->_me);
        int enemyTeamId = this->_gameLogic.getEnnemyTeamId(this->_me);

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


    EndDrawing();
}

void rtype::ResultScene::update(float deltatime, float getTime)
{
    if (this->_soundManager.menuSound.getStatus() != sf::SoundSource::Status::Playing)
        this->_soundManager.menuSound.play();
}
