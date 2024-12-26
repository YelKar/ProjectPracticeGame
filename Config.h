#pragma once

#include <SFML/Graphics/Font.hpp>
#include "Sound.h"


namespace Config {
    const std::string FONT_PATH = "assets/Font2.ttf";
    sf::Font font;

    Sounds sounds;

    namespace Workspace {
        const sf::Color BACKGROUND_COLOR = sf::Color(57, 57, 58);
        const sf::Color TEXT_COLOR = sf::Color(86, 174, 161);
        const sf::Color LETTER_FOCUS_BACKGROUND_COLOR = sf::Color(100, 100, 100);
        const sf::Color LETTER_ERROR_BACKGROUND_COLOR = sf::Color(100, 100, 100);
        const sf::Color LETTER_FOCUS_STROKE_COLOR = sf::Color(126, 207, 134);
        const sf::Color LETTER_ERROR_STROKE_COLOR = sf::Color(255, 30, 0);
        const sf::Color TERMINAL_BACKGROUND_COLOR = sf::Color(37, 37, 38);
    }

    namespace Inventory {
        const sf::Color CELL_BACKGROUND_COLOR = sf::Color(57, 57, 58);
        const sf::Color FOCUSED_CELL_BACKGROUND_COLOR = sf::Color(100, 100, 100);
        const sf::Color BORDER_COLOR = sf::Color(150, 150, 150);
        const sf::Color SYMBOL_COLOR = sf::Color(150, 150, 150);
        const sf::Vector2f CELL_SIZE = {40, 40};
        const float CELL_BORDER_SIZE = 2;
    }
}