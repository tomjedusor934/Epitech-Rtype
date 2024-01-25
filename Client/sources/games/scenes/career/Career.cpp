/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Menu
*/

#include "Career.hpp"

void rtype::Career::draw()
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

        this->_history.draw(this->_historyValues);

    EndDrawing();
}

void rtype::Career::update(float deltatime, float getTime)
{
    if (this->_start == false) {
        rtype::net::message<MainServerMsgTypes> msg;
        msg.header.id = MainServerMsgTypes::GameHistory;
        msg << this->_me;
        this->_clientToServer->Send(msg);
        this->_start = true;
        std::cout << "ask for history" << std::endl;
    }
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
    this->_history.update(this->_historyValues);
}
