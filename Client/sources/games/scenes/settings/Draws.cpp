/*
** EPITECH PROJECT, 2023
** RType
** File description:
** Draws
*/

#include "../../../../includes/MainInclusdes.hpp"

namespace rtype::ECS::Ecs3D {
    void drawTextInRectangle(std::initializer_list<std::string> texts, Rectangle rectangle, Font font, Color color, float fontSize)
    {
        int numTexts = texts.size();
        float totalHeight = rectangle.height - 2.0f * (numTexts - 1);
        float Space = totalHeight / numTexts;

        float x = rectangle.x + 10.0f;
        float y = rectangle.y + 10.0f;

        for (auto text : texts) {
            DrawTextEx(font, text.c_str(), {x, y}, fontSize, 0, color);
            y += Space;
        }
    }

    void drawBindInBox(std::initializer_list<std::string> texts, std::initializer_list<std::string> values, Rectangle rectangle, Font font, Color textColor, Color boxColor, float fontSize)
    {
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
            DrawTextEx(font, text.c_str(), {xText, y}, fontSize, 0, textColor);
            y += space;
        }

        y = rectangle.y + 10.0f;

        for (const auto& value : values) {
            DrawRectangle((int)xValue - 10, (int)y, MeasureText(value.c_str(), fontSize) + 10, (int)fontSize, boxColor);
            DrawTextEx(font, value.c_str(), {xValue, y}, fontSize, 0, textColor);
            y += space;
        }
        // Au lieu de draw, remplir un tableau d'entity avec des textes et des hitboxs
    }
}
