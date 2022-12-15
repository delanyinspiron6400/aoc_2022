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
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include <range/v3/all.hpp>

using Input = std::vector<std::vector<int>>;

Input ParseInput(std::istream&& input);

int main(int argc, char* argv[])
{
    std::cout << "*#*#*# AOC 15.12.2022 *#*#*#\n";

    auto coords = ParseInput(std::fstream("../input/aoc15.txt"));

    for(auto& line : coords)
    {
        fmt::print("{} {} {} {}\n", line[0], line[1], line[2], line[3]);
    }

    return 0;
}

Input ParseInput(std::istream && input)
{
    // using namespace ranges;
    using namespace ranges::views;
    using ranges::getlines;
    using ranges::to;

    return getlines(input) |
         transform([](auto& view) { return view | split(' ') | 
        transform([](auto && rng) 
            {
                return std::stoi(std::string(
                    &*rng.begin(), ranges::distance(rng)));
            }) | to<std::vector<int>>; }) |
         to<std::vector<std::vector<int>>>;
}
