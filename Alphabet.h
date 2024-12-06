#pragma once

#include <SFML/Graphics/Text.hpp>
#include <memory>
#include "Config.h"


class Alphabet {
    int fontSize = 24;
    sf::Vector2f symbolSize = {15, 30};
public:
    void setFontSize(int newFontSize) {
        fontSize = newFontSize;
    }

    void setSymbolSize(sf::Vector2f size) {
        symbolSize = size;
    }

    sf::Vector2f getSymbolSize() {
        return symbolSize;
    }

    [[nodiscard]] std::unique_ptr<sf::Text> get(wchar_t ch) const {
        auto textShape = std::make_unique<sf::Text>();
        textShape->setCharacterSize(fontSize);
        textShape->setStyle(sf::Text::Bold);
        textShape->setString(std::wstring(1, ch));
        textShape->setFillColor(Config::Workspace::TEXT_COLOR);
        textShape->setFont(Config::font);
        return textShape;
    }
};