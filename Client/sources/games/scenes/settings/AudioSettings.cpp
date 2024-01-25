/*
** EPITECH PROJECT, 2024
** RType
** File description:
** AudioSettings
*/

#include "AudioSettings.hpp"

void rtype::AudioSettings::updateAudio()
{
    std::map<std::string, int> params = {
        {"MasterVolume", this->_containerSettings.getAudioSettings().master},
        {"Music", this->_containerSettings.getAudioSettings().music},
        {"Effects", this->_containerSettings.getAudioSettings().effects},
        {"Voice", this->_containerSettings.getAudioSettings().voice},
        {"Voice chat", this->_containerSettings.getAudioSettings().voice_chat},
        {"Voice chat push to talk", this->_containerSettings.getAudioSettings().voice_chat_push_to_talk}
    };

    for (int i = 1; i < this->_coches.size(); i += 2) {
        std::shared_ptr<rtype::ECS::Ecs3D::Text> tmp = this->_coches[i - 1]->getComponent<rtype::ECS::Ecs3D::Text>();
        if (tmp) {
            std::string keyCoche = tmp->text;
            if (params.find(keyCoche) != params.end()) {
                this->_cochesBool[i] = params[keyCoche];
                this->_soundManager.updateVolume(keyCoche, (float)params[keyCoche]);
            }
        }
    }

    for (int i = 1; i < this->_jaugeBars.size(); i += 2) {
        std::shared_ptr<rtype::ECS::Ecs3D::Text> tmp = this->_jaugeBars[i - 1]->getComponent<rtype::ECS::Ecs3D::Text>();
        if (tmp) {
            std::string keyJaugeBar = tmp->text;
            if (params.find(keyJaugeBar) != params.end()) {
                this->_jaugeBarsLevel[i] = (float)params[keyJaugeBar] / 100.0f;
                this->_soundManager.updateVolume(keyJaugeBar, (float)params[keyJaugeBar]);
            }
        }
    }
}

void rtype::AudioSettings::draw()
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

void rtype::AudioSettings::update(float deltatime, float getTime)
{
    SetExitKey(0);
    if (IsKeyPressed(KEY_ESCAPE))
        this->_isSaving = true;


    if (this->_isSaving == false) {
        if (this->_start == false) {
            this->updateAudio();
            this->_start = true;
        }
        auto tmp_videosettings = this->getEntity("VideoSettings");
        auto tmp_controlsettings = this->getEntity("ControlsSettings");
        auto tmp_mousesettings = this->getEntity("MouseSettings");
        auto tmp_audiosettings = this->getEntity("AudioSettings");
        auto tmp_profilsettings = this->getEntity("ProfilSettings");

        rtype::ECS::Ecs3D::UpdateHoverEffect({tmp_videosettings, tmp_controlsettings, tmp_audiosettings, tmp_profilsettings, tmp_mousesettings});
        rtype::ECS::Ecs3D::systemButtonUpdatePlace(this->_scene, {tmp_videosettings, tmp_controlsettings, tmp_audiosettings, tmp_profilsettings, tmp_mousesettings}, this->_lastScene);
        UpdateCoche(_coches, _cochesBool, "audiosettings", this->_containerSettings);
        UpdateJaugeBar(_jaugeBars, _jaugeBarsLevel, "audiosettings", this->_containerSettings, this->_soundManager);
        AppliedSettings(this->_clientToServer, this->_id, this->_containerSettings);
    } else {
        IfSaving(this->_isSaving, {this->getEntity("Do you want to save new settings?"), this->getEntity("Yes"), this->getEntity("No")}, this->_clientToServer, this->_id, this->_scene, this->_containerSettings, this->_lastScene);
        this->_start = false;
    }

    updateGifMenu(this->getEntity("backgroundMenu"));
}