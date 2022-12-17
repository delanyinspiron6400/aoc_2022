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
using VisitCheck = std::bitset<64>;

Input ParseInput(auto && input);
int PressureReleased(const auto & valves);

auto main(int argc, char * argv[]) -> int
{
    std::cout << "*#*#*# AOC 16.12.2022 *#*#*#\n";

    auto valveInfo = ParseInput(std::fstream("../input/aoc16_test.txt"));

    ranges::sort(valveInfo, [](const auto& valve1, const auto& valve2){return std::get<0>(valve1) < std::get<0>(valve2);});

    for(const auto & [valve, flowRate, tunnels] : valveInfo)
    {
        fmt::print("Valve {} has rate {}, connected to ", valve, flowRate);
        for(auto tunnel : tunnels)
            fmt::print("{}, ", tunnel);
        fmt::print("\n");
    }

    fmt::print("Task 1: Most pressure released: {}\n", PressureReleased(valveInfo));

    return 0;
}

auto ValveTranslation(auto & remappingMap, const auto & str) -> int
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

auto ParseInput(auto && input) -> Input
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

int Visit(const auto& valves, int valveIndex, VisitCheck mask, int minutesLeft = 30)
{
    // Decompose tuple
    const auto& [valve, flowRate, tunnels]{valves[valveIndex]};

    if(minutesLeft <= 1)
        return 0;

    auto maxPressure{0};
    if(flowRate && !mask.test(valveIndex))
    {
        auto newMask{mask};
        newMask.set(valveIndex);
        for(auto tunnel : tunnels)
        {
            // fmt::print("Called Visit from {} with {} and {} minutes left and mask {}\n", valveIndex, tunnel, minutesLeft, mask.to_string());
            maxPressure = std::max(maxPressure, (minutesLeft - 1) * flowRate + Visit(valves, tunnel, newMask, minutesLeft - 2));
        }
    }

    for(auto tunnel : tunnels)
    {
        // fmt::print("Called Visit from {} with {} and {} minutes left and mask {}\n", valveIndex, tunnel, minutesLeft, mask.to_string());
            maxPressure = std::max(maxPressure, Visit(valves, tunnel, mask, minutesLeft - 1));
    }

    return maxPressure;
}

int BFS(const auto& valves, int valveIndex)
{
    std::vector<int> bfs_tracker(valves.size(), 0);
    std::queue<std::tuple<int, int, int, VisitCheck>> valveIndices;
    valveIndices.push(std::tuple<int, int, int, VisitCheck>{0, 0, 29, VisitCheck{}});

    while(valveIndices.size())
    {
        const auto& [valveIndex, pressure, minutesLeft, mask]{valveIndices.front()};
        const auto& [valve, flowRate, tunnels]{valves[valveIndex]};

        if(minutesLeft > 1)
        {
            for(auto tunnel : tunnels)
            {
                // Do not open own valve
                if(bfs_tracker[valveIndex] <= pressure)
                {
                    bfs_tracker[valveIndex] = pressure;
                    valveIndices.push(std::tuple<int, int, int, VisitCheck>{tunnel, pressure, minutesLeft - 1, mask});
                }

                // Open own valve
                if(!mask.test(tunnel) && minutesLeft > 2)
                {
                    auto new_mask{mask};
                    new_mask.set(tunnel);
                    auto new_pressure = pressure + (minutesLeft - 2) * flowRate;
                    if(bfs_tracker[valveIndex] <= new_pressure)
                    {
                        bfs_tracker[valveIndex] = new_pressure;
                        valveIndices.push(std::tuple<int, int, int, VisitCheck>{tunnel, new_pressure, minutesLeft - 2, new_mask});
                    }
                }
                
            }
        }

        valveIndices.pop();
    }

    return ranges::max(bfs_tracker);
}

auto PressureReleased(const auto & valves) -> int
{
    fmt::print("Call Visit\n");
    // return Visit(valves, 0, VisitCheck{});
    return BFS(valves, 0);
}
