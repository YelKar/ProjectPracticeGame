#pragma once


#include <map>
#include <string>
#include <SFML/Graphics/Image.hpp>
#include <memory>
#include <iostream>
#include "Alphabet.h"

const std::string ALPHABET_PATH = "assets/alphabet.png";

class PlayField {
    typedef struct letterImage {
        char letter{};
        sf::Sprite image{};
    } letterImage;

    Alphabet alphabet{"assets/alphabet.png"};
    const int LINE_SPACE = 10;
    int letterWidth = 22;
    int letterHeight = 40;

public:
    std::vector<letterImage> letterImages;

    explicit PlayField(const std::string& code) {
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
                    auto letterNum = Alphabet::LETTERS.find(letter);
                    if (letterNum == std::string::npos) {
                        posInLine++;
                        continue;
                    }
                    auto img = alphabet.get(letter);
                    img.setPosition({
                        static_cast<float>(posInLine * letterWidth),
                        static_cast<float>(line * (letterHeight + LINE_SPACE))}
                    );
                    img.getSprite().setScale({
                        static_cast<float>(letterWidth) / Alphabet::LETTER_WIDTH,
                        static_cast<float>(letterHeight) / Alphabet::LETTER_HEIGHT,
                    });
                    letterImages.push_back(letterImage{
                        .letter = letter,
                        .image = img.getSprite(),
                    });
                    posInLine ++;
            }
        }
    }
};
