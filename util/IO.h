#pragma once


#include <string>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <sys/wait.h>
#include <codecvt>

class IO {
public:
    static std::wstring::size_type getTextFromFile(const std::string& filename, std::wstring &result) {
        std::wifstream inFile(filename);
        if (!inFile.is_open()) {
            return -1;
        }

        inFile.seekg(0, std::ios::end);
        long size = inFile.tellg();
        inFile.seekg(0);

        std::wstring buffer(size, ' ');
        inFile.read(&buffer[0], size);
        inFile.close();
        result = buffer;

        return size;
    }

    struct CompileResult {
        bool ok;
        std::wstring text;
    };

    static CompileResult compileAndRun(const std::wstring& code) {
        std::wofstream outFile("output/temp.c");
        if (!outFile.is_open()) {
            std::cerr << "Failed to open result file" << std::endl;
            return {
                false, L""
            };
        }

        outFile.write(&code[0], (std::streamsize) code.length());
        outFile.close();
        std::cout << "Code was saved successfully" << std::endl;

        auto result = runCommand("./clingCli/cling '#include \"output/temp.c\"' 'main();'");

        std::cout << "RESULT:" << std::endl << result.stdoutOutput << std::endl;
        std::cout << "\n\n\nERROR:" << std::endl << result.stderrOutput << std::endl;
        std::cout << "\n\n\nSTATUS: " << result.exitCode << std::endl;
        if (!result.stdoutOutput.empty()) {
            return {
                true,
                utf8ToWstring(result.stdoutOutput)
            };
        } else {
            return {
                false,
                utf8ToWstring(result.stderrOutput)
            };
        }
    }

    static std::wstring utf8ToWstring(const std::string& utf8Str) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.from_bytes(utf8Str);
    }

    struct CommandResult {
        std::string stdoutOutput;
        std::string stderrOutput;
        int exitCode;
    };

    static CommandResult runCommand(const std::string& command) {
        int stdoutPipe[2];
        int stderrPipe[2];

        if (pipe(stdoutPipe) == -1 || pipe(stderrPipe) == -1) {
            throw std::runtime_error("Failed to create pipes");
        }

        pid_t pid = fork();
        if (pid == -1) {
            throw std::runtime_error("Failed to fork process");
        }

        if (pid == 0) {
            dup2(stdoutPipe[1], STDOUT_FILENO);
            dup2(stderrPipe[1], STDERR_FILENO);

            close(stdoutPipe[0]);
            close(stdoutPipe[1]);
            close(stderrPipe[0]);
            close(stderrPipe[1]);

            execl("/bin/sh", "sh", "-c", command.c_str(), nullptr);
            std::cerr << "Failed to execute command" << std::endl;
            exit(1);
        }

        close(stdoutPipe[1]);
        close(stderrPipe[1]);

        char buffer[128];
        std::string stdoutOutput, stderrOutput;

        ssize_t bytesRead;
        while ((bytesRead = read(stdoutPipe[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytesRead] = '\0';
            stdoutOutput += buffer;
        }

        while ((bytesRead = read(stderrPipe[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytesRead] = '\0';
            stderrOutput += buffer;
        }

        close(stdoutPipe[0]);
        close(stderrPipe[0]);

        int status;
        waitpid(pid, &status, 0);

        return CommandResult{
            stdoutOutput,
            stderrOutput,
            WEXITSTATUS(status)
        };
    }
};
