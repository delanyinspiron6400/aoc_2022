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
int64_t FindPossibleLocation(const Input & input);

int main(int argc, char* argv[])
{
    std::cout << "*#*#*# AOC 15.12.2022 *#*#*#\n";

    auto coords = ParseInput(std::fstream("../input/aoc15.txt"));

    // for(auto & [sensor, beacon] : coords)
    // {
    //     auto & [sx, sy]{sensor};
    //     auto & [bx, by]{beacon};
    //     fmt::print("{} {} {} {}\n", sx, sy, bx, by);
    // }

    fmt::print("Task 1: Number of fields without beacon {}\n",
               CountFieldsWithoutBeacon(coords));
    fmt::print("Task 2: Possible location {}\n", FindPossibleLocation(coords));

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

int ManhattanDistance(const Sensor& s, const Beacon& b)
{
    return std::abs(s.first - b.first) + std::abs(s.second - b.second);
}

int CountFieldsWithoutBeacon(const Input & input)
{
    static constexpr int rowIndex{2000000};
    return static_cast<int>(
        ranges::accumulate(ranges::views::all(input),
                           std::unordered_set<int>(),
                           [](auto & set, auto pair) {
                               const auto & [sensor, beacon]{pair};
                               const auto & [sX, sY]{sensor};
                               auto reach = ManhattanDistance(sensor, beacon);

                               int start = sX - reach + std::abs(sY - rowIndex);
                               int end = sX + reach - std::abs(sY - rowIndex);
                               for(int x = start; x <= end; x++)
                               {
                                   Beacon testPosition{x, rowIndex};
                                   if(testPosition != beacon)
                                       set.insert(x);
                               }
                               return set;
                           })
            .size());
}

int64_t computeTuningFrequency(const Beacon & beacon)
{
    return static_cast<int64_t>(beacon.first) * 4000000LL +
           static_cast<int64_t>(beacon.second);
}

int64_t FindPossibleLocation(const Input & input)
{
    for(auto row{0}; row < 4000000; ++row)
    {
        for(auto column{0}; column <= 4000000;)
        {
            bool possible{true};
            auto pos = std::make_pair(column, row);
            for(auto & [sensor, beacon] : input)
            {
                auto reach = ManhattanDistance(sensor, beacon);
                auto distance = ManhattanDistance(sensor, pos);
                const auto & [sX, sY]{sensor};
                if(reach >= distance)
                {
                    // Shift to the end of this sensor
                    column = sX + reach - std::abs(sY - row) + 1;
                    possible = false;
                    break;
                }
            }
            if(possible)
                return computeTuningFrequency({column, row});
        }
    }
    return std::numeric_limits<int64_t>::max();
}