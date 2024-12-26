cmake_minimum_required(VERSION 3.29)
project(ProjectPracticeGame)

set(CMAKE_CXX_STANDARD 20)

set(CLING_ROOT "/home/yel/Programming/Cling/cling")
include_directories(${CLING_ROOT}/include)
link_directories(${CLING_ROOT}/lib)

add_executable(ProjectPracticeGame main.cpp
        PlayField.h
        util/EventManager.h
        util/Button.h
        Menu.h
        GameState.h
        Config.h
        Alphabet.h
        Player.h
        Inventory.h
        util/IO.h
        Game.h
        GameOverScreen.h
        Sound.h
        util/Compilation.h
        Bonus.h
        ErrorPlace.h
)

target_link_libraries(ProjectPracticeGame sfml-graphics sfml-window sfml-system sfml-audio LLVM)
