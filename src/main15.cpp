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

using Sensor = std::pair<int, int>;
using Beacon = std::pair<int, int>;
using Input = std::vector<std::pair<Sensor, Beacon>>;

Input ParseInput(std::istream&& input);

int CountFieldsWithoutBeacon(const Input & input);

int main(int argc, char* argv[])
{
    std::cout << "*#*#*# AOC 15.12.2022 *#*#*#\n";

    auto coords = ParseInput(std::fstream("../input/aoc15.txt"));

    for(auto & [sensor, beacon] : coords)
    {
        auto & [sx, sy]{sensor};
        auto & [bx, by]{beacon};
        fmt::print("{} {} {} {}\n", sx, sy, bx, by);
    }

    fmt::print("Task 1: Number of fields without beacon {}\n",
               CountFieldsWithoutBeacon(coords));
    fmt::print("Task 2: \n");

    return 0;
}

Input ParseInput(std::istream && input)
{
    // using namespace ranges;
    using namespace ranges::views;
    using ranges::getlines;
    using ranges::to;

    auto res = getlines(input) | transform([](auto & view) {
                   return view | split(' ') | transform([](auto && rng) {
                              return std::stoi(
                                  std::string(&*rng.begin(), ranges::distance(rng)));
                          }) |
                          to<std::vector<int>>;
               }) |
               to<std::vector<std::vector<int>>>;

    Input inputParsed;
    for(auto & line : res)
    {
        inputParsed.emplace_back(Sensor{line[0], line[1]}, Beacon{line[2], line[3]});
    }

    return inputParsed;
}

int CountFieldsWithoutBeacon(const Input & input)
{

    return 0;
}
