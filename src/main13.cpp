#include <algorithm>
#include <bitset>
#include <fmt/core.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <queue>
#include <ranges>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

#include <range/v3/all.hpp>

struct List
{
    std::vector<int> items;
    std::vector<List> subLists;
};

using Input = std::vector<std::pair<List, List>>;

Input ParseInput(std::istream&& input);

int main(int argc, char* argv[])
{
    std::cout << "*#*#*# AOC 13.12.2022 *#*#*#\n";

    auto heightmap = ParseInput(std::fstream("../input/aoc13.txt"));

    return 0;
}

Input ParseInput(std::istream&& input)
{
    Input listPairs;
    std::string line;
    while(std::getline(input, line))
    {
        // Find middle
        auto middleIndex{0};
        auto bracketCount{1};
        for(auto [index, c]: ranges::views::enumerate(std::string_view(line.begin() + 1, line.end())) )
        {
            // auto c{std::get<1>(ch)};
            if(c == '[')
                ++bracketCount;
            else if(c == ']')
                --bracketCount;
            
            if(bracketCount == 1)
            {
                middleIndex = index + 1;
                break;
            }
        }

        std::cout << std::string_view(line.begin() + 1, line.begin() + middleIndex) << " ||||||| " << std::string_view(line.begin() + middleIndex + 1, line.end()) << std::endl;

    }
    return listPairs;
}

