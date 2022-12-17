#include <algorithm>
#include <bitset>
#include <cctype>
#include <ctype.h>
#include <fmt/core.h>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <queue>
#include <ranges>
#include <scn/scn.h>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include <range/v3/all.hpp>

enum class Direction
{
    LEFT,
    RIGHT
};

using Input = std::vector<Direction>;

Input ParseInput(auto && input);

auto main(int argc, char * argv[]) -> int
{
    std::cout << "*#*#*# AOC 17.12.2022 *#*#*#\n";

    auto directions = ParseInput(std::fstream("../input/aoc17.txt"));


    return 0;
}


auto ParseInput(auto && input) -> Input
{
    // using namespace ranges;
    using namespace ranges::views;
    using ranges::getlines;
    using ranges::to;

    std::string inputStr;
    std::getline(input, inputStr);
    return c_str(inputStr.c_str()) | transform([](auto c){
        if(c == '<')
            return Direction::LEFT;
        else
            return Direction::RIGHT;}) | to<Input>;
}

