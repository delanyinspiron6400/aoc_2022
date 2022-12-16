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

using Valve = int;
using FlowRate = int;
using Tunnels = std::vector<Valve>;
using Input = std::vector<std::tuple<Valve, FlowRate, Tunnels>>;

Input ParseInput(std::istream && input);

int CountFieldsWithoutBeacon(const Input & input);
int64_t FindPossibleLocation(const Input & input);

int main(int argc, char * argv[])
{
    std::cout << "*#*#*# AOC 16.12.2022 *#*#*#\n";

    auto valveInfo = ParseInput(std::fstream("../input/aoc16_test.txt"));

    for(const auto & [valve, flowRate, tunnels] : valveInfo)
    {
        fmt::print("Valve {} has rate {}, connected to ", valve, flowRate);
        for(auto tunnel : tunnels)
            fmt::print("{}, ", tunnel);
        fmt::print("\n");
    }

    return 0;
}

int ValveTranslation(auto & remappingMap, const std::string & str)
{
    if(remappingMap.find(str) != remappingMap.end())
    {
        return remappingMap[str];
    }
    else
    {
        int index{static_cast<int>(std::ssize(remappingMap))};
        remappingMap[str] = index;
        return index;
    }
}

Input ParseInput(std::istream && input)
{
    // using namespace ranges;
    using namespace ranges::views;
    using ranges::getlines;
    using ranges::to;

    std::unordered_map<std::string, int> remappingMap;

    return getlines(input) | transform([&remappingMap](auto & view) {
               std::string valveStr;
               FlowRate rate{};
               auto result = scn::scan(view, "{} {}", valveStr, rate);

               Valve valve{ValveTranslation(remappingMap, valveStr)};
               Tunnels tunnels =
                   view | split(' ') | drop(2) | transform([&remappingMap](auto in) {
                       return Valve{
                           ValveTranslation(remappingMap, std::string(&*in.begin(), 2))};
                   }) |
                   to<Tunnels>;

               return std::tuple<Valve, FlowRate, Tunnels>{valve, rate, tunnels};
           }) |
           to<Input>;
}
