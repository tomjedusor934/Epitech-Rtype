/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Loading
*/

#include "Loading.hpp"

rtype::Loading::~Loading()
{
}

void rtype::Loading::draw()
{
    BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Loading...", 10, 10, 20, WHITE);
    EndDrawing();
}

void rtype::Loading::update(float deltatime, float getTime)
{
}