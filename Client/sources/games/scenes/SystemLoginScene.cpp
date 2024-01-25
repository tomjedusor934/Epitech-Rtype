#include "../../../includes/MainInclusdes.hpp"

namespace rtype::ECS::Ecs3D {

    // ------------SYSTEMS LOGIN SCENE----------- //

    void systemClose(std::shared_ptr<IEntity> close)
    {
        if (CheckCollisionPointRec(GetMousePosition(), close->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button))
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        else
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), close->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button)) {
            exit(0);
        }
    }

    void systemUsernameTextArea(std::shared_ptr<IEntity> usernameTextArea, std::shared_ptr<IEntity> passwordTextArea)
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), usernameTextArea->getComponent<rtype::ECS::Ecs3D::Rectangle2d>()->rec)) {
            usernameTextArea->getComponent<rtype::ECS::Ecs3D::TextAreaComp>()->isFocus = true;
            passwordTextArea->getComponent<rtype::ECS::Ecs3D::TextAreaComp>()->isFocus = false;
            if (passwordTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->text == "") {
                passwordTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->text = "Password";
            }
            usernameTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->text = "";
        }

        if (usernameTextArea->getComponent<rtype::ECS::Ecs3D::TextAreaComp>()->isFocus) {
            int key = GetKeyPressed();
            if (key != KEY_NULL) {
                if (key == KEY_BACKSPACE) {
                    if (usernameTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->text.size() > 0)
                        usernameTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->text.pop_back();
                } else if (key == KEY_ENTER) {
                    usernameTextArea->getComponent<rtype::ECS::Ecs3D::TextAreaComp>()->isFocus = false;
                } else {
                    if (usernameTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->text.size() < usernameTextArea->getComponent<rtype::ECS::Ecs3D::TextAreaComp>()->nbChar) {
                        usernameTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->text += GetKeyName(key, true);
                        usernameTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->text = toLower(usernameTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->text);
                    }
                }
            }
        }
    }

    void systemPasswordTextArea(std::shared_ptr<IEntity> passwordTextArea, std::shared_ptr<IEntity> usernameTextArea)
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), passwordTextArea->getComponent<rtype::ECS::Ecs3D::Rectangle2d>()->rec)) {
            passwordTextArea->getComponent<rtype::ECS::Ecs3D::TextAreaComp>()->isFocus = true;
            usernameTextArea->getComponent<rtype::ECS::Ecs3D::TextAreaComp>()->isFocus = false;
            if (usernameTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->text == "") {
                usernameTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->text = "Username";
            }
            passwordTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->text = "";
        }

        if (passwordTextArea->getComponent<rtype::ECS::Ecs3D::TextAreaComp>()->isFocus) {
            int key = GetKeyPressed();
            if (key != KEY_NULL) {
                if (key == KEY_BACKSPACE) {
                    if (passwordTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->text.size() > 0)
                        passwordTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->text.pop_back();
                } else if (key == KEY_ENTER) {
                    passwordTextArea->getComponent<rtype::ECS::Ecs3D::TextAreaComp>()->isFocus = false;
                } else {
                    if (passwordTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->text.size() < passwordTextArea->getComponent<rtype::ECS::Ecs3D::TextAreaComp>()->nbChar) {
                        passwordTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->text += GetKeyName(key, true);
                        passwordTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->text = toLower(passwordTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->text);
                    }
                }
            }
        }
    }

    // ------------FUNCTION PRIMORDIAL LOGIN SYSTEMS SCENE----------- //

    //! call all systems for login scene coded above

    void systemLoginScene(std::shared_ptr<IEntity> close, std::shared_ptr<IEntity> usernameTextArea, std::shared_ptr<IEntity> passwordTextArea, std::shared_ptr<IEntity> login)
    {
        systemClose(close);
        systemUsernameTextArea(usernameTextArea, passwordTextArea);
        systemPasswordTextArea(passwordTextArea, usernameTextArea);
    }
}

