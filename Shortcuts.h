#pragma once


#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Text.hpp>
#include "Config.h"


struct Shortcut {
    std::wstring keys;
    std::wstring description;
};

class Shortcuts {
    sf::Sprite shape;
    sf::RenderTexture texture;

    sf::Vector2f size = {600, 400};

public:
    Shortcuts() {
        texture.create(static_cast<unsigned>(size.x), static_cast<unsigned>(size.y));
    }

    void updatePosition(sf::Vector2f windowSize, sf::Vector2f camera) {
        shape.setPosition(sf::Vector2f{windowSize.x / 2 - size.x, -size.y / 2} + camera);
    }



    void Render(std::vector<Shortcut>& shortcuts) {
        std::wstring string;

        for (const auto& shortcut : shortcuts) {
            string += shortcut.keys + L" — " + shortcut.description + L"\n";
        }

        texture.clear(Config::Workspace::BACKGROUND_COLOR);

        sf::RectangleShape rect(size - sf::Vector2f{4, 4});
        rect.setPosition({2, 2});
        rect.setOutlineThickness(2);
        rect.setOutlineColor(Config::Workspace::LETTER_FOCUS_STROKE_COLOR);
        rect.setFillColor(Config::Workspace::BACKGROUND_COLOR);
        texture.draw(rect);

        sf::Text text(string, Config::font, 20);
        text.setFillColor(Config::Workspace::SHORTCUTS_COLOR);
        text.setPosition(20, 10);
        texture.draw(text);
        texture.display();
        shape.setTexture(texture.getTexture());
    }

    operator sf::Drawable& () {
        return shape;
    }
};
