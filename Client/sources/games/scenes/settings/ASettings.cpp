/*
** EPITECH PROJECT, 2024
** RType
** File description:
** ASettings
*/

#include "ASettings.hpp"
#include "../../../../includes/MainInclusdes.hpp"
#include <tuple>

static std::string DelWordInSentence(std::string sentence, std::string word)
{
    std::string::size_type i = sentence.find(word);
    std::string res = sentence;
    if (i != std::string::npos)
        res.erase(i, word.length());
    return (res);
}

static void updateTints(std::vector<Color>& tints, int selectedIndex)
{
    const Color selectedColor = BLUE;
    const Color deselectedColor = GRAY;

    for (size_t i = 0; i < tints.size(); ++i) {
        tints[i] = (i == (size_t)selectedIndex) ? selectedColor : deselectedColor;
    }
}

static void handleChoice(const std::string& choiceText, const std::string& paramText)
{
    if (paramText == "Resolution") {
        if (choiceText == "1280x720") {
            if (IsWindowFullscreen())
                ToggleFullscreen();
            SetWindowSize(1280, 720);
        } else if (choiceText == "Fullscreen") {
            if (!IsWindowFullscreen())
                ToggleFullscreen();
        } else {
            if (IsWindowFullscreen())
                ToggleFullscreen();
            SetWindowSize(1920, 1080);
        }
    }
}

static void UpdateContainerAudio(std::string type, rtype::ContainerSettings &containerSettings, std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> entities, std::vector<bool> enabled, std::vector<float> level)
{
    audio_settings_t audioSettings = containerSettings.getAudioSettings();

    int i = 1;
    int index = 2;

    for (; i < entities.size(); i += index) {
        std::shared_ptr<rtype::ECS::Ecs3D::Text> tmp = entities[i - 1]->getComponent<rtype::ECS::Ecs3D::Text>();
        if (tmp) {
            std::string key = tmp->text;
            if (type == "Coche") {
                if (key == "MasterVolume")
                    audioSettings.master = enabled[i];
                if (key == "Music")
                    audioSettings.music = enabled[i];
                if (key == "Effects")
                    audioSettings.effects = enabled[i];
                if (key == "Voice")
                    audioSettings.voice = enabled[i];
                if (key == "Voice chat")
                    audioSettings.voice_chat = enabled[i];
                if (key == "Voice chat push to talk")
                    audioSettings.voice_chat_push_to_talk = enabled[i];
            } else {
                if (key == "MasterVolume")
                    audioSettings.master = level[i] * 100;
                if (key == "Music")
                    audioSettings.music = level[i] * 100;
                if (key == "Effects")
                    audioSettings.effects = level[i] * 100;
                if (key == "Voice")
                    audioSettings.voice = level[i] * 100;
                if (key == "Voice chat")
                    audioSettings.voice_chat = level[i] * 100;
                if (key == "Voice chat push to talk")
                    audioSettings.voice_chat_push_to_talk = level[i] * 100;
            }
        }
    }
    containerSettings.setAudioSettings(audioSettings);
}

static void UpdateContainerMouse(std::string type, rtype::ContainerSettings &containerSettings, std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> entities, std::vector<bool> enabled, std::vector<float> level)
{
    mouse_settings_t mouseSettings = containerSettings.getMouseSettings();

    int i = 1;
    int index = 2;

    for (; i < entities.size(); i += index) {
        std::shared_ptr<rtype::ECS::Ecs3D::Text> tmp = entities[i - 1]->getComponent<rtype::ECS::Ecs3D::Text>();
        if (tmp) {
            std::string key = tmp->id;
            if (type == "Coche") {
                if (key == "Sensitivity")
                    mouseSettings.sensitivity = enabled[i];
                if (key == "Invert y")
                    mouseSettings.invert_y = enabled[i];
                if (key == "Invert x")
                    mouseSettings.invert_x = enabled[i];
                if (key == "Mouse acceleration")
                    mouseSettings.mouse_acceleration = enabled[i];
                if (key == "Mouse acceleration x")
                    mouseSettings.mouse_acceleration_x = enabled[i];
                if (key == "Mouse acceleration y")
                    mouseSettings.mouse_acceleration_y = enabled[i];
                if (key == "Mouse smoothing")
                    mouseSettings.mouse_smoothing = enabled[i];
                if (key == "Mouse smoothing x")
                    mouseSettings.mouse_smoothing_x = enabled[i];
                if (key == "Mouse smoothing y")
                    mouseSettings.mouse_smoothing_y = enabled[i];
                if (key == "Mouse filtering")
                    mouseSettings.mouse_filtering = enabled[i];
                if (key == "Mouse filtering x")
                    mouseSettings.mouse_filtering_x = enabled[i];
                if (key == "Mouse filtering y")
                    mouseSettings.mouse_filtering_y = enabled[i];
            } else {
                if (key == "Sensitivity")
                    mouseSettings.sensitivity = level[i] * 100;
                if (key == "Invert y")
                    mouseSettings.invert_y = level[i] * 100;
                if (key == "Invert x")
                    mouseSettings.invert_x = level[i] * 100;
                if (key == "Mouse acceleration")
                    mouseSettings.mouse_acceleration = level[i] * 100;
                if (key == "Mouse acceleration x")
                    mouseSettings.mouse_acceleration_x = level[i] * 100;
                if (key == "Mouse acceleration y")
                    mouseSettings.mouse_acceleration_y = level[i] * 100;
                if (key == "Mouse smoothing")
                    mouseSettings.mouse_smoothing = level[i] * 100;
                if (key == "Mouse smoothing x")
                    mouseSettings.mouse_smoothing_x = level[i] * 100;
                if (key == "Mouse smoothing y")
                    mouseSettings.mouse_smoothing_y = level[i] * 100;
                if (key == "Mouse filtering")
                    mouseSettings.mouse_filtering = level[i] * 100;
                if (key == "Mouse filtering x")
                    mouseSettings.mouse_filtering_x = level[i] * 100;
                if (key == "Mouse filtering y")
                    mouseSettings.mouse_filtering_y = level[i] * 100;
            }
        }
    }
    containerSettings.setMouseSettings(mouseSettings);
}

static void UpdateContainerVideo(std::string type, rtype::ContainerSettings &containerSettings, std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> entities, std::vector<int> value, std::vector<bool> enabled, std::vector<float> level, std::map<std::string, std::string> selectors)
{
    display_settings_t displaySettings = containerSettings.getDisplaySettings();

    int i = 1;
    int index = 2;
    if (type == "Increment") {
        index = 4;
        i = 3;
    }

    for (auto &sel : selectors) {
        if (sel.first == "Resolution")
            std::memcpy(displaySettings.framerate, sel.second.c_str(), sel.second.size());
        if (sel.first == "Colorblind")
            std::memcpy(displaySettings.framerate, sel.second.c_str(), sel.second.size());
    }
    if (type == "Select")
        return;

    for (; i < entities.size(); i += index) {
        std::shared_ptr<rtype::ECS::Ecs3D::Text> tmp = entities[i - 1]->getComponent<rtype::ECS::Ecs3D::Text>();
        if (tmp) {
            std::string key = tmp->id;
            if (type == "Coche") {
                if (key == "Vsync")
                    displaySettings.vsync = enabled[i];
                if (key == "Fov")
                    displaySettings.fov = enabled[i];
                if (key == "Brightness")
                    displaySettings.brightness = enabled[i];
                if (key == "Gamma")
                    displaySettings.gamma = enabled[i];
                if (key == "Contrast")
                    displaySettings.contrast = enabled[i];
                if (key == "Saturation")
                    displaySettings.saturation = enabled[i];
                if (key == "Sharpness")
                    displaySettings.sharpness = enabled[i];
            } else if (type == "Increment") {
                if (key == "Fov")
                    displaySettings.fov = value[i];
                if (key == "Brightness")
                    displaySettings.brightness = value[i];
                if (key == "Gamma")
                    displaySettings.gamma = value[i];
                if (key == "Contrast")
                    displaySettings.contrast = value[i];
                if (key == "Saturation")
                    displaySettings.saturation = value[i];
                if (key == "Sharpness")
                    displaySettings.sharpness = value[i];
                if (key == "Frame Per Second (FPS)") {
                    if (value[i] == 241)
                        std::memcpy(displaySettings.framerate, "unlimited", 10);
                    else {
                        std::string tmp = std::to_string(value[i]);
                        std::memcpy(displaySettings.framerate,tmp.c_str(), tmp.size());
                    }
                }
            } else {
                if (key == "Fov")
                    displaySettings.fov = level[i] * 100;
                if (key == "Brightness")
                    displaySettings.brightness = level[i] * 100;
                if (key == "Gamma")
                    displaySettings.gamma = level[i] * 100;
                if (key == "Contrast")
                    displaySettings.contrast = level[i] * 100;
                if (key == "Saturation")
                    displaySettings.saturation = level[i] * 100;
                if (key == "Sharpness")
                    displaySettings.sharpness = level[i] * 100;
            }
        }
    }

    containerSettings.setDisplaySettings(displaySettings);
}

static void UpdateContainerProfile(rtype::ContainerSettings &containerSetting)
{
}

static void UpdateContainer(std::string whereAmI, std::string type, rtype::ContainerSettings &containerSettings, std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> entities, std::vector<int> value, std::vector<bool> enabled, std::vector<float> level, std::map<std::string, std::string> selectors)
{
    if (whereAmI == "audiosettings")
        UpdateContainerAudio(type, containerSettings, entities, enabled, level);
    if (whereAmI == "mousesettings")
        UpdateContainerMouse(type, containerSettings, entities, enabled, level);
    if (whereAmI == "videosettings")
        UpdateContainerVideo(type, containerSettings, entities, value, enabled, level, selectors);
    if (whereAmI == "profilsettings")
        UpdateContainerProfile(containerSettings);
}

void rtype::ASettings::drawTextInRectangle(std::vector<std::string> texts, std::vector<std::string> values, Rectangle rectangle, Font font, Color color, float fontSize)
{
    int numTexts = texts.size();
    float totalHeight = rectangle.height - 2.0f * (numTexts - 1);
    float Space = totalHeight / numTexts;

    float x = rectangle.x + 10.0f;
    float y = rectangle.y + 10.0f;
    float lengthText = 0.0f;

    for (auto text : texts) {
        float tmp = MeasureText(text.c_str(), fontSize);
        if (lengthText < tmp)
            lengthText = tmp;
    }

    for (int i = 0; i < texts.size(); i++) {
        DrawTextEx(font, texts[i].c_str(), {x, y}, fontSize, 0, color);
        DrawTextEx(font, values[i].c_str(), {x + lengthText, y}, fontSize, 0, color);
        y += Space;
    }
}

std::map<std::string, std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>>> rtype::ASettings::createBindInBox(std::initializer_list<std::string> texts, std::initializer_list<std::string> values, Rectangle rectangle, Font font, Color textColor, float fontSize)
{
    std::map<std::string, std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>>> map;
    int it = 0;
    float maxTextWidth = 0.0f;
    for (const auto& text : texts) {
        float textWidth = MeasureText(text.c_str(), fontSize);
        maxTextWidth = fmax(maxTextWidth, textWidth);
    }

    float totalWidth = 10.0f + maxTextWidth + 20.0f;
    int numValues = values.size();
    float totalHeight = rectangle.height - 2.0f * (numValues - 1);
    float space = totalHeight / numValues;

    for (const auto& value : values) {
        float tmpWidth = 10.0f + MeasureText(value.c_str(), fontSize);
        if (tmpWidth > totalWidth)
            totalWidth = tmpWidth;
    }

    float xText = rectangle.x + 10.0f;
    float xValue = rectangle.x + totalWidth - 10.0f;
    float y = rectangle.y + 10.0f;
    for (const auto& text : texts) {
        std::shared_ptr<rtype::ECS::Ecs3D::IEntity> tmpText = rtype::ECS::Ecs3D::createText("NameBinding " + std::to_string(it), text, {xText, y}, font, fontSize, textColor, 0);
        map.insert(std::pair<std::string, std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>>>("NameBinding " + std::to_string(it), {tmpText}));
        y += space;
        it += 1;
    }

    y = rectangle.y + 10.0f;
    it = 0;

    for (const auto& value : values) {
        std::shared_ptr<rtype::ECS::Ecs3D::IEntity> tmpText = rtype::ECS::Ecs3D::createText("Binding " + std::to_string(it), value, {xValue, y}, font, fontSize, textColor, 0);
        std::shared_ptr<rtype::ECS::Ecs3D::IEntity> tmpBox = rtype::ECS::Ecs3D::createBox("BindingBox " + std::to_string(it), {xValue - 10, y, (float)200, fontSize});
        map.find("NameBinding " + std::to_string(it))->second.push_back(tmpText);
        map.find("NameBinding " + std::to_string(it))->second.push_back(tmpBox);
        y += space;
        it += 1;
    }
    return (map);
}

int rtype::ASettings::WhichBindisClikcked(std::map<std::string, std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>>> map)
{
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePosition = GetMousePosition();
        for (auto &it : map) {
            auto button = it.second[2]->getComponent<rtype::ECS::Ecs3D::Rectangle2d>();
            if (CheckCollisionPointRec(mousePosition, button->rec)) {
                return (std::stoi(DelWordInSentence(it.first, "NameBinding ")));
            }
        }
    }
    return (-1);
}

void rtype::ASettings::UpdateBinding(std::map<std::string, std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>>> &map, int &rectVisible, std::string whereAmI, rtype::ContainerSettings &containerSettings)
{
    std::string key = "NameBinding " + std::to_string(rectVisible);
    int input = GetKeyPressed();
    if (input > 0) {
        map.find(key)->second[1]->getComponent<rtype::ECS::Ecs3D::Text>()->text = rtype::ECS::Ecs3D::GetKeyName(input);
        UpdateBindStruct(map.find(key)->second[0]->getComponent<rtype::ECS::Ecs3D::Text>()->text, input, containerSettings);
        rectVisible = -1;
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        map.find(key)->second[1]->getComponent<rtype::ECS::Ecs3D::Text>()->text = "Left click";
        UpdateBindStruct(map.find(key)->second[0]->getComponent<rtype::ECS::Ecs3D::Text>()->text, MOUSE_BUTTON_LEFT, containerSettings);
        rectVisible = -1;
    }
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        map.find(key)->second[1]->getComponent<rtype::ECS::Ecs3D::Text>()->text = "Right click";
        UpdateBindStruct(map.find(key)->second[0]->getComponent<rtype::ECS::Ecs3D::Text>()->text, MOUSE_BUTTON_RIGHT, containerSettings);
        rectVisible = -1;
    }
}

void rtype::ASettings::UpdateCoche(std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> coches, std::vector<bool> &enabled, std::string whereAmI, rtype::ContainerSettings &containerSettings)
{
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        int index = 0;
        Vector2 mousePosition = GetMousePosition();
        std::string name;
        for (auto &coche : coches) {
            auto button = coche->getComponent<rtype::ECS::Ecs3D::Rectangle2d>();
            auto text = coche->getComponent<rtype::ECS::Ecs3D::Text>();
            if (text)
                name = text->text;
            if (button && CheckCollisionPointRec(mousePosition, button->rec)) {
                rtype::SoundManager sounds;
                enabled[index] = !enabled[index];
                ChooseGoodUpdateStruct(whereAmI, name, std::to_string(enabled[index]), containerSettings, sounds);
            }
            index += 1;
        }
        UpdateContainer(whereAmI, "Coche", containerSettings, coches, {}, enabled, {}, {});
    }
}

void rtype::ASettings::DrawCoche(std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> coches, std::vector<bool> enabled, Font font)
{
    int index = 0;
    for (auto &coche : coches) {
        auto button = coche->getComponent<rtype::ECS::Ecs3D::Rectangle2d>();
        auto text = coche->getComponent<rtype::ECS::Ecs3D::Text>();
        if (button) {
            DrawRectangleRec(button->rec, LIGHTGRAY);
            if (enabled[index]) {
                DrawLine(button->rec.x, button->rec.y, button->rec.x + button->rec.width, button->rec.y + button->rec.height, BLACK);
                DrawLine(button->rec.x, button->rec.y + button->rec.height, button->rec.x + button->rec.width, button->rec.y, BLACK);
            }
            std::string tmp = (enabled[index] ? " Enabled" : " Disabled");
            DrawTextEx(font, tmp.c_str(), {button->rec.x + button->rec.width + 10.0f, button->rec.y - 10.0f}, text->fontSize, 0, WHITE);
        } else if (text) {
            DrawTextEx(font, text->text.c_str(), {text->position.x, text->position.y}, text->fontSize, 0, text->tint);
        }
        index += 1;
    }
}

void rtype::ASettings::UpdateJaugeBar(std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> bars, std::vector<float> &level, std::string whereAmI, rtype::ContainerSettings &containerSettings, rtype::SoundManager &soundManager)
{
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        int index = 0;
        Vector2 mousePosition = GetMousePosition();
        std::string name;
        for (auto &bar : bars) {
            auto button = bar->getComponent<rtype::ECS::Ecs3D::Rectangle2d>();
            auto text = bar->getComponent<rtype::ECS::Ecs3D::Text>();
            if (text)
                name = text->text;
            if (button && CheckCollisionPointRec(mousePosition, button->rec)) {
                level[index] = (GetMouseX() - button->rec.x) / (button->rec.width - button->rec.height);
                if (level[index] < 0.0f)
                    level[index] = 0.0f;
                if (level[index] > 1.0f)
                    level[index] = 1.0f;
                ChooseGoodUpdateStruct(whereAmI, name, std::to_string((int)(level[index] * 100)), containerSettings, soundManager);
                }
            index += 1;
        }
        UpdateContainer(whereAmI, "JaugeBar", containerSettings, bars, {}, {}, level, {});
    }
}

void rtype::ASettings::DrawJaugeBar(std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> bars, std::vector<float> level, Font font)
{
    int index = 0;
    for (auto &bar : bars) {
        auto button = bar->getComponent<rtype::ECS::Ecs3D::Rectangle2d>();
        auto text = bar->getComponent<rtype::ECS::Ecs3D::Text>();
        if (button) {
            DrawRectangleRec(button->rec, LIGHTGRAY);
            DrawRectangle(button->rec.x, button->rec.y, button->rec.width * level[index], button->rec.height, BLUE);
            std::string tmp = std::to_string((int)(level[index] * 100)) + "%";
            DrawTextEx(font, tmp.c_str(), {button->rec.x + button->rec.width + 20.0f, button->rec.y + 10.0f}, 20, 0, WHITE);
        }
        if (text) {
            DrawTextEx(font, text->text.c_str(), {text->position.x, text->position.y}, text->fontSize, 0, text->tint);
        }
        index += 1;
    }
}

void rtype::ASettings::UpdateIncrement(std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> increments, std::vector<int> &value, std::string whereAmI, rtype::ContainerSettings &containerSettings)
{
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        for (int i = 0; i + 3 < increments.size();) {
            int index = (i + 1 / 4) + 1;
            auto buttonLess = increments[i]->getComponent<rtype::ECS::Ecs3D::Rectangle2d>();
            auto buttonMore = increments[i + 2]->getComponent<rtype::ECS::Ecs3D::Rectangle2d>();
            auto text = increments[i + 1]->getComponent<rtype::ECS::Ecs3D::Text>();
            if (CheckCollisionPointRec(GetMousePosition(), buttonLess->rec)) {
                if (value[index] == 241)
                    value[index] = 240;
                else
                    value[index] -= 30;
                if (value[index] < 30)
                    value[index] = 30;
                if (text->text.find("FPS") != std::string::npos) {
                    SetTargetFPS(value[index]);
                    rtype::SoundManager sounds;
                    ChooseGoodUpdateStruct(whereAmI, text->text, std::to_string(value[index]), containerSettings, sounds);
                }
            }
            if (CheckCollisionPointRec(GetMousePosition(), buttonMore->rec)) {
                bool incremented = false;
                if (value[index] < 240) {
                    value[index] += 30;
                    incremented = true;
                }

                if (text->text.find("FPS") != std::string::npos) {
                    if (value[index] == 240 && incremented == false)
                        value[index] += 1;
                    SetTargetFPS(value[index]);
                    rtype::SoundManager sounds;
                    ChooseGoodUpdateStruct(whereAmI, text->text, std::to_string(value[index]), containerSettings, sounds);
                    if (value[index] > 240) {
                        SetTargetFPS(0);
                        ChooseGoodUpdateStruct(whereAmI, text->text, "unlimited", containerSettings, sounds);
                    }
                }
            }
            i += 4;
        }
        UpdateContainer(whereAmI, "Increment", containerSettings, increments, value, {}, {}, {});
    }
}

void rtype::ASettings::DrawIncrement(std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> increments, std::vector<int> value, Font font)
{
    for (int i = 0; i + 3 < increments.size();) {
        int index = (i + 1 / 4) + 1;
        auto buttonLess = increments[i]->getComponent<rtype::ECS::Ecs3D::Rectangle2d>()->rec;
        auto buttonText = increments[i + 1]->getComponent<rtype::ECS::Ecs3D::Rectangle2d>()->rec;
        auto buttonMore = increments[i + 2]->getComponent<rtype::ECS::Ecs3D::Rectangle2d>()->rec;
        auto textLess = increments[i]->getComponent<rtype::ECS::Ecs3D::Text>();
        auto textText = increments[i + 1]->getComponent<rtype::ECS::Ecs3D::Text>();
        auto textMore = increments[i + 2]->getComponent<rtype::ECS::Ecs3D::Text>();
        auto textParam = increments[i + 3]->getComponent<rtype::ECS::Ecs3D::Text>();
        DrawTextEx(font, textParam->text.c_str(), {textParam->position.x, textParam->position.y}, textParam->fontSize, 0, textParam->tint);
        DrawRectangleRec(buttonLess, BLUE);
        DrawTextEx(font, textLess->text.c_str(), {buttonLess.x + 15, buttonLess.y + 15}, textLess->fontSize, 0, textLess->tint);
        DrawRectangleRec(buttonMore, BLUE);
        DrawTextEx(font, textMore->text.c_str(), {buttonMore.x + 15, buttonMore.y + 15}, textMore->fontSize, 0, textMore->tint);
        DrawRectangleRec(buttonText, GRAY);
        if (textText->text.find("FPS") != std::string::npos) {
            std::string fps = "Unlimited";
            if (value[index] != 241) {
                fps = std::to_string(value[index]);
                DrawTextEx(font, fps.c_str(), {buttonText.x + 60, buttonText.y + 15}, textText->fontSize, 0, textText->tint);
            } else
                DrawTextEx(font, fps.c_str(), {buttonText.x + 8, buttonText.y + 15}, textText->fontSize, 0, textText->tint);
        }
        i += 4;
    }
}

void rtype::ASettings::UpdateSelect(std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> selects, std::vector<Color>& tints, std::string whereAmI, rtype::ContainerSettings &containerSettings)
{
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        std::map<std::string, std::string> tmp;
        rtype::SoundManager sounds;
        for (int i = 0; i + 3 < selects.size(); i += 4) {
            auto firstChoice = selects[i]->getComponent<rtype::ECS::Ecs3D::Rectangle2d>();
            auto nameFirstChoice = selects[i]->getComponent<rtype::ECS::Ecs3D::Text>();
            auto secondChoice = selects[i + 1]->getComponent<rtype::ECS::Ecs3D::Rectangle2d>();
            auto nameSecondChoice = selects[i + 1]->getComponent<rtype::ECS::Ecs3D::Text>();
            auto thirdChoice = selects[i + 2]->getComponent<rtype::ECS::Ecs3D::Rectangle2d>();
            auto nameThirdChoice = selects[i + 2]->getComponent<rtype::ECS::Ecs3D::Text>();
            auto nameParam = selects[i + 3]->getComponent<rtype::ECS::Ecs3D::Text>();

            if (CheckCollisionPointRec(GetMousePosition(), firstChoice->rec)) {
                rtype::SoundManager sounds;
                updateTints(tints, i);
                handleChoice(nameFirstChoice->text, nameParam->text);
                ChooseGoodUpdateStruct(whereAmI, nameParam->text, nameFirstChoice->text, containerSettings, sounds);
                tmp.insert(std::pair<std::string, std::string>(nameParam->text, nameFirstChoice->text));
            }
            if (CheckCollisionPointRec(GetMousePosition(), secondChoice->rec)) {
                rtype::SoundManager sounds;
                updateTints(tints, i + 1);
                handleChoice(nameSecondChoice->text, nameParam->text);
                ChooseGoodUpdateStruct(whereAmI, nameParam->text, nameSecondChoice->text, containerSettings, sounds);
                tmp.insert(std::pair<std::string, std::string>(nameParam->text, nameSecondChoice->text));
            }
            if (CheckCollisionPointRec(GetMousePosition(), thirdChoice->rec)) {
                rtype::SoundManager sounds;
                updateTints(tints, i + 2);
                handleChoice(nameThirdChoice->text, nameParam->text);
                ChooseGoodUpdateStruct(whereAmI, nameParam->text, nameThirdChoice->text, containerSettings, sounds);
                tmp.insert(std::pair<std::string, std::string>(nameParam->text, nameThirdChoice->text));
            }
        }
        UpdateContainer(whereAmI, "Select", containerSettings, selects, {}, {}, {}, tmp);
    }
}

void rtype::ASettings::DrawSelect(std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> selects, std::vector<Color> tints, Font font)
{
    for (int i = 0; i < selects.size(); i++) {
        auto choice = selects[i]->getComponent<rtype::ECS::Ecs3D::Rectangle2d>();
        auto nameChoice = selects[i]->getComponent<rtype::ECS::Ecs3D::Text>();
        if (choice && nameChoice) {
            DrawRectangleRec(choice->rec, tints[i]);
            DrawTextEx(font, nameChoice->text.c_str(), {choice->rec.x + 10, choice->rec.y + 10}, nameChoice->fontSize, 0, nameChoice->tint);
        } else if (nameChoice) {
            DrawTextEx(font, nameChoice->text.c_str(), {nameChoice->position.x, nameChoice->position.y}, nameChoice->fontSize, 0, nameChoice->tint);
        }
    }
}

void rtype::ASettings::UpdateBindStruct(std::string index, int value, rtype::ContainerSettings &containerSettings)
{
    _binds = containerSettings.getBinds();
    _old_binds = containerSettings.getOldBinds();
    static const std::unordered_map<std::string, int*> indexToMember = {
        {"Forward", &_binds.forward},
        {"Backward", &_binds.backward},
        {"Left", &_binds.left},
        {"Right", &_binds.right},
        {"Shoot", &_binds.shoot},
        {"Scope", &_binds.scope},
        {"Reload", &_binds.reload},
        {"Run", &_binds.run},
        {"Swap", &_binds.swap},
        {"Jump", &_binds.jump},
        {"Pause", &_binds.pause},
        {"Chat", &_binds.chat},
        {"Score", &_binds.score},
        {"Map", &_binds.map},
        {"Mute", &_binds.mute},
        {"Quit", &_binds.quit}
    };

    auto it = indexToMember.find(index);
    if (it != indexToMember.end()) {
        *(it->second) = value;
    }
    containerSettings.setBinds(_binds);
}

void rtype::ASettings::UpdateMouseSettingsStruct(std::string index, int value, rtype::ContainerSettings &containerSettings)
{
    _mouseSettings = containerSettings.getMouseSettings();
    _old_mouseSettings = containerSettings.getOldMouseSettings();
    static const std::unordered_map<std::string, int*> indexToMember = {
        {"Sensitivity", &_mouseSettings.sensitivity},
        {"Invert y", &_mouseSettings.invert_y},
        {"Invert x", &_mouseSettings.invert_x},
        {"Mouse acceleration", &_mouseSettings.mouse_acceleration},
        {"Mouse smoothing", &_mouseSettings.mouse_smoothing},
        {"Mouse filtering", &_mouseSettings.mouse_filtering},
        {"Mouse acceleration x", &_mouseSettings.mouse_acceleration_x},
        {"Mouse acceleration y", &_mouseSettings.mouse_acceleration_y},
        {"Mouse smoothing x", &_mouseSettings.mouse_smoothing_x},
        {"Mouse smoothing y", &_mouseSettings.mouse_smoothing_y},
        {"Mouse filtering x", &_mouseSettings.mouse_filtering_x},
        {"Mouse filtering y", &_mouseSettings.mouse_filtering_y}
    };

    auto it = indexToMember.find(index);
    if (it != indexToMember.end()) {
        *(it->second) = value;
    }
    containerSettings.setMouseSettings(_mouseSettings);
}

void rtype::ASettings::UpdateAudioStruct(std::string index, int value, rtype::ContainerSettings &containerSettings, rtype::SoundManager &sounds)
{
    _audio = containerSettings.getAudioSettings();
    _old_audio = containerSettings.getOldAudioSettings();
    static const std::unordered_map<std::string, int*> indexToMember = {
        {"MasterVolume", &_audio.master},
        {"Music", &_audio.music},
        {"Effects", &_audio.effects},
        {"Voice", &_audio.voice},
        {"Voice chat", &_audio.voice_chat},
        {"Voice chat push to talk", &_audio.voice_chat_push_to_talk}
    };

    auto it = indexToMember.find(index);
    if (it != indexToMember.end()) {
        *(it->second) = value;
        sounds.updateVolume(index, (float)value);
    }
    containerSettings.setAudioSettings(_audio);
}

void rtype::ASettings::UpdateVideoSettingsStruct(std::string index, std::string value, rtype::ContainerSettings &containerSettings)
{
    _videoSettings = containerSettings.getDisplaySettings();
    _old_videoSettings = containerSettings.getOldDisplaySettings();
    static const std::unordered_map<std::string, int*> indexToMember = {
        {"Vsync", &_videoSettings.vsync},
        {"Fov", &_videoSettings.fov},
        {"Brightness", &_videoSettings.brightness},
        {"Gamma", &_videoSettings.gamma},
        {"Contrast", &_videoSettings.contrast},
        {"Saturation", &_videoSettings.saturation},
        {"Sharpness", &_videoSettings.sharpness}
    };
    if (index == "Resolution") {
        strncpy(_videoSettings.resolution, value.c_str(), sizeof(_videoSettings.resolution) - 1);
        _videoSettings.resolution[sizeof(_videoSettings.resolution) - 1] = '\0';
    }
    if (index == "Frame Per Second (FPS)") {
        strncpy(_videoSettings.framerate, value.c_str(), sizeof(_videoSettings.framerate) - 1);
        _videoSettings.framerate[sizeof(_videoSettings.framerate) - 1] = '\0';
    }
    if (index == "Colorblind") {
        strncpy(_videoSettings.colorblind, value.c_str(), sizeof(_videoSettings.colorblind) - 1);
        _videoSettings.colorblind[sizeof(_videoSettings.colorblind) - 1] = '\0';
    }

    auto it = indexToMember.find(index);
    if (it != indexToMember.end()) {
        *(it->second) = std::stoi(value);
    }
    containerSettings.setDisplaySettings(_videoSettings);
}

void rtype::ASettings::UpdateProfileStruct(std::string index, std::string value, rtype::ContainerSettings &containerSettings)
{
    _profile = containerSettings.getProfile();
    _old_profile = containerSettings.getOldProfile();
    if (index == "Username") {
        strncpy(_profile.username, value.c_str(), sizeof(_profile.username) - 1);
        _profile.username[sizeof(_profile.username) - 1] = '\0';
    }
    if (index == "Level")
        _profile.level = std::stoi(value);

    containerSettings.setProfile(_profile);
}

void rtype::ASettings::ChooseGoodUpdateStruct(std::string whereAmI, std::string index, std::string value, rtype::ContainerSettings &containerSettings, rtype::SoundManager &sounds)
{
    if (whereAmI == "videosettings")
        rtype::ASettings::UpdateVideoSettingsStruct(index, value, containerSettings);
    if (whereAmI == "audiosettings")
        rtype::ASettings::UpdateAudioStruct(index, std::stoi(value), containerSettings, sounds);
    if (whereAmI == "mousesettings")
        rtype::ASettings::UpdateMouseSettingsStruct(index, std::stoi(value), containerSettings);
    if (whereAmI == "binds")
        rtype::ASettings::UpdateBindStruct(index, std::stoi(value), containerSettings);
    if (whereAmI == "profile")
        rtype::ASettings::UpdateProfileStruct(index, value, containerSettings);
}

void rtype::ASettings::sendSettingsToServer(std::unique_ptr<rtype::ClientToServer> &clientToServer, int id, rtype::ContainerSettings &co)
{
    rtype::net::message<MainServerMsgTypes> msgBind;
    rtype::net::message<MainServerMsgTypes> msgProfile;
    rtype::net::message<MainServerMsgTypes> msgVideoSettings;
    rtype::net::message<MainServerMsgTypes> msgMouseSettings;
    rtype::net::message<MainServerMsgTypes> msgAudio;

    msgBind.header.id = MainServerMsgTypes::Binds;
    msgProfile.header.id = MainServerMsgTypes::ProfileInfos;
    msgVideoSettings.header.id = MainServerMsgTypes::DisplaySettings;
    msgMouseSettings.header.id = MainServerMsgTypes::MouseSettings;
    msgAudio.header.id = MainServerMsgTypes::AudioSettings;

    _binds = co.getBinds();
    _profile = co.getProfile();
    _videoSettings = co.getDisplaySettings();
    _mouseSettings = co.getMouseSettings();
    _audio = co.getAudioSettings();

    msgBind << _binds;
    msgProfile << _profile;
    msgVideoSettings << _videoSettings;
    msgMouseSettings << _mouseSettings;
    msgAudio << _audio;

    clientToServer->Send(msgBind);
    clientToServer->Send(msgProfile);
    clientToServer->Send(msgVideoSettings);
    clientToServer->Send(msgMouseSettings);
    clientToServer->Send(msgAudio);

    _old_audio = _audio;
    _old_videoSettings = _videoSettings;
    _old_mouseSettings = _mouseSettings;
    _old_binds = _binds;
    _old_profile = _profile;
}

void rtype::ASettings::AppliedSettings(std::unique_ptr<rtype::ClientToServer> &clientToServer, int id, rtype::ContainerSettings &co)
{
    Vector2 mousePoint = GetMousePosition();
    for (auto &entity : this->getEntities()) {
        if (entity.second->getComponent<rtype::ECS::Ecs3D::ButtonMenu>() != nullptr) {
            auto button = entity.second->getComponent<rtype::ECS::Ecs3D::ButtonMenu>();
            if (CheckCollisionPointRec(mousePoint, button->button)) {
                button->isHover = true;

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (button->id == "AppliedButton")
                        sendSettingsToServer(clientToServer, id, co);
                }
            } else
                button->isHover = false;
            if (GetFrameTime() == 0.0f)
                button->initialPosition = { button->button.x, button->button.y };
        }
    }
}

void rtype::ASettings::WantToSave(bool wantToSave, std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> texts, Font font)
{
    if (wantToSave) {
        DrawRectangle(GetScreenWidth() / 2 - 225, GetScreenHeight() / 2 - 225, 450, 450, ORANGE);
        for (auto &text : texts) {
            auto button = text->getComponent<rtype::ECS::Ecs3D::ButtonMenu>();
            auto fontSize = text->getComponent<rtype::ECS::Ecs3D::FontSize>();
            DrawTextEx(font, button->id.c_str(), {button->button.x, button->button.y}, fontSize->size, 0, WHITE);
        }
    }
}

void rtype::ASettings::IfSaving(bool &wantToSave, std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> texts, std::unique_ptr<rtype::ClientToServer> &clientToServer, int id, _Scene &scene, rtype::ContainerSettings &containerSettings, _Scene lastScene)
{
    if (wantToSave && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        for (const auto& entity : texts) {
            auto button = entity->getComponent<rtype::ECS::Ecs3D::ButtonMenu>();
            if (CheckCollisionPointRec(GetMousePosition(), button->button)) {
                if (button->id == "Yes")
                    sendSettingsToServer(clientToServer, id, containerSettings);
                else {
                    if (!_FirstSave) {
                        _old_audio = containerSettings.getAudioSettings();
                        _old_videoSettings = containerSettings.getDisplaySettings();
                        _old_mouseSettings = containerSettings.getMouseSettings();
                        _old_binds = containerSettings.getBinds();
                        _old_profile = containerSettings.getProfile();
                        _FirstSave = true;
                    } else {
                        _audio = _old_audio;
                        _videoSettings = _old_videoSettings;
                        _mouseSettings = _old_mouseSettings;
                        _binds = _old_binds;
                        _profile = _old_profile;
                        containerSettings.setAudioSettings(_audio);
                        containerSettings.setDisplaySettings(_videoSettings);
                        containerSettings.setMouseSettings(_mouseSettings);
                        containerSettings.setBinds(_binds);
                        containerSettings.setProfile(_profile);
                    }
                }
                SetExitKey(KEY_ESCAPE);
                if (lastScene == _Scene::GAME) {
                    std::cout << "GAME" << std::endl;
                    EnableCursor();
                    SetExitKey(0);
                }
                scene = lastScene;
                wantToSave = false;
            }
        }
    }
}