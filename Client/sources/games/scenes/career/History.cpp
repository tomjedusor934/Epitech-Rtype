/*
** EPITECH PROJECT, 2024
** RType
** File description:
** History
*/

#include "History.hpp"

static std::string TimerToString(int timer)
{
    int seconds = timer % 60;
    int min = timer / 60;
    int hour = min / 24;
    std::string secondsStr = ((seconds < 10) ? ("0" + std::to_string(seconds)) : std::to_string(seconds));

    if (hour > 0)
        return std::to_string(hour) + ":" + std::to_string(min) + ":" + secondsStr;
    else if (min > 0)
        return std::to_string(min) + ":" + secondsStr;
    else
        return secondsStr;
}

void rtype::History::update(std::map<std::string, std::vector<int>> &historyValues)
{
    if (this->_visibleFriends < historyValues["Kill"].size()) {
        int scroll = GetMouseWheelMove();
        if (scroll != 0) {
            this->_paramFriends -= scroll;
            if (this->_paramFriends < 0)
                this->_paramFriends = 0;
            if (this->_paramFriends > historyValues["Kill"].size() - this->_visibleFriends)
                this->_paramFriends = historyValues["Kill"].size() - this->_visibleFriends;
        }

        // Barre de défilement
        int maxVisibleItems = historyValues["Kill"].size() - this->_visibleFriends;
        if (maxVisibleItems > 0) {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), this->_scrollbarRect.rec)) {
                this->_paramFriends = (int)((GetMouseY() - this->_scrollbarRect.rec.y) / this->_scrollbarRect.rec.height * maxVisibleItems);
            }
        }
    }
}

void rtype::History::draw(std::map<std::string, std::vector<int>> &historyValues)
{
    ClearBackground(RAYWHITE);
    auto kill = historyValues["Kill"];
    auto death = historyValues["Death"];
    auto assist = historyValues["Assist"];
    auto win = historyValues["Win"];
    auto headshot = historyValues["Headshot"];
    auto hits = historyValues["Hits"];
    auto shots = historyValues["Shots"];


    for (int i = 0; i < this->_visibleFriends; i++) {
        int index = this->_paramFriends + i;
        if (index < historyValues["Kill"].size()) {
            auto it = std::next(historyValues["Kill"].begin(), index);

            if (kill.size() > index && death.size() > index && assist.size() > index && win.size() > index) {
                Rectangle itemRect = {(float)this->_posX, (float)this->_posY + ITEM_HEIGHT * i, (float)ITEM_WIDTH, (float)ITEM_HEIGHT};
                std::string kda = std::to_string(kill[index]) + "/" + std::to_string(death[index]) + "/" + std::to_string(assist[index]);
                std::string statusGame = (win[index] == 0) ? "Win" : "Lose";
                std::string headShotsGame = std::to_string(headshot[index]);
                std::string hitsGame = std::to_string(hits[index]);
                std::string shotsGame = std::to_string(shots[index]);
                std::string gameModeGame = "deathmatch";
                Color color = (win[index] == 0) ? Fade(DARKGREEN, 0.6) : Fade(RED, 0.6);
                float heightAll = itemRect.y + ITEM_HEIGHT / 2 - 15;
                float widthStatusGame = (win[index] == 0) ? this->_posX + (ITEM_WIDTH / 2) - 95 : this->_posX + (ITEM_WIDTH / 2) - 120;
                float hs = this->_posX + 20 + MeasureText("Headshots", 20);
                float hi = hs + 50 + MeasureText("Hits", 20);
                float sh = hi + 50 + MeasureText("Shots", 20);

                DrawRectangleRec(itemRect, color);
                DrawRectangleLinesEx(itemRect, 1, WHITE);
                DrawText("KDA", this->_posX + 10, heightAll - 10, 20, WHITE);
                DrawText(kda.c_str(), this->_posX + 20 + MeasureText("KDA", 20), heightAll - 10, 20, WHITE);
                DrawText(statusGame.c_str(), widthStatusGame, heightAll - 20, 40, GOLD);
                DrawText("Headshots", this->_posX + 10, heightAll + 20, 20, WHITE);
                DrawText(headShotsGame.c_str(), hs, heightAll + 20, 20, WHITE);
                DrawText("Hits", hs + 40, heightAll + 20, 20, WHITE);
                DrawText(hitsGame.c_str(), hi, heightAll + 20, 20, WHITE);
                DrawText("Shots", hi + 40, heightAll + 20, 20, WHITE);
                DrawText(shotsGame.c_str(), sh, heightAll + 20, 20, WHITE);
                DrawText(gameModeGame.c_str(), this->_posX + ITEM_WIDTH - this->_map1.width - MeasureText(gameModeGame.c_str(), 20) - 20, heightAll, 20, WHITE);
                DrawTexture(this->_map1, this->_posX + ITEM_WIDTH - this->_map1.width - 10.0f, itemRect.y + 10.0f, WHITE);
            }
        }
    }

    int maxVisibleItems = historyValues["Kill"].size() - this->_visibleFriends;
    if (maxVisibleItems > 0) {
        DrawRectangle(this->_scrollbarRect.rec.x, this->_scrollbarRect.rec.y, this->_scrollbarRect.rec.width, this->_scrollbarRect.rec.height + 15, BLACK);
        float scrollbarHandleY = (float)(this->_paramFriends) / maxVisibleItems * this->_scrollbarRect.rec.height;
        DrawRectangle(this->_scrollbarRect.rec.x, this->_scrollbarRect.rec.y + scrollbarHandleY, this->_scrollbarRect.rec.width, 15, WHITE);
    }
}