/*
** EPITECH PROJECT, 2024
** RType
** File description:
** ControlsSettings
*/

#include "ControlsSettings.hpp"

void rtype::ControlsSettings::updateControls()
{
    std::map<std::string, int> inputs = {
        {"Forward", this->_containerSettings.getBinds().forward},
        {"Backward", this->_containerSettings.getBinds().backward},
        {"Left", this->_containerSettings.getBinds().left},
        {"Right", this->_containerSettings.getBinds().right},
        {"Shoot", this->_containerSettings.getBinds().shoot},
        {"Scope", this->_containerSettings.getBinds().scope},
        {"Reload", this->_containerSettings.getBinds().reload},
        {"Run", this->_containerSettings.getBinds().run},
        {"Swap", this->_containerSettings.getBinds().swap},
        {"Jump", this->_containerSettings.getBinds().jump},
        {"Pause", this->_containerSettings.getBinds().pause},
        {"Chat", this->_containerSettings.getBinds().chat},
        {"Score", this->_containerSettings.getBinds().score},
        {"Map", this->_containerSettings.getBinds().map},
        {"Mute", this->_containerSettings.getBinds().mute},
        {"Quit", this->_containerSettings.getBinds().quit}
    };

    for (int i = 0; i < this->binding.size(); i++) {
        std::string key = "NameBinding " + std::to_string(i);
        if (this->binding.find(key) != this->binding.end()) {
            std::string keyMap = this->binding.find(key)->second[0]->getComponent<rtype::ECS::Ecs3D::Text>()->text;
            if (inputs.find(keyMap) != inputs.end())
                this->binding.find(key)->second[1]->getComponent<rtype::ECS::Ecs3D::Text>()->text = rtype::ECS::Ecs3D::GetKeyName(inputs.find(keyMap)->second);
        }
    }
}

void rtype::ControlsSettings::draw()
{
    float zoomFactor = 3.7f;
    Vector2 imagePosition = { -110, -50 };

    ClearBackground(WHITE);
    BeginDrawing();
        rtype::ECS::Ecs3D::drawMenu(this->getEntity("backgroundMenu")->getComponent<rtype::ECS::Ecs3D::Texture2d>()->texture, imagePosition, zoomFactor);
        DrawFPS(1800, 0);

        for (auto& it : binding) {
            auto text0 = it.second[0]->getComponent<rtype::ECS::Ecs3D::Text>();
            auto postext0 = it.second[0]->getComponent<rtype::ECS::Ecs3D::PositionComponent2d>();
            auto text1 = it.second[1]->getComponent<rtype::ECS::Ecs3D::Text>();
            auto postext1 = it.second[1]->getComponent<rtype::ECS::Ecs3D::PositionComponent2d>();
            auto rect2 = it.second[2]->getComponent<rtype::ECS::Ecs3D::Rectangle2d>();
            DrawTextEx(_font, text0->text.c_str(), {postext0->position.x, postext0->position.y}, text0->fontSize, 0, text0->tint);
            DrawRectangle(rect2->rec.x, rect2->rec.y, rect2->rec.width, rect2->rec.height, GRAY);
            DrawTextEx(_font, text1->text.c_str(), {postext1->position.x, postext1->position.y}, text1->fontSize, 0, text1->tint);
        }

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

        DrawTextEx(_font, "VIDEO", {tmp_videosettings->button.x, tmp_videosettings->button.y}, entity_videosettings->getComponent<rtype::ECS::Ecs3D::FontSize>()->size, 0, WHITE);
        DrawTextEx(_font, "CONTROLES", {tmp_controlsettings->button.x, tmp_controlsettings->button.y}, entity_controlsettings->getComponent<rtype::ECS::Ecs3D::FontSize>()->size, 0, WHITE);
        DrawTextEx(_font, "MOUSESETTINGS", {tmp_mousesettings->button.x, tmp_mousesettings->button.y}, entity_mousesettings->getComponent<rtype::ECS::Ecs3D::FontSize>()->size, 0, WHITE);
        DrawTextEx(_font, "AUDIO", {tmp_audiosettings->button.x, tmp_audiosettings->button.y}, entity_audiosettings->getComponent<rtype::ECS::Ecs3D::FontSize>()->size, 0, WHITE);
        DrawTextEx(_font, "PROFIL", {tmp_profilsettings->button.x, tmp_profilsettings->button.y}, entity_profilsettings->getComponent<rtype::ECS::Ecs3D::FontSize>()->size, 0, WHITE);
        if (tmp_appliedbutton->isHover)
            rtype::ECS::Ecs3D::drawMenu(entity_appliedbuttonhover->getComponent<rtype::ECS::Ecs3D::Texture2d>()->texture, { tmp_appliedbuttonhover->button.x - 40, tmp_appliedbuttonhover->button.y - 10 }, 0.1f);
        else
            rtype::ECS::Ecs3D::drawMenu(entity_appliedbutton->getComponent<rtype::ECS::Ecs3D::Texture2d>()->texture, { tmp_appliedbutton->button.x - 40, tmp_appliedbutton->button.y - 10 }, 0.1f);

        if (RecVisible != -1) {
            DrawRectangle(GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 - 50, 200, 100, DARKBLUE);
            DrawTextEx(_font, "Press a key", {(float)GetScreenWidth() / 2 - 65, (float)GetScreenHeight() / 2 - 15}, 30.0f, 0, WHITE);
        }
        WantToSave(this->_isSaving, {this->getEntity("Do you want to save new settings?"), this->getEntity("Yes"), this->getEntity("No")}, _font);

    EndDrawing();
}

void rtype::ControlsSettings::update(float deltatime, float getTime)
{
    SetExitKey(0);
    if (IsKeyPressed(KEY_ESCAPE) && RecVisible == -1)
        this->_isSaving = true;


    if (this->_isSaving == false) {
        if (this->_start == false) {
            this->updateControls();
            this->_start = true;
        }
        auto tmp_videosettings = this->getEntity("VideoSettings");
        auto tmp_controlsettings = this->getEntity("ControlsSettings");
        auto tmp_mousesettings = this->getEntity("MouseSettings");
        auto tmp_audiosettings = this->getEntity("AudioSettings");
        auto tmp_profilsettings = this->getEntity("ProfilSettings");

        rtype::ECS::Ecs3D::UpdateHoverEffect({tmp_videosettings, tmp_controlsettings, tmp_audiosettings, tmp_profilsettings, tmp_mousesettings});
        rtype::ECS::Ecs3D::systemButtonUpdatePlace(this->_scene, {tmp_videosettings, tmp_controlsettings, tmp_audiosettings, tmp_profilsettings, tmp_mousesettings}, this->_lastScene);
        if (RecVisible != -1) {
            UpdateBinding(binding, RecVisible, "binds", this->_containerSettings);
        } else {
            RecVisible = WhichBindisClikcked(binding);
        }
        AppliedSettings(this->_clientToServer, this->_id, this->_containerSettings);
    } else {
        IfSaving(this->_isSaving, {this->getEntity("Do you want to save new settings?"), this->getEntity("Yes"), this->getEntity("No")}, this->_clientToServer, this->_id, this->_scene, this->_containerSettings, this->_lastScene);
        this->_start = false;
    }

    updateGifMenu(this->getEntity("backgroundMenu"));
}