cmake_minimum_required(VERSION 3.18)
project(aoc2022 LANGUAGES CXX VERSION 1.0)

add_library(common INTERFACE)

# Dependencies from vcpkg
find_package(fmt CONFIG REQUIRED)
find_package(range-v3 CONFIG REQUIRED)
find_package(scn CONFIG REQUIRED)

target_link_libraries(common INTERFACE
  fmt::fmt
  range-v3
  scn::scn
)

set_property(TARGET common PROPERTY CXX_STANDARD 23)

function(add_day_target DAY)
  add_executable(aoc${DAY} src/main${DAY}.cpp)
  target_link_libraries(aoc${DAY} PRIVATE common)
  set_property(TARGET aoc${DAY} PROPERTY CXX_STANDARD 23)
endfunction()

include_directories(include)

set (CMAKE_CXX_STANDARD 23)

add_day_target(01)
add_day_target(02)
add_day_target(03)
add_day_target(04)
add_day_target(05)
add_day_target(06)
add_day_target(07)
add_day_target(08)
add_day_target(09)
add_day_target(10)
add_day_target(11)
add_day_target(12)
add_day_target(13)
add_day_target(14)
add_day_target(15)
add_day_target(16)
add_day_target(17)