#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <ranges>
#include <algorithm>
#include <string_view>
#include <bitset>
#include <memory>
#include <ranges>
#include <unordered_set>

#include <range/v3/all.hpp>
#include <fmt/core.h>

using Input = std::vector<std::vector<int>>;

Input ParseInput(std::istream&& input, std::array<int,2>& startIndex, std::array<int,2>& endIndex);

int main(int argc, char* argv[])
{
    std::cout << "*#*#*# AOC 12.12.2022 *#*#*#\n";

    std::array<int,2> startIndex{0,0};
    std::array<int,2> endIndex{0,0};
    auto heightmap = ParseInput(std::fstream("../input/aoc12.txt"), startIndex, endIndex);

    fmt::print("StartIndex ({}, {}) and EndIndex ({}, {})\n", startIndex[0], startIndex[1], endIndex[0], endIndex[1]);

    // for(auto& line : heightmap)
    // {
    //     for(auto height : line)
    //         std::cout << height << "|";
    //     std::cout << std::endl;
    // }

    fmt::print("Task 1 \n");

    return 0;
}

Input ParseInput(std::istream&& input, std::array<int,2>& startIndex, std::array<int,2>& endIndex)
{
  // using namespace ranges;
  using namespace ranges::views;
  using ranges::getlines;
  using ranges::to;
  return getlines(input) | enumerate |
         transform([&startIndex, &endIndex](auto view) { 
            auto rowIndex{static_cast<int>(std::get<0>(view))};
            return std::get<1>(view) | enumerate | transform([rowIndex, &startIndex, &endIndex](auto item) 
         {
            auto columnIndex{static_cast<int>(std::get<0>(item))};
            auto c{std::get<1>(item)};
            if(c == 'S')
            {
                startIndex = {rowIndex, columnIndex};
                return -1;
            }
            else if (c == 'E')
            {
                endIndex = {rowIndex, columnIndex};
                return -2;
            }
            else
                return c - 'a'; 
        }) | to<std::vector<int>>; }) |
         to<std::vector<std::vector<int>>>;
}
