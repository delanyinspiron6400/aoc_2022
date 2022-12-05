cmake_minimum_required(VERSION 3.18)
project(aoc2022 LANGUAGES CXX VERSION 1.0)

include_directories(include)

set (CMAKE_CXX_STANDARD 23)

add_executable(aoc01
    src/main01.cpp
)

add_executable(aoc02
    src/main02.cpp
)

add_executable(aoc03
    src/main03.cpp
)

add_executable(aoc04
    src/main04.cpp
)

add_executable(aoc05
    src/main05.cpp
)