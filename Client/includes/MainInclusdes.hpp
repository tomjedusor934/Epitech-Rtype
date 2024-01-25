/*
** EPITECH PROJECT, 2023
** RType
** File description:
** MainInclusdes
*/

#pragma once
    #include "./CommonIncludes.hpp"
    #include "../sources/net/ClientToServer.hpp"
    #include "../sources/net/ClientToGame.hpp"
    #include "../sources/games/soundManager/soundManager.hpp"


static const std::map<int, std::string> inputKeyBoardMap = {
    {KEY_APOSTROPHE, "\'"},
    {KEY_COMMA, ","},
    {KEY_MINUS, "-"},
    {KEY_PERIOD, "."},
    {KEY_SLASH, "/"},
    {KEY_ZERO, "0"},
    {KEY_ONE, "1"},
    {KEY_TWO, "2"},
    {KEY_THREE, "3"},
    {KEY_FOUR, "4"},
    {KEY_FIVE, "5"},
    {KEY_SIX, "6"},
    {KEY_SEVEN, "7"},
    {KEY_EIGHT, "8"},
    {KEY_NINE, "9"},
    {KEY_SEMICOLON, ";"},
    {KEY_EQUAL, "="},
    {KEY_A, "A"},
    {KEY_B, "B"},
    {KEY_C, "C"},
    {KEY_D, "D"},
    {KEY_E, "E"},
    {KEY_F, "F"},
    {KEY_G, "G"},
    {KEY_H, "H"},
    {KEY_I, "I"},
    {KEY_J, "J"},
    {KEY_K, "K"},
    {KEY_L, "L"},
    {KEY_M, "M"},
    {KEY_N, "N"},
    {KEY_O, "O"},
    {KEY_P, "P"},
    {KEY_Q, "Q"},
    {KEY_R, "R"},
    {KEY_S, "S"},
    {KEY_T, "T"},
    {KEY_U, "U"},
    {KEY_V, "V"},
    {KEY_W, "W"},
    {KEY_X, "X"},
    {KEY_Y, "Y"},
    {KEY_Z, "Z"},
    {KEY_LEFT_BRACKET, "["},
    {KEY_BACKSLASH, "\\"},
    {KEY_RIGHT_BRACKET, "]"},
    {KEY_GRAVE, "`"},
    {KEY_SPACE, "Space"},
    {KEY_ESCAPE, "Esc"},
    {KEY_ENTER, "Enter"},
    {KEY_TAB, "Tab"},
    {KEY_BACKSPACE, "Backspace"},
    {KEY_INSERT, "Ins"},
    {KEY_DELETE, "Del"},
    {KEY_RIGHT, "Cursor right"},
    {KEY_LEFT, "Cursor left"},
    {KEY_DOWN, "Cursor down"},
    {KEY_UP, "Cursor up"},
    {KEY_PAGE_UP, "Page up"},
    {KEY_PAGE_DOWN, "Page down"},
    {KEY_HOME, "Home"},
    {KEY_END, "End"},
    {KEY_CAPS_LOCK, "Caps lock"},
    {KEY_SCROLL_LOCK, "Scroll down"},
    {KEY_NUM_LOCK, "Num lock"},
    {KEY_PRINT_SCREEN, "Print screen"},
    {KEY_PAUSE, "Pause"},
    {KEY_F1, "F1"},
    {KEY_F2, "F2"},
    {KEY_F3, "F3"},
    {KEY_F4, "F4"},
    {KEY_F5, "F5"},
    {KEY_F6, "F6"},
    {KEY_F7, "F7"},
    {KEY_F8, "F8"},
    {KEY_F9, "F9"},
    {KEY_F10, "F10"},
    {KEY_F11, "F11"},
    {KEY_F12, "F12"},
    {KEY_LEFT_SHIFT, "Shift left"},
    {KEY_LEFT_CONTROL, "Control left"},
    {KEY_LEFT_ALT, "Alt left"},
    {KEY_LEFT_SUPER, "Super left"},
    {KEY_RIGHT_SHIFT, "Shift right"},
    {KEY_RIGHT_CONTROL, "Control right"},
    {KEY_RIGHT_ALT, "Alt right"},
    {KEY_RIGHT_SUPER, "Super right"},
    {KEY_KB_MENU, "KB menu"},
    {KEY_KP_0, "KEYpad 0"},
    {KEY_KP_1, "KEYpad 1"},
    {KEY_KP_2, "KEYpad 2"},
    {KEY_KP_3, "KEYpad 3"},
    {KEY_KP_4, "KEYpad 4"},
    {KEY_KP_5, "KEYpad 5"},
    {KEY_KP_6, "KEYpad 6"},
    {KEY_KP_7, "KEYpad 7"},
    {KEY_KP_8, "KEYpad 8"},
    {KEY_KP_9, "KEYpad 9"},
    {KEY_KP_DECIMAL, "KEYpad ."},
    {KEY_KP_DIVIDE, "KEYpad /"},
    {KEY_KP_MULTIPLY, "KEYpad *"},
    {KEY_KP_SUBTRACT, "KEYpad -"},
    {KEY_KP_ADD, "+"},
    {KEY_KP_ENTER, "Enter"},
    {KEY_KP_EQUAL, "="},
    {KEY_BACK, "back button"},
    {KEY_MENU, "menu button"},
    {KEY_VOLUME_UP, "volume up button"},
    {KEY_VOLUME_DOWN, "volume down button"},
    {MOUSE_BUTTON_LEFT, "Left click"},
    {MOUSE_BUTTON_RIGHT, "Right click"},
    {MOUSE_BUTTON_MIDDLE, "Middle click"},
    {MOUSE_BUTTON_SIDE, "Side click"},
    {MOUSE_BUTTON_EXTRA, "Extra click"}

};

namespace rtype::ECS::Ecs3D {

    // file path : sources/game/Builder.cpp
    void builderImg(std::string name, std::string filepathImage, AScene &scene);
    std::shared_ptr<IEntity> createImg(std::string name, std::string filepathImage);
    void builderButtonWithImage(std::string name, std::string filepath, Rectangle position, AScene &scene);
    std::shared_ptr<IEntity> createButtonWithImage(std::string name, std::string filepath, Rectangle position);
    void buildTextArea(std::string name, std::string text, Vector2 position, Rectangle rec, AScene &scene, Font font, int fontSize, Color color, int spacing, bool isFocus, int nbChar);
    std::shared_ptr<IEntity> createTextArea(std::string name, std::string text, Vector2 position, Rectangle rec, Font font, int fontSize, Color color, int spacing, bool isFocus, int nbChar);
    void builderImgAnim(std::string name, std::string filepathImage, int frameCount, AScene &menu);
    std::shared_ptr<IEntity> createImgAnim(std::string name, std::string filepathImage, int frameCount);
    void builderButtonWithHover(std::string name, std::string filepath, std::string filepathHover, Rectangle position, AScene &menu);
    std::shared_ptr<IEntity> createButtonWithHoverFirstPart(std::string name, std::string filepath, Rectangle position);
    std::shared_ptr<IEntity> createButtonWithHoverSecondPart(std::string name, std::string filepathHover, Rectangle position);
    void builderButtonWithFont(std::string name, Rectangle position, float size, AScene &menu);
    std::shared_ptr<IEntity> createButtonWithFont(std::string name, Rectangle position, float size);
    void buildCameraMenu(std::string name, Vector3 position, Vector3 target, Vector3 up, float fov, int projection, AScene &menu);
    void buildModel3dMenu(std::string name, Model model, int animsCount, unsigned int animCurrentFrame, unsigned int animIndex, std::string filepath, Vector3 scale, Vector3 rotationAxis, float rotationAngle, float animationSpeed, AScene &menu);
    void builderFont(std::string name, std::string id, float size, std::string filepath, AScene &menu);
    std::shared_ptr<IEntity> createFont(std::string name, std::string id, float size, std::string filepath);
    std::shared_ptr<IEntity> createText(std::string name, std::string text, Vector2 position, Font font, int fontSize, Color color, int spacing);
    std::shared_ptr<IEntity> createBox(std::string name, Rectangle rec);
    void buildMyPlayer(std::string name, Vector3 position, AScene &game);
    std::shared_ptr<IEntity> createBoxWithText(std::string name, Rectangle rec, std::string text, Font font, int fontSize, Color color, int spacing);
    std::vector<std::shared_ptr<IEntity>> builderHeaderWithButton(std::string name, std::string filepath, std::string filepath2, std::string text, Rectangle position, Font font, float height, float width, float height2, float width2);
    std::map<std::string, std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>>> BuilderParam(std::map<std::string, std::string> params, Rectangle rectangle, Font font, Color textColor, float fontSize);

    // file path : sources/game/scenes/SystemLoginScene.cpp
    void systemLoginScene(std::shared_ptr<IEntity> close, std::shared_ptr<IEntity> usernameTextArea, std::shared_ptr<IEntity> passwordTextArea, std::shared_ptr<IEntity> login);


    // file path : sources/game/scenes/DrawLoginScene.cpp
    void drawTextLoginScene(std::shared_ptr<IEntity> usernameTextArea, std::shared_ptr<IEntity> passwordTextArea);
    void drawTextureAndRecLoginScene(std::shared_ptr<IEntity> backgroundMenu, std::shared_ptr<IEntity> logo, std::shared_ptr<IEntity> close, std::shared_ptr<IEntity> usernameTextArea, std::shared_ptr<IEntity> passwordTextArea, std::shared_ptr<IEntity> login, std::shared_ptr<IEntity> registerBtn);

    // file path : sources/game/scenes/settings/Draws.cpp
    void drawTextInRectangle(std::initializer_list<std::string> texts, Rectangle rectangle, Font font, Color color, float fontSize);
    void drawBindInBox(std::initializer_list<std::string> texts, std::initializer_list<std::string> values, Rectangle rectangle, Font font, Color textColor, Color boxColor, float fontSize);

    // file path : sources/game/Updates.cpp
    void updateCameraPosition(Camera2D *camera, Vector2 playerPosition);

    // file path : sources/game/scenes/systemMenu.cpp
    void systemMenu(_Scene &actualScene, AScene &menu, Vector2 &imagePosition, float &zoomFactor);
    void updateGifMenu(std::shared_ptr<IEntity> backgroundMenu);
    void drawMenu(Texture2D texture, Vector2 imagePosition, float zoomFactor);
    void hoverEffect(std::shared_ptr<IEntity> button);
    void UpdateHoverEffect(std::initializer_list<std::shared_ptr<IEntity>> entities);
    void systemButtonUpdatePlace(_Scene &actualScene, std::initializer_list<std::shared_ptr<IEntity>> entities, _Scene lastScene = _Scene::LOADING);
    void systemModelAnimation(std::shared_ptr<rtype::ECS::Ecs3D::Model3d> model3D, float animationSpeed, int indexAnim, float deltatime);

    bool systemReadyToPlay(std::shared_ptr<IEntity> buttonPlay, std::unique_ptr<rtype::ClientToServer> &_clientToServer, int me, int lobby_id);

    static const std::string GetKeyName(int key, bool onlyLetter = false) {
        std::string Qwerty = "aAmMqQwWzZ;:,./?%*$&'-@<>[]{}";
        std::string Azerty = "QQ,?AAZZWWMM;:&§(_'èù)é./^$¨£";

        std::unordered_map<char, char> QwertyToAzertyMap;

        for (size_t i = 0; i < Azerty.size(); ++i)
            QwertyToAzertyMap[Qwerty[i]] = Azerty[i];

        auto it = inputKeyBoardMap.find(key);
        std::string sString = (it != inputKeyBoardMap.end()) ? it->second : "Unknown";
        if (onlyLetter && sString.size() != 1)
            sString = (char)key;
        std::string result;
        if (sString.size() != 1)
            return sString;
        for (char c : sString) {
            char convertedChar;
            if (QwertyToAzertyMap.count(c)) {
                convertedChar = QwertyToAzertyMap[c];
            } else {
                convertedChar = c;
            }
            result += convertedChar;
        }

        return result;
    }

    static const std::string toLower(const std::string& input) {
        std::string result = input;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }

}

 // file path : sources/game/scenes/SystemLoginScene.cpp
void systemConnexionButton(std::shared_ptr<rtype::ECS::Ecs3D::IEntity> login, std::shared_ptr<rtype::ECS::Ecs3D::IEntity> usernameTextArea, std::shared_ptr<rtype::ECS::Ecs3D::IEntity> passwordTextArea, std::unique_ptr<rtype::ClientToServer> &_clientToServer, _Scene &actualScene);
void systemRegisterButton(std::shared_ptr<rtype::ECS::Ecs3D::IEntity> registerBtn, std::shared_ptr<rtype::ECS::Ecs3D::IEntity> usernameTextArea, std::shared_ptr<rtype::ECS::Ecs3D::IEntity> passwordTextArea, std::unique_ptr<rtype::ClientToServer> &_clientToServer);
