cmake_minimum_required(VERSION 3.29)
project(ProjectPracticeGame)

set(CMAKE_CXX_STANDARD 20)

add_executable(ProjectPracticeGame main.cpp
        src/Game.cpp
#        src/Game.h
        src/common/Entity.h
        src/Cursor.h
        src/PlayField.h
        #        src/Ball.cpp
#        src/Ball.h
)

target_link_libraries(ProjectPracticeGame sfml-graphics sfml-window sfml-system)
