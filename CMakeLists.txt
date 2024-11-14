cmake_minimum_required(VERSION 3.29)
project(ProjectPracticeGame)

set(CMAKE_CXX_STANDARD 20)

add_executable(ProjectPracticeGame main.cpp)

target_link_libraries(ProjectPracticeGame sfml-graphics sfml-window sfml-system)
