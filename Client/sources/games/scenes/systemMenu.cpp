#include "../../../includes/MainInclusdes.hpp"

namespace rtype::ECS::Ecs3D {
    // ------- DRAW MENU ----------- //
    void drawMenu(Texture2D texture, Vector2 imagePosition, float zoomFactor)
    {
        ClearBackground(BLACK);

        Vector2 texturePosition = { 1920 / 100.0f + imagePosition.x, 1080 / 100.0f + imagePosition.y };
        DrawTextureEx(texture, texturePosition, 0, zoomFactor, WHITE);
    }


    // ------- DRAW BACKGROUND MENU ----------- //
    void updateGifMenu(std::shared_ptr<IEntity> backgroundMenu)
    {
        backgroundMenu->getComponent<GifAnim>()->frameCounter++;
        if (backgroundMenu->getComponent<GifAnim>()->frameCounter >= backgroundMenu->getComponent<GifAnim>()->frameDelay) {
            // Move to next frame
            // NOTE: If final frame is reached we return to first frame
            backgroundMenu->getComponent<GifAnim>()->currentAnimFrame++;
            if (backgroundMenu->getComponent<GifAnim>()->currentAnimFrame >= backgroundMenu->getComponent<ImageAnim>()->animFrames)
                backgroundMenu->getComponent<GifAnim>()->currentAnimFrame = 0;

            // Get memory offset position for next frame data in image.data
            backgroundMenu->getComponent<GifAnim>()->nextFrameDataOffset = backgroundMenu->getComponent<ImageAnim>()->image.width * backgroundMenu->getComponent<ImageAnim>()->image.height * 4 * backgroundMenu->getComponent<GifAnim>()->currentAnimFrame;

            // Update GPU texture data with next frame image data
            // WARNING: Data size (frame size) and pixel format must match already created texture
            UpdateTexture(backgroundMenu->getComponent<Texture2d>()->texture, ((unsigned char *)backgroundMenu->getComponent<ImageAnim>()->image.data) + backgroundMenu->getComponent<GifAnim>()->nextFrameDataOffset);

            backgroundMenu->getComponent<GifAnim>()->frameCounter = 0;
        }
    }

    // ------- CLICK AND HOVER BUTTON MENU ----------- //
    void systemMenu(_Scene &actualScene, AScene &menu, Vector2 &imagePosition, float &zoomFactor)
    {
        Vector2 mousePoint = GetMousePosition();

        for (auto &entity : menu.getEntities()) {
            if (entity.second->getComponent<ButtonMenu>() != nullptr) {
                auto button = entity.second->getComponent<ButtonMenu>();
                if (CheckCollisionPointRec(mousePoint, button->button)) {
                    // Changer l'état du bouton au survol
                    button->isHover = true;

                    // Action lorsque le bouton Play est cliqué
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        if (button->id == "PlayButton" && actualScene == _Scene::LOBBY) {
                            actualScene = _Scene::LOADING;
                        } else if (button->id == "ExitButton" && actualScene == _Scene::LOBBY) {
                            CloseWindow();
                            std::exit(EXIT_SUCCESS);
                        } else if (button->id == "SettingsButton" && actualScene == _Scene::LOBBY) {
                            actualScene = _Scene::VIDEOSETTINGS;
                        }
                    }
                } else {
                    // Revenir à l'état normal lorsque la souris n'est pas au-dessus du bouton
                    button->isHover = false;
                }

                // Mise à jour de la position initiale lors de la première frame
                if (GetFrameTime() == 0.0f) {
                    button->initialPosition = { button->button.x, button->button.y };
                }
            }
        }


        Vector2 mousePosition = GetMousePosition();
        imagePosition.x += (mousePosition.x - imagePosition.x) * 0.03f;
        imagePosition.y += (mousePosition.y - imagePosition.y) * 0.03f;

        zoomFactor += (1.0f - zoomFactor) * 0.1f;
    }


    // ------- HOVER EFFECT ----------- //
    void hoverEffect(std::shared_ptr<IEntity> button)
    {
        auto initialFontSize = button->getComponent<InitialFontSize>();
        auto fontSize = button->getComponent<FontSize>();
        auto buttonMenu = button->getComponent<ButtonMenu>();

        if (CheckCollisionPointRec(GetMousePosition(), buttonMenu->button)) {
            fontSize->size += 0.5f;
            if (fontSize->size > initialFontSize->initial_size + 5.0f) {
                fontSize->size = initialFontSize->initial_size + 5.0f;
            }
            buttonMenu->button.x = buttonMenu->initialPosition.x - (fontSize->size - initialFontSize->initial_size) * 0.5f;
            buttonMenu->button.y = buttonMenu->initialPosition.y - (fontSize->size - initialFontSize->initial_size) * 0.5f;
        } else {
            fontSize->size -= 0.5f;
            if (fontSize->size < initialFontSize->initial_size) {
                fontSize->size = initialFontSize->initial_size;
            }
            buttonMenu->button.x = buttonMenu->initialPosition.x - (fontSize->size - initialFontSize->initial_size) * 0.5f;
            buttonMenu->button.y = buttonMenu->initialPosition.y - (fontSize->size - initialFontSize->initial_size) * 0.5f;
        }
    }

    // ------- UPDATE HOVER EFFECT ----------- //
    void UpdateHoverEffect(std::initializer_list<std::shared_ptr<IEntity>> entities)
    {
        for (const auto& entity : entities) {
            hoverEffect(entity);
        }
    }

    void systemButtonUpdatePlace(_Scene &actualScene, std::initializer_list<std::shared_ptr<IEntity>> entities, _Scene lastScene)
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            for (const auto& entity : entities) {
                auto button = entity->getComponent<ButtonMenu>();
                if (CheckCollisionPointRec(GetMousePosition(), button->button)) {
                    if (button->id == "Settings") {
                        actualScene = _Scene::VIDEOSETTINGS;
                    } else if (button->id == "Career") {
                        actualScene = _Scene::CAREER;
                    } else if (button->id == "Lobby") {
                        actualScene = _Scene::LOBBY;
                    } else if (button->id == "Store") {
                        actualScene = _Scene::STORE;
                    } else if (button->id == "Collection") {
                        actualScene = _Scene::COLLECTION;
                    } else if (button->id == "VideoSettings") {
                        if (lastScene == _Scene::GAME)
                            actualScene = _Scene::VIDEOSETTINGSGAME;
                        else
                            actualScene = _Scene::VIDEOSETTINGS;
                    } else if (button->id == "ControlsSettings") {
                        if (lastScene == _Scene::GAME)
                            actualScene = _Scene::CONTROLSSETTINGSGAME;
                        else
                            actualScene = _Scene::CONTROLSSETTINGS;
                    } else if (button->id == "AudioSettings") {
                        if (lastScene == _Scene::GAME)
                            actualScene = _Scene::AUDIOSETTINGSGAME;
                        else
                            actualScene = _Scene::AUDIOSETTINGS;
                    } else if (button->id == "ProfilSettings") {
                        if (lastScene == _Scene::GAME)
                            actualScene = _Scene::PROFILSETTINGSGAME;
                        else
                            actualScene = _Scene::PROFILSETTINGS;
                    } else if (button->id == "MouseSettings") {
                        if (lastScene == _Scene::GAME)
                            actualScene = _Scene::MOUSESETTINGSGAME;
                        else
                            actualScene = _Scene::MOUSESETTINGS;
                    } else if (button->id == "SettingsGame") {
                        actualScene = _Scene::VIDEOSETTINGSGAME;
                    }
                }
            }
        }
    }

    void systemModelAnimation(std::shared_ptr<rtype::ECS::Ecs3D::Model3d> model3D, float animationSpeed, int indexAnim, float deltatime)
    {
        float totalAnimationTime = static_cast<float>(model3D->modelAnimations[indexAnim].frameCount);

        model3D->animationTime += deltatime;

        float frameRate = animationSpeed * totalAnimationTime / static_cast<float>(model3D->modelAnimations[indexAnim].frameCount);
        float frame = fmod(model3D->animationTime * frameRate, static_cast<float>(model3D->modelAnimations[indexAnim].frameCount));

        model3D->animCurrentFrame = static_cast<int>(frame);

        if (model3D->animationTime >= totalAnimationTime) {
            model3D->animationTime = 0.0f;
            model3D->animCurrentFrame = 0;
        }
        UpdateModelAnimation(model3D->model, model3D->modelAnimations[indexAnim], model3D->animCurrentFrame);
    }
}