#include "../../includes/MainInclusdes.hpp"

static std::string delWordInSentence(std::string wordToRemove, std::string sentence)
{
    size_t pos = sentence.find(wordToRemove);
    if (pos != std::string::npos)
        sentence.erase(pos, wordToRemove.length());
    return (sentence);
}

static std::vector<std::string> split(std::string str, std::string sep)
{
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = str.find(sep);

    while (end != std::string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + sep.length();
        end = str.find(sep, start);
    }

    result.push_back(str.substr(start, end));

    return result;
}

namespace rtype::ECS::Ecs3D {
    void builderImg(std::string name, std::string filepathImage, AScene &scene)
    {
        IEntity backgroundMenu(name);
        backgroundMenu.addComponent<ImageComp>(filepathImage, name);
        backgroundMenu.addComponent<Texture2d>(backgroundMenu.getComponent<ImageComp>()->image, name);
        scene.addEntity(std::make_shared<IEntity>(backgroundMenu));
    }

    std::shared_ptr<IEntity> createImg(std::string name, std::string filepathImage)
    {
        IEntity backgroundMenu(name);
        backgroundMenu.addComponent<ImageComp>(filepathImage, name);
        backgroundMenu.addComponent<Texture2d>(backgroundMenu.getComponent<ImageComp>()->image, name);
        return (std::make_shared<IEntity>(backgroundMenu));
    }

    void builderButtonWithImage(std::string name, std::string filepath, Rectangle position, AScene &scene)
    {
        IEntity Button(name);
        Button.addComponent<ImageComp>(filepath, name + "Image");
        Button.addComponent<Texture2d>(Button.getComponent<ImageComp>()->image, name + "Texture");
        Button.addComponent<ButtonMenu>(position, name);
        scene.addEntity(std::make_shared<IEntity>(Button));
    }

    std::shared_ptr<IEntity> createButtonWithImage(std::string name, std::string filepath, Rectangle position)
    {
        IEntity Button(name);
        Button.addComponent<ImageComp>(filepath, name + "Image");
        Button.addComponent<Texture2d>(Button.getComponent<ImageComp>()->image, name + "Texture");
        Button.addComponent<ButtonMenu>(position, name);
        return (std::make_shared<IEntity>(Button));
    }

    void buildTextArea(std::string name, std::string text, Vector2 position, Rectangle rec, AScene &scene, Font font, int fontSize, Color color, int spacing, bool isFocus, int nbChar)
    {
        IEntity textArea(name);
        textArea.addComponent<PositionComponent2d>(position, name);
        textArea.addComponent<Rectangle2d>(name, rec);
        textArea.addComponent<Text>(name, font, text, textArea.getComponent<PositionComponent2d>()->position, fontSize, spacing, color);
        textArea.addComponent<TextAreaComp>(name, isFocus, nbChar);
        scene.addEntity(std::make_shared<IEntity>(textArea));
    }

    std::shared_ptr<IEntity> createTextArea(std::string name, std::string text, Vector2 position, Rectangle rec, Font font, int fontSize, Color color, int spacing, bool isFocus, int nbChar)
    {
        IEntity textArea(name);
        textArea.addComponent<PositionComponent2d>(position, name);
        textArea.addComponent<Rectangle2d>(name, rec);
        textArea.addComponent<Text>(name, font, text, textArea.getComponent<PositionComponent2d>()->position, fontSize, spacing, color);
        textArea.addComponent<TextAreaComp>(name, isFocus, nbChar);
        return (std::make_shared<IEntity>(textArea));
    }

    void builderImgAnim(std::string name, std::string filepathImage, int frameCount, AScene &menu)
    {
        IEntity backgroundMenu(name);
        backgroundMenu.addComponent<ImageAnim>(filepathImage, name, frameCount);
        backgroundMenu.addComponent<Texture2d>(backgroundMenu.getComponent<ImageAnim>()->image, name);
        backgroundMenu.addComponent<GifAnim>();
        menu.addEntity(std::make_shared<IEntity>(backgroundMenu));
    }

    std::shared_ptr<IEntity> createImgAnim(std::string name, std::string filepathImage, int frameCount)
    {
        IEntity backgroundMenu(name);
        backgroundMenu.addComponent<ImageAnim>(filepathImage, name, frameCount);
        backgroundMenu.addComponent<Texture2d>(backgroundMenu.getComponent<ImageAnim>()->image, name);
        backgroundMenu.addComponent<GifAnim>();
        return (std::make_shared<IEntity>(backgroundMenu));
    }

    void builderButtonWithHover(std::string name, std::string filepath, std::string filepathHover, Rectangle position, AScene &menu)
    {
        IEntity Button(name);
        Button.addComponent<ImageComp>(filepath, name + "Image");
        Button.addComponent<Texture2d>(Button.getComponent<ImageComp>()->image, name + "Texture");
        Button.addComponent<ButtonMenu>(position, name);
        menu.addEntity(std::make_shared<IEntity>(Button));
        IEntity ButtonHover(name + "Hover");
        ButtonHover.addComponent<ImageComp>(filepathHover, name + "Imagehover");
        ButtonHover.addComponent<Texture2d>(ButtonHover.getComponent<ImageComp>()->image, name + "Texturehover");
        ButtonHover.addComponent<ButtonMenu>(position, name);
        menu.addEntity(std::make_shared<IEntity>(ButtonHover));
    }

    std::shared_ptr<IEntity> createButtonWithHoverFirstPart(std::string name, std::string filepath, Rectangle position)
    {
        IEntity Button(name);
        Button.addComponent<ImageComp>(filepath, name + "Image");
        Button.addComponent<Texture2d>(Button.getComponent<ImageComp>()->image, name + "Texture");
        Button.addComponent<ButtonMenu>(position, name);
        return (std::make_shared<IEntity>(Button));
    }

    std::shared_ptr<IEntity> createButtonWithHoverSecondPart(std::string name, std::string filepathHover, Rectangle position)
    {
        IEntity ButtonHover(name + "Hover");
        ButtonHover.addComponent<ImageComp>(filepathHover, name + "Imagehover");
        ButtonHover.addComponent<Texture2d>(ButtonHover.getComponent<ImageComp>()->image, name + "Texturehover");
        ButtonHover.addComponent<ButtonMenu>(position, name);
        return (std::make_shared<IEntity>(ButtonHover));
    }

    void builderButtonWithFont(std::string name, Rectangle position, float size, AScene &menu)
    {
        IEntity Button(name);
        Button.addComponent<ButtonMenu>(position, name);
        Button.addComponent<FontSize>(name, size);
        Button.addComponent<InitialFontSize>(name, size);
        menu.addEntity(std::make_shared<IEntity>(Button));
    }

    std::shared_ptr<IEntity> createButtonWithFont(std::string name, Rectangle position, float size)
    {
        IEntity Button(name);
        Button.addComponent<ButtonMenu>(position, name);
        Button.addComponent<FontSize>(name, size);
        Button.addComponent<InitialFontSize>(name, size);
        return (std::make_shared<IEntity>(Button));
    }

    void buildCameraMenu(std::string name, Vector3 position, Vector3 target, Vector3 up, float fov, int projection, AScene &menu)
    {
        IEntity camera(name);
        camera.addComponent<RayLibCameraComponent>(name, position, target, up, fov, projection);
        menu.addEntity(std::make_shared<IEntity>(camera));
    }

    void buildModel3dMenu(std::string name, Model model, int animsCount, unsigned int animCurrentFrame, unsigned int animIndex, std::string filepath, Vector3 scale, Vector3 rotationAxis, float rotationAngle, float animationSpeed, AScene &menu)
    {
        IEntity model3d(name);
        ModelAnimation *modelAnimations = LoadModelAnimations(filepath.c_str(), &animsCount);
        model3d.addComponent<Model3d>(name, model, animsCount, animCurrentFrame, animIndex, modelAnimations, scale, rotationAxis, rotationAngle, animationSpeed);
        menu.addEntity(std::make_shared<IEntity>(model3d));
    }

    void builderFont(std::string name, std::string id, float size, std::string filepath, AScene &menu)
    {
        IEntity FontComp(name);
        FontComp.addComponent<FontComponent>(id, size, filepath);
        menu.addEntity(std::make_shared<IEntity>(FontComp));
    }

    std::shared_ptr<IEntity> createFont(std::string name, std::string id, float size, std::string filepath)
    {
        IEntity FontComp(name);
        FontComp.addComponent<FontComponent>(id, size, filepath);
        return (std::make_shared<IEntity>(FontComp));
    }

    std::shared_ptr<IEntity> createText(std::string name, std::string text, Vector2 position, Font font, int fontSize, Color color, int spacing)
    {
        IEntity textArea(name);
        textArea.addComponent<PositionComponent2d>(position, name);
        textArea.addComponent<Text>(name, font, text, position, fontSize, spacing, color);
        return (std::make_shared<IEntity>(textArea));
    }

    std::shared_ptr<IEntity> createBox(std::string name, Rectangle rec)
    {
        IEntity box(name);
        box.addComponent<Rectangle2d>(name, rec);
        return (std::make_shared<IEntity>(box));
    }

    void buildMyPlayer(std::string name, Vector3 position, AScene &game)
    {
        IEntity player(name);
        player.addComponent<PositionComponent3d>(position, name);
        game.addEntity(std::make_shared<IEntity>(player));
    }

    std::shared_ptr<IEntity> createBoxWithText(std::string name, Rectangle rec, std::string text, Font font, int fontSize, Color color, int spacing)
    {
        IEntity box(name);
        box.addComponent<Rectangle2d>(name + "Rect", rec);
        box.addComponent<Text>(name + "Text", font, text, Vector2{rec.x, rec.y}, fontSize, spacing, color);
        return (std::make_shared<IEntity>(box));
    }

    std::vector<std::shared_ptr<IEntity>> builderHeaderWithButton(std::string name, std::string filepath, std::string filepath2, std::string text, Rectangle position, Font font, float height, float width,  float height2, float width2)
    {
        std::vector<std::shared_ptr<IEntity>> tmp;
        float x_text = (float)MeasureText(text.c_str(), 20) + 20.0f;
        Rectangle rec = {position.x + x_text, position.y, width, height};
        Rectangle rec2 = {rec.x + rec.width + 20, position.y, width2, height2};

        IEntity Button(name);
        Button.addComponent<ImageComp>(filepath, name + "Image");
        Button.addComponent<Texture2d>(Button.getComponent<ImageComp>()->image, name + "Texture");
        Button.addComponent<ButtonMenu>(rec, name);
        Button.addComponent<PositionComponent2d>(Vector2{rec.x, rec.y}, name);
        tmp.push_back(std::make_shared<IEntity>(Button));

        IEntity Button2(name);
        Button2.addComponent<ImageComp>(filepath2, name + "Image");
        Button2.addComponent<Texture2d>(Button2.getComponent<ImageComp>()->image, name + "Texture");
        Button2.addComponent<ButtonMenu>(rec2, name);
        Button2.addComponent<PositionComponent2d>(Vector2{rec2.x, rec2.y}, name);
        tmp.push_back(std::make_shared<IEntity>(Button2));

        IEntity textArea(name + "Text");
        textArea.addComponent<Text>(name, font, text, Vector2{position.x + 10, position.y + (position.height / 2) - 10.0f}, 20, 0, WHITE);
        textArea.addComponent<Rectangle2d>(name, position);
        tmp.push_back(std::make_shared<IEntity>(textArea));

        return (tmp);
    }

    std::map<std::string, std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>>> BuilderParam(std::map<std::string, std::string> params, Rectangle rectangle, Font font, Color textColor, float fontSize)
    {
        std::map<std::string, std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>>> map;
        float maxTextWidth = 0.0f;

        for (const auto& text : params) {
            float textWidth = MeasureText(split(text.first, "/")[0].c_str(), fontSize);
            maxTextWidth = fmax(maxTextWidth, textWidth);
        }

        float totalWidth = 10.0f + maxTextWidth + 20.0f;
        int numValues = params.size();
        float totalHeight = rectangle.height - 2.0f * (numValues - 1);
        float space = totalHeight / numValues;
        float xText = rectangle.x + 10.0f;
        float y = rectangle.y + 10.0f;

        std::string key = "undefined";
        for (const auto& text : params) {
            std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> tmp;

            if (text.second.find("Coche") != std::string::npos) {
                key = "Coche";
                tmp = {
                    rtype::ECS::Ecs3D::createText(text.first, text.first, {xText, y}, font, fontSize, textColor, 0),
                    rtype::ECS::Ecs3D::createBoxWithText(text.first, {xText + maxTextWidth + 20.0f, y + 10.0f, 20, 20}, text.first, font, fontSize, textColor, 0)};
            } else if (text.second.find("JaugeBar") != std::string::npos) {
                key = "JaugeBar";
                tmp = {
                    rtype::ECS::Ecs3D::createText(text.first, text.first, {xText, y}, font, fontSize, textColor, 0),
                    rtype::ECS::Ecs3D::createBox(text.first, {xText + maxTextWidth + 20.0f, y, 300, 40})};
            } else if (text.second.find("Select") != std::string::npos) {
                key = "Select";
                std::vector<std::string> choice = split(text.first, "/");
                float sizeText1 = (float)MeasureText(choice[1].c_str(), fontSize) + 20.0f;
                float sizeText2 = (float)MeasureText(choice[2].c_str(), fontSize) + 20.0f;
                float sizeText3 = (float)MeasureText(choice[3].c_str(), fontSize) + 20.0f;
                if (choice.size() != 4)
                    continue;
                tmp = {
                    rtype::ECS::Ecs3D::createBoxWithText(choice[1], {xText + maxTextWidth + 20.0f, y, sizeText1, 50}, choice[1], font, fontSize, textColor, 0),
                    rtype::ECS::Ecs3D::createBoxWithText(choice[2], {xText + maxTextWidth + 40.0f + sizeText1, y, sizeText2, 50}, choice[2] , font, fontSize, textColor, 0),
                    rtype::ECS::Ecs3D::createBoxWithText(choice[3] + "+", {xText + maxTextWidth + 60.0f + sizeText1 + sizeText2, y, sizeText3, 50}, choice[3], font, fontSize, textColor, 0),
                    rtype::ECS::Ecs3D::createText(choice[0], choice[0], {xText, y + 10.0f}, font, fontSize, textColor, 0)
                };
            } else if (text.second.find("Increment") != std::string::npos) {
                key = "Increment";
                tmp = {
                    rtype::ECS::Ecs3D::createBoxWithText(text.first + "-", {xText + maxTextWidth + 20.0f, y, 50, 50}, "-", font, fontSize, textColor, 0),
                    rtype::ECS::Ecs3D::createBoxWithText(text.first, {xText + maxTextWidth + 80.0f, y, 180, 50}, text.first , font, fontSize, textColor, 0),
                    rtype::ECS::Ecs3D::createBoxWithText(text.first + "+", {xText + maxTextWidth + 270.0f, y, 50, 50}, "+", font, fontSize, textColor, 0),
                    rtype::ECS::Ecs3D::createText(text.first, text.first, {xText, y + 10.0f}, font, fontSize, textColor, 0)
                };
            }

            y += space;

            auto it = map.find(key);
            if (it != map.end()) {
                it->second.insert(it->second.end(), tmp.begin(), tmp.end());
            } else {
                map[key] = tmp;
            }
        }

        return map;
    }
}
