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

#include <range/v3/all.hpp>
#include <fmt/core.h>

size_t countVisibleTrees(const std::vector<std::vector<int>>& treeGrid);
bool isTreeVisible(const std::vector<std::vector<int>>& treeGrid, size_t x, size_t y);
std::vector<std::vector<int>> parseTreeGrid(std::istream&& input);
std::vector<std::vector<int>> ComputeTreeScore(const std::vector<std::vector<int>> treeGrid);

int main(int argc, char* argv[])
{
    std::cout << "*#*#*# AOC 08.12.2022 *#*#*#\n";

    auto treeGrid = parseTreeGrid(std::fstream("../input/aoc08.txt"));

    auto newTreesVisible {countVisibleTrees(treeGrid)};

    auto treeScoreGrid = ComputeTreeScore(treeGrid);
    auto treeScore = ranges::max(treeScoreGrid | ranges::views::join);

    fmt::print("Task 1: Number of Trees visible is: {}\n", newTreesVisible);
    fmt::print("Task 2: Best Score is: {}\n", treeScore);

    return 0;
}

std::vector<std::vector<int>> parseTreeGrid(std::istream&& input)
{
  // using namespace ranges;
  using namespace ranges::views;
  using ranges::getlines;
  using ranges::to;
  return getlines(input) |
         transform([](auto& view) { return view | transform([](char c) { return c - '0'; }) | to<std::vector<int>>; }) |
         to<std::vector<std::vector<int>>>;
}

std::vector<std::vector<int>> ComputeTreeScore(const std::vector<std::vector<int>> treeGrid)
{
    auto treeScoreGrid = treeGrid;

    auto rowsCount{treeGrid.size()};
    auto columnCount{treeGrid[0].size()};

    // Go over grid
    for(auto row = 1; row < rowsCount - 1; ++row)
    {
        for(auto column = 1; column < columnCount - 1; ++column)
        {
            auto currentHeight{treeGrid[row][column]};

            // Go up
            auto up_count{0};
            for(auto rowIt{row - 1}; rowIt >= 0; --rowIt)
            {
                if(currentHeight >= treeGrid[rowIt][column])
                {
                    ++up_count;
                    if(currentHeight == treeGrid[rowIt][column])
                        break;
                }
                else
                {
                    ++up_count;
                    break;
                }
            }

            // Go down
            auto down_count{0};
            for(auto rowIt{row + 1}; rowIt < rowsCount; ++rowIt)
            {
                if(currentHeight >= treeGrid[rowIt][column])
                {
                    ++down_count;
                    if(currentHeight == treeGrid[rowIt][column])
                        break;
                }
                else
                {
                    ++down_count;
                    break;
                }
            }

            // Go left
            auto left_count{0};
            for(auto colIt{column - 1}; colIt >= 0; --colIt)
            {
                if(currentHeight >= treeGrid[row][colIt])
                {
                    ++left_count;
                    if(currentHeight == treeGrid[row][colIt])
                        break;
                }
                else
                {
                    ++left_count;
                    break;
                }
            }

            // Go right
            auto right_count{0};
            for(auto colIt{column + 1}; colIt < columnCount; ++colIt)
            {
                if(currentHeight >= treeGrid[row][colIt])
                {
                    ++right_count;
                    if(currentHeight == treeGrid[row][colIt])
                        break;
                }
                else
                {
                    ++right_count;
                    break;
                }
            }

            treeScoreGrid[row][column] = left_count * right_count * up_count * down_count;
        }
    }


    return treeScoreGrid;
}

bool isTreeVisible(const std::vector<std::vector<int>>& treeGrid, std::size_t x, std::size_t y)
{
  using namespace ranges;
  auto view_left = treeGrid[y] | views::take(x);
  auto view_right = treeGrid[y] | views::drop(x + 1);

  auto view_topdown = treeGrid | views::transform([x](const auto& view) { return view[x]; });
  auto view_top = view_topdown | views::take(y);
  auto view_bottom = view_topdown | views::drop(y + 1);

  auto isSmaller = [compare = treeGrid[y][x]](int h) { return h < compare; };

  return ranges::all_of(view_left, isSmaller) || ranges::all_of(view_right, isSmaller) ||
         ranges::all_of(view_top, isSmaller) || ranges::all_of(view_bottom, isSmaller);
}

size_t countVisibleTrees(const std::vector<std::vector<int>>& treeGrid)
{
  using namespace ranges::views;
  return ranges::accumulate(treeGrid |
    enumerate | 
    transform([&treeGrid](const auto& rows)
    {
        return ranges::count_if(iota(0u, treeGrid[std::get<0>(rows)].size()),
                                [&treeGrid, &rows](auto col_index) 
                                {
                                    return isTreeVisible(treeGrid, col_index, std::get<0>(rows));
                                });
    }),
     0);
}