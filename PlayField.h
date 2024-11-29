#pragma once


#include <map>
#include <string>
#include <SFML/Graphics/Image.hpp>
#include <memory>
#include <iostream>
#include "Alphabet.h"

class PlayField {
    typedef struct letterImage {
        wchar_t letter{};
        sf::Text symbol{};
        sf::FloatRect hitBox{};
    } letterImage;
    Alphabet alphabet;
    int LINE_SPACE = 30;
    int letterWidth = 22;
    int letterHeight = 40;
    std::wstring code;

public:
    std::map<int, std::map<int, letterImage>> letterImages;

    explicit PlayField() {
        alphabet.setFontSize(30);
        alphabet.setSymbolSize({static_cast<float>(letterWidth), static_cast<float>(letterHeight)});
    }

    sf::Vector2f getSymbolSize() {
        return alphabet.getSymbolSize();
    }

    void setCode(const std::wstring& code_) {
        code = code_;
    }

    void update() {
        for (auto letterImage : letterImages) {
            letterImage.second.clear();
        }
        letterImages.clear();
        for (int line = 0, posInLine = 0, pos = 0; pos < code.length(); pos ++) {
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
                    };
                    posInLine ++;
            }
        }
    }

    bool intersects(sf::FloatRect rect) {
        for (const auto& line : letterImages) {
            for (const auto& letter : line.second) {
                if (letter.second.hitBox.intersects(rect)) {
                    return true;
                }
            }
        }
        return false;
    }
};
