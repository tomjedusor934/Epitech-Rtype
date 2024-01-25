/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Menu
*/

#include "Store.hpp"

void rtype::Store::draw()
{
    BeginDrawing();
        rtype::ECS::Ecs3D::drawMenu(this->getEntity("backgroundGame")->getComponent<rtype::ECS::Ecs3D::Texture2d>()->texture, { -110, -50 }, 2.0f);
        DrawFPS(1800, 0);

        auto font = this->getEntity("Font")->getComponent<rtype::ECS::Ecs3D::FontComponent>()->font;

        auto home = this->getEntity("Settings");
        auto career = this->getEntity("Career");
        auto collection = this->getEntity("Collection");
        auto store = this->getEntity("Store");
        auto lobby = this->getEntity("Lobby");

        auto buttonHome = home->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button;
        auto buttonCareer = career->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button;
        auto buttonCollection = collection->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button;
        auto buttonStore = store->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button;
        auto buttonLobby = lobby->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button;

        DrawTextEx(font, "LOBBY", {buttonLobby.x, buttonLobby.y}, lobby->getComponent<rtype::ECS::Ecs3D::FontSize>()->size, 0, WHITE);
        DrawTextEx(font, "SETTINGS", {buttonHome.x, buttonHome.y}, home->getComponent<rtype::ECS::Ecs3D::FontSize>()->size, 0, WHITE);
        DrawTextEx(font, "CAREER", {buttonCareer.x, buttonCareer.y}, career->getComponent<rtype::ECS::Ecs3D::FontSize>()->size, 0, WHITE);
        DrawTextEx(font, "COLLECTION", {buttonCollection.x, buttonCollection.y}, collection->getComponent<rtype::ECS::Ecs3D::FontSize>()->size, 0, WHITE);
        DrawTextEx(font, "STORE", {buttonStore.x, buttonStore.y}, store->getComponent<rtype::ECS::Ecs3D::FontSize>()->size, 0, WHITE);

        DrawRectangle(GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 - 50, 200, 100, ORANGE);
        DrawTextEx(font, "Coming Soon", {(float)GetScreenWidth() / 2 - 65, (float)GetScreenHeight() / 2 - 15}, 30.0f, 0, WHITE);

    EndDrawing();
}

void rtype::Store::update(float deltatime, float getTime)
{
    SetExitKey(0);
    if (IsKeyPressed(KEY_ESCAPE))
        this->_shouldExit = true;

    auto home = this->getEntity("Settings");
    auto career = this->getEntity("Career");
    auto collection = this->getEntity("Collection");
    auto store = this->getEntity("Store");
    auto lobby = this->getEntity("Lobby");

    rtype::ECS::Ecs3D::UpdateHoverEffect({lobby, home, career, collection, store});
    rtype::ECS::Ecs3D::systemButtonUpdatePlace(this->_scene, {lobby, home, career, collection, store});
}
