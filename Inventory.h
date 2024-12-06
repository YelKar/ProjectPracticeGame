#pragma once


#include <array>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include "Config.h"


using namespace Config::Inventory;

class Inventory {
    std::array<wchar_t, 8> letters{};
    int selection = 0;
    sf::RenderTexture texture;
    sf::Sprite shape;

public:
    Inventory() {
        texture.create(40 * letters.size(), 40);
    }

    wchar_t toggle(wchar_t ch) {
        wchar_t result = letters[selection];
        letters[selection] = ch;
        return result;
    }

    wchar_t toggle() {
        wchar_t result = letters[selection];
        letters[selection] = 0;
        return result;
    }

    void forward() {
        selection = std::min(selection + 1, (int) letters.max_size() - 1);
    }

    void backward() {
        selection = std::max(selection - 1, 0);
    }

    operator sf::Drawable&() {
        texture.clear();

        sf::RectangleShape cell(
            CELL_SIZE
            - sf::Vector2f{CELL_BORDER_SIZE, CELL_BORDER_SIZE} * 2.f
        );
        cell.setOutlineColor(BORDER_COLOR);
//        cell.setOutlineThickness(CELL_BORDER_SIZE);

        sf::Text text;
        text.setFillColor(SYMBOL_COLOR);
        text.setFont(Config::font);

        for (int i = 0; i < letters.max_size(); i ++) {
            if (i == selection) {
                cell.setFillColor(FOCUSED_CELL_BACKGROUND_COLOR);
            } else {
                cell.setFillColor(CELL_BACKGROUND_COLOR);
            }

            cell.setPosition({static_cast<float>(i) * 40.f + 2, 2});
            texture.draw(cell);

            if (!letters[i]) {
                continue;
            }

            text.setString(std::wstring(1, letters[i]));
            auto letter = text.getLocalBounds();

            text.setPosition(
                sf::Vector2f{static_cast<float>(i) * CELL_SIZE.x + (CELL_SIZE.x - letter.width) / 2.f, 0}
                - sf::Vector2f{letter.getPosition().x, 0}
            );
            texture.draw(text);
        }

        texture.display();

        shape.setTexture(texture.getTexture());
        return shape;
    }
};
