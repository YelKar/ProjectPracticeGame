#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <regex>
#include "IO.h"


namespace Compilation {
    struct Error {
        int line{};
        int posInLine{};
        std::wstring text;
    };

    std::vector<Error> parseErrors(const std::wstring& input) {
        std::vector<Error> errors;

        std::wregex regex(LR"((\d+):(\d+): ошибка: ([\s\S]*?)(?=^\s*\d+|\z))", std::regex::multiline);
        std::wsmatch match;
        std::wstring::const_iterator searchStart(input.cbegin());

        while (std::regex_search(searchStart, input.cend(), match, regex)) {
            Error error;
            error.line = std::stoi(match[1].str());
            error.posInLine = std::stoi(match[2].str());

            std::wostringstream textStream;
            textStream << match[3].str();
            error.text = textStream.str();

            errors.push_back(error);

            searchStart = match.suffix().first;
        }

        return errors;
    }

    struct CompilationResult {
        bool ok;
        std::wstring text;
    };

    CompilationResult compileAndRun(const std::wstring& code) {
        std::wofstream outFile("output/temp.cpp");
        if (!outFile.is_open()) {
            std::cerr << "Failed to open result file" << std::endl;
            return {
                false, L""
            };
        }

        outFile.write(&code[0], (std::streamsize) code.length());
        outFile.close();
        std::cout << "Code was saved successfully" << std::endl;

        auto result = IO::runCommand("g++ output/temp.cpp &&./a.out");

        std::cout << "RESULT:" << std::endl << result.stdoutOutput << std::endl;
        std::cout << "\n\n\nERROR:" << std::endl << result.stderrOutput << std::endl;
        std::cout << "\n\n\nSTATUS: " << result.exitCode << std::endl;

        if (!result.stdoutOutput.empty()) {
            return {
                true,
                IO::utf8ToWstring(result.stdoutOutput)
            };
        } else {
            return {
                false,
                IO::utf8ToWstring(result.stderrOutput)
            };
        }
    }
};
