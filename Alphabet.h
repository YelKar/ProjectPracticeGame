#pragma once


#include "util/Image.h"




class Alphabet {
    Image letter;
public:
    static const int LETTER_HEIGHT = 20;
    static const int LETTER_WIDTH = 11;
    using letterType = char;
    static std::string LETTERS;

    explicit Alphabet(const std::string& src) : letter(src) {}
    Image get(char ch) {
        auto pos = LETTERS.find(ch);

        letter.setRect({static_cast<int>(LETTER_WIDTH * pos), 0, LETTER_WIDTH, LETTER_HEIGHT});
        return letter;
    }
};

std::string Alphabet::LETTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,!?(){}[]<=>|-_;:'\"/\\%&*#$+`~";
