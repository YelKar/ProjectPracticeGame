#include <iostream>
#include <vector>
#include <regex>
#include <string>
#include <sstream>

struct Error {
    int line;
    int posInLine;
    std::wstring text;
};

std::vector<Error> parseErrors(const std::wstring& input) {
    std::vector<Error> errors;

    // Регулярное выражение для строки ошибки
    std::wregex regex(LR"((\d+):(\d+): ошибка: ([\s\S]*?)(?=^\s*\d+|\z))", std::regex::multiline);
    std::wsmatch match;
    std::wstring::const_iterator searchStart(input.cbegin());

    while (std::regex_search(searchStart, input.cend(), match, regex)) {
        Error error;
        error.line = std::stoi(match[1].str());
        error.posInLine = std::stoi(match[2].str());

        // Текст ошибки (включая многословные части)
        std::wostringstream textStream;
        textStream << match[3].str();
        error.text = textStream.str();

        errors.push_back(error);

        // Продолжаем поиск с позиции после текущего совпадения
        searchStart = match.suffix().first;
    }

    return errors;
}

int main() {
    // Пример ввода
    std::wstring log = LR"(
./input/test.cpp: In function «int main()»:
output/temp.cpp:7:14: ошибка: «cou» is not a member of «std»; did you mean «cout»?
    7 |         std::cou << "Muultiplicsation table by " << i << std::endl;
      |              ^~~
      |              cout
output/temp.cpp:9:18: ошибка: «cut» is not a member of «std»; did you mean «cout»?
    9 |             std::cut << j << " x " << i << " = " << i * j << std::endl;
      |                  ^~~
      |                  cout
)";
    std::vector<Error> errors = parseErrors(log);

    // Вывод результата
    for (const auto& error : errors) {
        std::wcout << L"Line: " << error.line << L", Pos: " << error.posInLine << L"\n";
        std::wcout << L"Text: \n" << error.text << L"\n";
    }

    return 0;
}
