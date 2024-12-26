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


class Button {
protected:
    sf::Color backgroundColor = sf::Color::Black;
    sf::Color textColor = sf::Color::White;
    sf::Font font;
    std::wstring text;
    int textSize = 20;
    sf::Vector2f insideTextPosition = {0, 0};
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Text::Style textStyle = sf::Text::Style::Bold; sf::Vector2f size;
    sf::Vector2f position;
    sf::Vector2f clickAreaPosition;
public:
    [[nodiscard]] bool isInside(sf::Vector2f pos) const {
        return pos.x >= clickAreaPosition.x &&
               pos.x <= clickAreaPosition.x + size.x &&
               pos.y >= clickAreaPosition.y &&
               pos.y <= clickAreaPosition.y + size.y;
    }

    [[nodiscard]] static bool isInside(const Button& btn, sf::Vector2f pos) {
        return pos.x >= btn.clickAreaPosition.x &&
               pos.x <= btn.clickAreaPosition.x + btn.size.x &&
               pos.y >= btn.clickAreaPosition.y &&
               pos.y <= btn.clickAreaPosition.y + btn.size.y;
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

    Button& setPosition(sf::Vector2f newPos) {
        position = newPos;
        return *this;
    }

    Button& setClickAreaPosition(sf::Vector2f newPos) {
        clickAreaPosition = newPos;
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

    sf::Rect<float> getRect() {
        return {
            this->position,
            this->size
        };
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
        renderTexture.clear(backgroundColor);
        renderTexture.draw(textShape);
        renderTexture.display();

        texture = renderTexture.getTexture();
        sprite.setTexture(texture);
        sprite.setPosition(this->position);
        return *this;
    }

    Button& Connect(EventManager& eventManager, sf::Event::EventType type, const std::function<void(sf::Event&)>& listener) {
        eventManager.AddEventListener(type, [this, listener, type](sf::Event& event) {
            static sf::Vector2f pos;
            listenInside(type, event, pos, listener);
        });
        return *this;
    }

    operator sf::Sprite& () { // NOLINT(*-explicit-constructor)
        return sprite;
    }

private:
    void listenInside(sf::Event::EventType type, sf::Event event, sf::Vector2f &pos, const Listener& listener) const {
        switch (type) {
            case sf::Event::MouseButtonPressed:
                pos = sf::Vector2f{
                    static_cast<float>(event.mouseButton.x),
                    static_cast<float>(event.mouseButton.y),
                };
                break;
            case sf::Event::MouseMoved:
                pos = sf::Vector2f{
                    static_cast<float>(event.mouseMove.x),
                    static_cast<float>(event.mouseMove.y),
                };
                break;
            default:
                return;
        }
        if (this->isInside(pos)) {
            listener(event);
        }
    }
};
