#pragma once


#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <string>
#include <utility>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <memory>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <iostream>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "EventManager.h"

class BaseButton {
protected:
    sf::Vector2f size;
    sf::Vector2f position;
public:
    [[nodiscard]] bool isInside(sf::Vector2f pos) const {
        return pos.x >= position.x &&
               pos.x <= position.x + size.x &&
               pos.y >= position.y &&
               pos.y <= position.y + size.y;
    }

    [[nodiscard]] static bool isInside(BaseButton btn, sf::Vector2f pos) {
        return pos.x >= btn.position.x &&
               pos.x <= btn.position.x + btn.size.x &&
               pos.y >= btn.position.y &&
               pos.y <= btn.position.y + btn.size.y;
    }

    [[nodiscard]] static bool isInside(sf::FloatRect rect, sf::Vector2f pos) {
        return pos.x >= static_cast<float>(rect.left) &&
               pos.x <= static_cast<float>(rect.left) + static_cast<float>(rect.width) &&
               pos.y >= static_cast<float>(rect.top) &&
               pos.y <= static_cast<float>(rect.top) + static_cast<float>(rect.width);
    }

    sf::Vector2f getPosition() {
        return position;
    }

    sf::Vector2f getSize() {
        return size;
    }
};

class Button : public BaseButton {
protected:
    sf::Color backgroundColor = sf::Color::Black;
    sf::Color textColor = sf::Color::White;
    sf::Font font;
    std::wstring text;
    int textSize = 20;
    sf::Vector2f insideTextPosition = {0, 0};
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Text::Style textStyle = sf::Text::Style::Bold;
public:
    Button& setPosition(sf::Vector2f newPos) {
        position = newPos;
        return *this;
    }

    Button& setSize(sf::Vector2f newSize) {
        this->size = newSize;
        return *this;
    }

    Button& setBackgroundColor(sf::Color color) {
        backgroundColor = color;
        return *this;
    }

    [[nodiscard]] sf::Color getBackgroundColor() const {
        return backgroundColor;
    }

    Button& setTextColor(sf::Color color) {
        textColor = color;
        return *this;
    }

    [[nodiscard]] sf::Color getTextColor() const {
        return textColor;
    }

    Button& setText(const std::wstring& newText) {
        text = newText;
        return *this;
    }

    [[nodiscard]] const std::wstring& getText() const {
        return text;
    }

    Button& setTextSize(int newTextSize) {
        textSize = newTextSize;
        return *this;
    }

    [[nodiscard]] int getTextSize() const {
        return textSize;
    }

    Button& setFont(const sf::Font& newFont) {
        font = newFont;
        return *this;
    }

    [[nodiscard]] const sf::Font& getFont() {
        return font;
    }

    Button& setInsideTextPosition(sf::Vector2f newInsideTextPosition) {
        insideTextPosition = newInsideTextPosition;
        return *this;
    }

    [[nodiscard]] sf::Vector2f getInsideTextPosition() {
        return insideTextPosition;
    }

    Button& Render() {
        sf::Text textShape(text, font, textSize);
        textShape.setStyle(textStyle);
        textShape.setFillColor(textColor);
        textShape.setPosition(insideTextPosition);

        sf::RectangleShape rectangle(size);
        rectangle.setFillColor(backgroundColor);
        rectangle.setPosition(0, 0);

        sf::RenderTexture renderTexture;
        renderTexture.create(static_cast<unsigned>(size.x), static_cast<unsigned>(size.y));
        renderTexture.clear(sf::Color::Transparent);
        renderTexture.draw(rectangle);
        renderTexture.draw(textShape);
        renderTexture.display();

        texture = renderTexture.getTexture();
        sprite.setTexture(texture);
        sprite.setPosition(this->position);
        return *this;
    }

    Button& Connect(EventManager& eventManager, sf::Event::EventType type, const std::function<void(sf::Event&)>& listener) {
        sf::Rect<float> btnRect(
            this->position,
            this->size
        );
        eventManager.AddEventListener(type, [btnRect, listener](sf::Event& event) {
            std::cout << "listener\n";
            if (BaseButton::isInside(btnRect, {static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)})) {
                listener(event);
                std::cout << "inside\n";
            };
        });
        return *this;
    }

    operator sf::Sprite& () { // NOLINT(*-explicit-constructor)
        return sprite;
    }
};
