#pragma once


#include <map>
#include <string>
#include <SFML/Graphics/Image.hpp>
#include <memory>
#include <iostream>
#include "Alphabet.h"
#include "util/Compilation.h"


using namespace Config::Workspace;


class PlayField {
    Alphabet alphabet;
    int LINE_SPACE = 56;
    int letterWidth = 32;
    int letterHeight = 56;
    std::wstring code;

    int focus = -1;
    int height{};
    Compilation::CompilationResult compilationResult = {
        true, L""
    };

public:
    typedef struct letterImage {
        wchar_t letter{};
        sf::Text symbol{};
        sf::FloatRect hitBox{};
        int index{};
    } letterImage;

    std::map<int, std::map<int, letterImage>> letterImages;

    explicit PlayField() {
        alphabet.setFontSize(static_cast<int>(letterHeight * 0.8));
        alphabet.setSymbolSize({static_cast<float>(letterWidth), static_cast<float>(letterHeight)});
    }

    sf::Vector2f getSymbolSize() {
        return alphabet.getSymbolSize();
    }

    void setCode(const std::wstring& code_) {
        code = code_;
    }

    [[nodiscard]] const std::wstring& getCode() const {
        return code;
    }

    [[nodiscard]] int getHeight() const {
        return height;
    }

    void update() {
        for (auto letterImage : letterImages) {
            letterImage.second.clear();
        }
        letterImages.clear();
        int line = 0;
        for (int posInLine = 0, pos = 0; pos < code.length(); pos ++) {
            auto letter = code[pos];

            switch (letter) {
                case ' ':
                    posInLine ++;
                    continue;
                case '\n':
                    posInLine = 0;
                    line++;
                    continue;
                default:
                    auto img = alphabet.get(letter);
                    img->setPosition({
                        static_cast<float>(posInLine * letterWidth),
                        static_cast<float>(line * (letterHeight + LINE_SPACE))}
                    );
                    letterImages[line][posInLine] = letterImage{
                        .letter = letter,
                        .symbol = *img,
                        .hitBox = sf::FloatRect{
                            (sf::Vector2f) img->getPosition() + sf::Vector2f{1, 4},
                            sf::Vector2f{static_cast<float>(letterWidth) - 5, static_cast<float>(letterHeight) - 6}
                        },
                        .index = pos,
                    };
                    posInLine ++;
            }
        }
        height = line * (letterHeight + LINE_SPACE);
    }

    std::shared_ptr<letterImage> intersection(sf::FloatRect rect) {
        for (const auto& line : letterImages) {
            for (const auto& letter : line.second) {
                if (letter.second.hitBox.intersects(rect)) {
                    return std::make_shared<letterImage>(letter.second);
                }
            }
        }
        return nullptr;
    }

    void take(int letterIndex) {
        code = code.substr(0, letterIndex) + code.substr(letterIndex+1, code.length());
    }

    void put(int letterIndex, wchar_t ch) {
        code = code.substr(0, letterIndex) + ch + code.substr(letterIndex, code.length());
    }

    void moveLetter(int letterIndex, int direction = 0) {
        if (code.size() <= letterIndex || abs(direction) > 1 || letterIndex + direction < 0) {
            return;
        }

        if (letterIndex + direction >= code.size() || code[letterIndex + direction] == '\n') {
            code = code.substr(0, letterIndex) + std::wstring(1, ' ') + code.substr(letterIndex, code.size());
        } else {
            std::wcout << "INSERT" << std::endl;
            wchar_t ch = code[letterIndex];
            code = code.substr(0, letterIndex) + code.substr(letterIndex + 1, code.length());
            code = code.insert(letterIndex + direction, std::wstring(1, ch));
        }
    }

    void draw(sf::RenderWindow& window) const {
        window.clear(BACKGROUND_COLOR);
        drawErrors(window);
        auto letters = letterImages;
        for (const auto& line : letters) {
            for (const auto& letter : line.second) {
                if (focus == letter.second.index) {
                    window.draw(PlayField::createFocusShape(letter.second));
                }
                window.draw(letter.second.symbol);
            }
        }
    }

    void drawErrors(sf::RenderWindow& window) const {
        sf::RectangleShape rect({static_cast<float>(letterWidth), static_cast<float>(letterHeight)});
        rect.setFillColor(LETTER_ERROR_BACKGROUND_COLOR);
        rect.setOutlineColor(LETTER_ERROR_STROKE_COLOR);
        rect.setOutlineThickness(2);
        if (!compilationResult.ok) {
            auto errors = Compilation::parseErrors(compilationResult.text);

            for (const auto& error : errors) {
                rect.setPosition({static_cast<float>((error.posInLine - 1) * letterWidth), static_cast<float>((error.line - 1) * (letterHeight + LINE_SPACE))});
                window.draw(rect);
            }
        }
    }

    void setFocus(int index) {
        focus = index;
    }

    void checkErrors() {
        compilationResult = Compilation::compileAndRun(code);
    }

    void hideErrors() {
        compilationResult = {
            true, L""
        };
    }

private:
    static sf::RectangleShape createFocusShape(const letterImage& letter) {
        sf::RectangleShape rect(letter.hitBox.getSize());
        rect.setPosition(letter.hitBox.getPosition());
        rect.setFillColor(LETTER_FOCUS_BACKGROUND_COLOR);
        rect.setOutlineColor(LETTER_FOCUS_STROKE_COLOR);
        rect.setOutlineThickness(1);

        return rect;
    }
};
