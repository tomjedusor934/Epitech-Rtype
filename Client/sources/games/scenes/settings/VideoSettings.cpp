/*
** EPITECH PROJECT, 2024
** RType
** File description:
** VideoSettings
*/

#include "VideoSettings.hpp"

void rtype::VideoSettings::updateVideo()
{
    std::map<std::string, int> params = {
        {"Vsync", this->_containerSettings.getDisplaySettings().vsync},
        {"Fov", this->_containerSettings.getDisplaySettings().fov},
        {"Brightness", this->_containerSettings.getDisplaySettings().brightness},
        {"Gamma", this->_containerSettings.getDisplaySettings().gamma},
        {"Contrast", this->_containerSettings.getDisplaySettings().contrast},
        {"Saturation", this->_containerSettings.getDisplaySettings().saturation},
        {"Sharpness", this->_containerSettings.getDisplaySettings().sharpness},
    };

    for (int i = 1; i < this->_coches.size(); i += 2) {
        std::shared_ptr<rtype::ECS::Ecs3D::Text> tmp = this->_coches[i - 1]->getComponent<rtype::ECS::Ecs3D::Text>();
        if (tmp) {
            std::string keyCoche = tmp->text;
            if (params.find(keyCoche) != params.end()) {
                this->_cochesBool[i] = params[keyCoche];
            }
        }
    }

    for (int i = 1; i < this->_jaugeBars.size(); i += 2) {
        std::shared_ptr<rtype::ECS::Ecs3D::Text> tmp = this->_jaugeBars[i - 1]->getComponent<rtype::ECS::Ecs3D::Text>();
        if (tmp) {
            std::string keyJaugeBar = tmp->text;
            if (params.find(keyJaugeBar) != params.end()) {
                this->_jaugeBarsLevel[i] = (float)params[keyJaugeBar] / 100.0f;
            }
        }
    }

    for (int i = 0; i + 3 < this->_increments.size(); i += 4) {
        int index = (i + 1 / 4) + 1;
        auto text = this->_increments[i + 1]->getComponent<rtype::ECS::Ecs3D::Text>();
        if (text->text == "Frame Per Second (FPS)") {
            std::string tmpValue = this->_containerSettings.getDisplaySettings().framerate;
            if (tmpValue == "unlimited") {
                this->_incrementsValue[index] = 241;
                SetTargetFPS(0);
            } else {
                this->_incrementsValue[index] = std::stoi(tmpValue);
                SetTargetFPS(std::stoi(tmpValue));
            }
        }
    }

    for (int i = 0; i + 3 < this->_selects.size(); i += 4) {
        auto nameFirstChoice = this->_selects[i]->getComponent<rtype::ECS::Ecs3D::Text>();
        auto nameSecondChoice = this->_selects[i + 1]->getComponent<rtype::ECS::Ecs3D::Text>();
        auto nameThirdChoice = this->_selects[i + 2]->getComponent<rtype::ECS::Ecs3D::Text>();
        auto nameParam = this->_selects[i + 3]->getComponent<rtype::ECS::Ecs3D::Text>();
        if (nameParam->text == "Resolution") {
            std::string tmpValue = this->_containerSettings.getDisplaySettings().resolution;
            if (tmpValue == "1920x1080") {
                this->_selectsTint[i + 1] = BLUE;
                if (IsWindowFullscreen())
                    ToggleFullscreen();
                SetWindowSize(1920, 1080);
            } else if (tmpValue == "1280x720") {
                this->_selectsTint[i] = BLUE;
                if (IsWindowFullscreen())
                    ToggleFullscreen();
                SetWindowSize(1280, 720);
            } else if (tmpValue == "fullscreen") {
                this->_selectsTint[i + 2] = BLUE;
                if (!IsWindowFullscreen())
                    ToggleFullscreen();
            }
        }
        if (nameParam->text == "Colorblind") {
            std::string tmpValue = this->_containerSettings.getDisplaySettings().colorblind;
            if (tmpValue == "none") {
                this->_selectsTint[i] = BLUE;
            } else if (tmpValue == "protanopia") {
                this->_selectsTint[i + 1] = BLUE;
            } else if (tmpValue == "deuteranopia") {
                this->_selectsTint[i + 2] = BLUE;
            }
        }
    }
}

void rtype::VideoSettings::draw()
{
    float zoomFactor = 3.7f;
    Vector2 imagePosition = { -110, -50 };
    Font font = this->getEntity("Font")->getComponent<rtype::ECS::Ecs3D::FontComponent>()->font;

    ClearBackground(WHITE);
    BeginDrawing();
        rtype::ECS::Ecs3D::drawMenu(this->getEntity("backgroundMenu")->getComponent<rtype::ECS::Ecs3D::Texture2d>()->texture, imagePosition, zoomFactor);
        DrawFPS(1800, 0);

        DrawCoche(_coches, _cochesBool, font);
        DrawJaugeBar(_jaugeBars, _jaugeBarsLevel, font);
        DrawIncrement(_increments, _incrementsValue, font);
        DrawSelect(_selects, _selectsTint, font);

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
        if (tmp_appliedbutton->isHover)
            rtype::ECS::Ecs3D::drawMenu(entity_appliedbuttonhover->getComponent<rtype::ECS::Ecs3D::Texture2d>()->texture, { tmp_appliedbuttonhover->button.x - 40, tmp_appliedbuttonhover->button.y - 10 }, 0.1f);
        else
            rtype::ECS::Ecs3D::drawMenu(entity_appliedbutton->getComponent<rtype::ECS::Ecs3D::Texture2d>()->texture, { tmp_appliedbutton->button.x - 40, tmp_appliedbutton->button.y - 10 }, 0.1f);

        WantToSave(this->_isSaving, {this->getEntity("Do you want to save new settings?"), this->getEntity("Yes"), this->getEntity("No")}, font);

    EndDrawing();
}

void rtype::VideoSettings::update(float deltatime, float getTime)
{
    SetExitKey(0);
    if (IsKeyPressed(KEY_ESCAPE))
        this->_isSaving = true;


    if (this->_isSaving == false) {
        if (this->_start == false) {
            this->updateVideo();
            this->_start = true;
        }
        auto tmp_videosettings = this->getEntity("VideoSettings");
        auto tmp_controlsettings = this->getEntity("ControlsSettings");
        auto tmp_mousesettings = this->getEntity("MouseSettings");
        auto tmp_audiosettings = this->getEntity("AudioSettings");
        auto tmp_profilsettings = this->getEntity("ProfilSettings");

        rtype::ECS::Ecs3D::UpdateHoverEffect({tmp_videosettings, tmp_controlsettings, tmp_audiosettings, tmp_profilsettings, tmp_mousesettings});
        rtype::ECS::Ecs3D::systemButtonUpdatePlace(this->_scene, {tmp_videosettings, tmp_controlsettings, tmp_audiosettings, tmp_profilsettings, tmp_mousesettings}, this->_lastScene);
        UpdateCoche(_coches, _cochesBool, "videosettings", this->_containerSettings);
        UpdateJaugeBar(_jaugeBars, _jaugeBarsLevel, "videosettings", this->_containerSettings, this->soundManager);
        UpdateIncrement(_increments, _incrementsValue, "videosettings", this->_containerSettings);
        UpdateSelect(_selects, _selectsTint, "videosettings", this->_containerSettings);
        AppliedSettings(this->_clientToServer, this->_id, this->_containerSettings);
    } else {
        IfSaving(this->_isSaving, {this->getEntity("Do you want to save new settings?"), this->getEntity("Yes"), this->getEntity("No")}, this->_clientToServer, this->_id, this->_scene, this->_containerSettings, this->_lastScene);
        this->_start = false;
    }

    updateGifMenu(this->getEntity("backgroundMenu"));
}
