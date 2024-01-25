/*
** EPITECH PROJECT, 2024
** RType
** File description:
** ProfilSettings
*/

#include "ProfilSettings.hpp"


void rtype::ProfilSettings::drawChangeName()
{
    this->getEntity("Username")->getComponent<rtype::ECS::Ecs3D::Rectangle2d>()->rec.x = (float)GetScreenWidth() / 2 - 140;
    this->getEntity("Username")->getComponent<rtype::ECS::Ecs3D::Rectangle2d>()->rec.y = (float)GetScreenHeight() / 2;
    this->_close->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button.x = (float)GetScreenWidth() / 2 + 150 - 22;
    this->_close->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button.y = (float)GetScreenHeight() / 2 - 90;
    this->_buttoneditName->getComponent<rtype::ECS::Ecs3D::Rectangle2d>()->rec.x = (float)GetScreenWidth() / 2 - 50;
    this->_buttoneditName->getComponent<rtype::ECS::Ecs3D::Rectangle2d>()->rec.y = (float)GetScreenHeight() / 2 + 40;

    auto usernameTextArea = this->getEntity("Username");
    auto textUsername = usernameTextArea->getComponent<rtype::ECS::Ecs3D::Text>();
    auto buttonUsername = usernameTextArea->getComponent<rtype::ECS::Ecs3D::Rectangle2d>()->rec;
    auto posUsername = usernameTextArea->getComponent<rtype::ECS::Ecs3D::PositionComponent2d>()->position;
    auto closePos = this->_close->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button;
    auto textAddFriend = this->_buttoneditName->getComponent<rtype::ECS::Ecs3D::Text>();

    DrawRectangleRounded({(float)GetScreenWidth() / 2 - 150, (float)GetScreenHeight() / 2 - 90, 300.0f, 180.0f}, 0.2f, 0, BLACK);
    DrawTextEx(textUsername->font, "EDIT NAME", {((float)GetScreenWidth() / 2) - (MeasureText("EDIT NAME", textUsername->fontSize) / 2) + 15, (float)GetScreenHeight() / 2 - 60}, 20, textUsername->spacing, textUsername->tint);
    DrawTextEx(textUsername->font, textUsername->text.c_str(), {((float)GetScreenWidth() / 2) - 140, (float)GetScreenHeight() / 2}, textUsername->fontSize, textUsername->spacing, textUsername->tint);
    DrawRectangleLines(GetScreenWidth() / 2 - 145, GetScreenHeight() / 2 - 5, buttonUsername.width - 25, 30, GOLD);
    DrawRectangleLinesEx(this->_buttoneditName->getComponent<rtype::ECS::Ecs3D::Rectangle2d>()->rec, 1.0f, GOLD);
    DrawTextEx(textAddFriend->font, "Edit Name", {this->_buttoneditName->getComponent<rtype::ECS::Ecs3D::Rectangle2d>()->rec.x + 10, this->_buttoneditName->getComponent<rtype::ECS::Ecs3D::Rectangle2d>()->rec.y + 10}, textAddFriend->fontSize, textAddFriend->spacing, textAddFriend->tint);
    DrawTexture(this->_close->getComponent<rtype::ECS::Ecs3D::Texture2d>()->texture, closePos.x, closePos.y, WHITE);
}

void rtype::ProfilSettings::draw()
{
    float zoomFactor = 3.7f;
    Vector2 imagePosition = { -110, -50 };
    Font font = this->getEntity("Font")->getComponent<rtype::ECS::Ecs3D::FontComponent>()->font;
    Rectangle zoneEditNameButton = Rectangle{200, 220, 200, 70};

    ClearBackground(WHITE);
    BeginDrawing();
        rtype::ECS::Ecs3D::drawMenu(this->getEntity("backgroundMenu")->getComponent<rtype::ECS::Ecs3D::Texture2d>()->texture, imagePosition, zoomFactor);
        DrawFPS(1800, 0);

        std::vector<std::string> tmp = {this->_containerSettings.getProfile().username, std::to_string(this->_containerSettings.getProfile().level)};
        drawTextInRectangle({"Username:", "Level:"}, tmp, {(float)100, (float)150, (float)1820, (float)930}, font, WHITE, 40.0f);

        auto entity_videosettings = this->getEntity("VideoSettings");
        auto entity_controlsettings = this->getEntity("ControlsSettings");
        auto entity_mousesettings = this->getEntity("MouseSettings");
        auto entity_audiosettings = this->getEntity("AudioSettings");
        auto entity_profilsettings = this->getEntity("ProfilSettings");
        auto entity_appliedbutton = this->getEntity("AppliedButton");
        auto entity_appliedbuttonhover = this->getEntity("AppliedButtonHover");

        auto tmp_videosettings = entity_videosettings->getComponent<rtype::ECS::Ecs3D::ButtonMenu>();
        auto tmp_controlsettings = entity_controlsettings->getComponent<rtype::ECS::Ecs3D::ButtonMenu>();
        auto tmp_mousesettings = entity_mousesettings->getComponent<rtype::ECS::Ecs3D::ButtonMenu>();
        auto tmp_audiosettings = entity_audiosettings->getComponent<rtype::ECS::Ecs3D::ButtonMenu>();
        auto tmp_profilsettings = entity_profilsettings->getComponent<rtype::ECS::Ecs3D::ButtonMenu>();
        auto tmp_appliedbutton = entity_appliedbutton->getComponent<rtype::ECS::Ecs3D::ButtonMenu>();
        auto tmp_appliedbuttonhover = entity_appliedbuttonhover->getComponent<rtype::ECS::Ecs3D::ButtonMenu>();

        DrawTextEx(font, "VIDEO", {tmp_videosettings->button.x, tmp_videosettings->button.y}, entity_videosettings->getComponent<rtype::ECS::Ecs3D::FontSize>()->size, 0, WHITE);
        DrawTextEx(font, "CONTROLES", {tmp_controlsettings->button.x, tmp_controlsettings->button.y}, entity_controlsettings->getComponent<rtype::ECS::Ecs3D::FontSize>()->size, 0, WHITE);
        DrawTextEx(font, "MOUSESETTINGS", {tmp_mousesettings->button.x, tmp_mousesettings->button.y}, entity_mousesettings->getComponent<rtype::ECS::Ecs3D::FontSize>()->size, 0, WHITE);
        DrawTextEx(font, "AUDIO", {tmp_audiosettings->button.x, tmp_audiosettings->button.y}, entity_audiosettings->getComponent<rtype::ECS::Ecs3D::FontSize>()->size, 0, WHITE);
        DrawTextEx(font, "PROFIL", {tmp_profilsettings->button.x, tmp_profilsettings->button.y}, entity_profilsettings->getComponent<rtype::ECS::Ecs3D::FontSize>()->size, 0, WHITE);
        DrawRectangle(zoneEditNameButton.x, zoneEditNameButton.y, zoneEditNameButton.width, zoneEditNameButton.height, BLACK);
        DrawRectangleLinesEx(zoneEditNameButton, 1.0f, GOLD);
        DrawTextEx(font, "Edit Name", {zoneEditNameButton.x + 20.0f, zoneEditNameButton.y + 20.0f}, 30.0f, 0, GOLD);
        if (tmp_appliedbutton->isHover)
            rtype::ECS::Ecs3D::drawMenu(entity_appliedbuttonhover->getComponent<rtype::ECS::Ecs3D::Texture2d>()->texture, { tmp_appliedbuttonhover->button.x - 40, tmp_appliedbuttonhover->button.y - 10 }, 0.1f);
        else
            rtype::ECS::Ecs3D::drawMenu(entity_appliedbutton->getComponent<rtype::ECS::Ecs3D::Texture2d>()->texture, { tmp_appliedbutton->button.x - 40, tmp_appliedbutton->button.y - 10 }, 0.1f);

        WantToSave(this->_isSaving, {this->getEntity("Do you want to save new settings?"), this->getEntity("Yes"), this->getEntity("No")}, font);
        if (this->_editName)
            drawChangeName();

    EndDrawing();
}

void rtype::ProfilSettings::updateChangeName()
{
    auto usernameTextArea = this->getEntity("Username");

    auto textArea = usernameTextArea->getComponent<rtype::ECS::Ecs3D::TextAreaComp>();
    auto textUsername = usernameTextArea->getComponent<rtype::ECS::Ecs3D::Text>();
    auto posUsername = usernameTextArea->getComponent<rtype::ECS::Ecs3D::PositionComponent2d>()->position;
    auto recUsername = usernameTextArea->getComponent<rtype::ECS::Ecs3D::Rectangle2d>()->rec;

    posUsername.x = (float)GetScreenWidth() / 2 - 140;
    posUsername.y = (float)GetScreenHeight() / 2;

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), {posUsername.x, posUsername.y, recUsername.width, recUsername.height})) {
        textArea->isFocus = true;
    }

    if (textArea->isFocus) {
        int key = GetKeyPressed();
        if (key != KEY_NULL) {
            if (key == KEY_BACKSPACE) {
                if (textUsername->text.size() > 0)
                    textUsername->text.pop_back();
            } else if (key == KEY_ENTER) {
                textArea->isFocus = false;
            } else {
                if (textUsername->text.size() < textArea->nbChar)
                    textUsername->text += rtype::ECS::Ecs3D::toLower(rtype::ECS::Ecs3D::GetKeyName(key, true));
            }
        }
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), this->_buttoneditName->getComponent<rtype::ECS::Ecs3D::Rectangle2d>()->rec)) {
        textArea->isFocus = false;
        std::cout << "Changer " << textUsername->text << "le nom" << std::endl;
        char username[21] = {0};
        strcpy(username, textUsername->text.c_str());
        UpdateProfileStruct("Username", username, this->_containerSettings);
        this->_editName = false;
    }

    auto close = this->_close->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button;

    if (CheckCollisionPointRec(GetMousePosition(), close))
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    else
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), close)) {
        this->_editName = false;
    }
}

void rtype::ProfilSettings::update(float deltatime, float getTime)
{
    SetExitKey(0);
    Rectangle zoneEditNameButton = Rectangle{200, 220, 200, 70};
    if (IsKeyPressed(KEY_ESCAPE))
        this->_isSaving = true;

    if (this->_isSaving == false) {
        auto tmp_videosettings = this->getEntity("VideoSettings");
        auto tmp_controlsettings = this->getEntity("ControlsSettings");
        auto tmp_mousesettings = this->getEntity("MouseSettings");
        auto tmp_audiosettings = this->getEntity("AudioSettings");
        auto tmp_profilsettings = this->getEntity("ProfilSettings");

        rtype::ECS::Ecs3D::UpdateHoverEffect({tmp_videosettings, tmp_controlsettings, tmp_audiosettings, tmp_profilsettings, tmp_mousesettings});
        rtype::ECS::Ecs3D::systemButtonUpdatePlace(this->_scene, {tmp_videosettings, tmp_controlsettings, tmp_audiosettings, tmp_profilsettings, tmp_mousesettings}, this->_lastScene);
        AppliedSettings(this->_clientToServer, this->_id, this->_containerSettings);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), zoneEditNameButton)) {
            auto textArea = this->getEntity("Username")->getComponent<rtype::ECS::Ecs3D::TextAreaComp>();
            textArea->isFocus = true;
            this->_editName = true;
        }
        if (this->_editName)
            updateChangeName();
    } else
        IfSaving(this->_isSaving, {this->getEntity("Do you want to save new settings?"), this->getEntity("Yes"), this->getEntity("No")}, this->_clientToServer, this->_id, this->_scene, this->_containerSettings, this->_lastScene);

    updateGifMenu(this->getEntity("backgroundMenu"));
}