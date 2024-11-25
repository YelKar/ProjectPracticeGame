cmake_minimum_required(VERSION 3.29)
project(ProjectPracticeGame)

set(CMAKE_CXX_STANDARD 20)

add_executable(ProjectPracticeGame main.cpp
        PlayField.h
        Alphabet.h
        util/EventManager.h
        util/Button.h
        Menu.h
        GameState.h
        Globals.h
)

target_link_libraries(ProjectPracticeGame sfml-graphics sfml-window sfml-system)
