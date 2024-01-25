#include "../../../includes/MainInclusdes.hpp"

namespace rtype::ECS::Ecs3D {
    void drawTextLoginScene(std::shared_ptr<IEntity> usernameTextArea, std::shared_ptr<IEntity> passwordTextArea)
    {
        DrawTextEx(usernameTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->font, "Enter username", {usernameTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->position.x, usernameTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->position.y - 25.0f}, 20, usernameTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->spacing, usernameTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->tint);
        DrawTextEx(usernameTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->font, usernameTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->text.c_str(), {usernameTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->position.x + 10, usernameTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->position.y + 8}, usernameTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->fontSize, usernameTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->spacing, usernameTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->tint);
        DrawTextEx(passwordTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->font, "Enter password", {passwordTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->position.x, passwordTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->position.y - 25.0f}, 20, passwordTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->spacing, passwordTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->tint);

        std::string passwordNonVisible = "";
        if (passwordTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->text != "Password") {
            for (int i = 0; i < passwordTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->text.size(); i++)
                passwordNonVisible += "*";
            DrawTextEx(passwordTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->font, passwordNonVisible.c_str(), {passwordTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->position.x + 10, passwordTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->position.y + 8}, passwordTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->fontSize, passwordTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->spacing, passwordTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->tint);
        } else {
            DrawTextEx(passwordTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->font, passwordTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->text.c_str(), {passwordTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->position.x + 10, passwordTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->position.y + 8}, passwordTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->fontSize, passwordTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->spacing, passwordTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->tint);
        }
    }

    void drawTextureAndRecLoginScene(std::shared_ptr<IEntity> backgroundMenu, std::shared_ptr<IEntity> logo, std::shared_ptr<IEntity> close, std::shared_ptr<IEntity> usernameTextArea, std::shared_ptr<IEntity> passwordTextArea, std::shared_ptr<IEntity> login, std::shared_ptr<IEntity> registerBtn)
    {
        DrawTexture(backgroundMenu->getComponent<rtype::ECS::Ecs3D::Texture2d>()->texture, 0, 0, WHITE);
        DrawRectangle(0, 0, 400, 720, WHITE);
        DrawTextureEx(logo->getComponent<rtype::ECS::Ecs3D::Texture2d>()->texture, {70.0f, 20.0f}, 0, 0.5, WHITE);

        DrawRectangle(close->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button.x + 1, close->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button.y + 1, close->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button.width, close->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button.height, WHITE);
        DrawTexture(close->getComponent<rtype::ECS::Ecs3D::Texture2d>()->texture, close->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button.x, close->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button.y, WHITE);

        DrawRectangleLines(usernameTextArea->getComponent<rtype::ECS::Ecs3D::Rectangle2d>()->rec.x, usernameTextArea->getComponent<rtype::ECS::Ecs3D::Rectangle2d>()->rec.y, usernameTextArea->getComponent<rtype::ECS::Ecs3D::Rectangle2d>()->rec.width, usernameTextArea->getComponent<rtype::ECS::Ecs3D::Rectangle2d>()->rec.height, {200, 200, 200, 255});
        DrawRectangleLines(passwordTextArea->getComponent<rtype::ECS::Ecs3D::Rectangle2d>()->rec.x, passwordTextArea->getComponent<rtype::ECS::Ecs3D::Rectangle2d>()->rec.y, passwordTextArea->getComponent<rtype::ECS::Ecs3D::Rectangle2d>()->rec.width, passwordTextArea->getComponent<rtype::ECS::Ecs3D::Rectangle2d>()->rec.height, {200, 200, 200, 255});
    
        // DrawRectangle(login->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button.x, login->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button.y, login->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button.width, login->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button.height, RED);
        DrawTextureEx(login->getComponent<rtype::ECS::Ecs3D::Texture2d>()->texture, {login->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button.x, login->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button.y}, 0, 0.15, WHITE);
        DrawTextureEx(registerBtn->getComponent<rtype::ECS::Ecs3D::Texture2d>()->texture, {registerBtn->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button.x, registerBtn->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button.y}, 0, 0.15, WHITE);
    }
}