#include <algorithm>
#include <bitset>
#include <cctype>
#include <ctype.h>
#include <fmt/core.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <queue>
#include <ranges>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_set>
#include <variant>
#include <vector>

#include <range/v3/all.hpp>

using Input = std::vector<float>;

Input ParseInput(std::istream&& input);

int main(int argc, char* argv[])
{
    std::cout << "*#*#*# AOC 14.12.2022 *#*#*#\n";

    auto traces = ParseInput(std::fstream("../input/aoc14.txt"));

    return 0;
}

Input ParseInput(std::istream && input)
{
    Input listPairs;
    // std::string line1;
    // while(std::getline(input, line1))
    // {
    //     std::string line2;
    //     std::getline(input, line2);

    //     listPairs.emplace_back(
    //         std::pair<List, List>{GenerateListItem(line1), GenerateListItem(line2)});

    //     std::getline(input, line2); // Empty line
    // }
    return listPairs;
}