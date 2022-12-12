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

using Input = std::vector<std::vector<int>>;
static constexpr int INVALID_VALUE{-1};
auto neighbours{std::vector<std::array<int, 2>>{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}};

Input ParseInput(std::istream&& input, std::array<int,2>& startIndex, std::array<int,2>& endIndex);

int BFS(const Input & grid, const std::array<int, 2> & startIndex,
        const std::array<int, 2> & endIndex);

std::vector<std::array<int, 2>> FindStartingPositions(const Input & grid);

int main(int argc, char* argv[])
{
    std::cout << "*#*#*# AOC 12.12.2022 *#*#*#\n";

    std::array<int,2> startIndex{0,0};
    std::array<int,2> endIndex{0,0};
    auto heightmap = ParseInput(std::fstream("../input/aoc12.txt"), startIndex, endIndex);

    fmt::print("StartIndex ({}, {}) and EndIndex ({}, {})\n", startIndex[0], startIndex[1], endIndex[0], endIndex[1]);

    auto numSteps{BFS(heightmap, startIndex, endIndex)};

    fmt::print("Task 1 : Number of steps needed: {}\n", numSteps);

    auto startingPositions{FindStartingPositions(heightmap)};
    std::vector<int> routeLenghts;
    for(auto startingPosition : startingPositions)
    {
        auto length{BFS(heightmap, startingPosition, endIndex)};
        if(length != -1)
            routeLenghts.emplace_back(length);
    }

    ranges::sort(routeLenghts);

    fmt::print("Task 2 : Shortest route is: {}\n", routeLenghts[0]);

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
                return 0;
            }
            else if (c == 'E')
            {
                endIndex = {rowIndex, columnIndex};
                return 25;
            }
            else
                return c - 'a'; 
        }) | to<std::vector<int>>; }) |
         to<std::vector<std::vector<int>>>;
}

bool ValidIndex(const Input & grid, const std::array<int, 2> & index)
{
    auto rows{grid.size()};
    auto columns{grid[0].size()};
    if(index[0] >= 0 && index[0] < rows && index[1] >= 0 && index[1] < columns)
        return true;
    else
        return false;
}

int BFS(const Input & grid, const std::array<int, 2> & startIndex,
        const std::array<int, 2> & endIndex)
{
    auto bfsTracker{grid};
    for(auto & line : bfsTracker)
        std::fill(line.begin(), line.end(), 0);

    std::queue<std::array<int, 2>> indices;
    indices.push(startIndex);

    while(indices.size())
    {
        auto currentElement{indices.front()};
        auto currentHeight{grid[currentElement[0]][currentElement[1]]};

        for(auto [row, column] : neighbours)
        {
            auto neighbour = currentElement;
            neighbour[0] += row;
            neighbour[1] += column;
            if(ValidIndex(grid, neighbour))
            {
                auto height{grid[neighbour[0]][neighbour[1]]};
                if(height <= (currentHeight + 1))
                {
                    if(bfsTracker[neighbour[0]][neighbour[1]] == 0)
                    {
                        if(neighbour == endIndex)
                            return bfsTracker[currentElement[0]][currentElement[1]] + 1;
                        bfsTracker[neighbour[0]][neighbour[1]] =
                            bfsTracker[currentElement[0]][currentElement[1]] + 1;
                        indices.push(neighbour);
                    }
                }
            }
        }

        indices.pop();
    }

    return -1;
}

std::vector<std::array<int, 2>> FindStartingPositions(const Input & grid)
{
    // using namespace ranges;
    using namespace ranges::views;
    using ranges::getlines;
    using ranges::to;

    return grid | enumerate | transform([](const auto & line) {
               auto rowIndex{static_cast<int>(std::get<0>(line))};
               return std::get<1>(line) | enumerate |
                      filter([](auto v) { return std::get<1>(v) == 0; }) |
                      transform([rowIndex](auto val) {
                          auto columnIndex{static_cast<int>(std::get<0>(val))};
                          std::array<int, 2> index{rowIndex, columnIndex};
                          return index;
                      }) |
                      to<std::vector<std::array<int, 2>>>;
           }) |
           join | to<std::vector<std::array<int, 2>>>;
}