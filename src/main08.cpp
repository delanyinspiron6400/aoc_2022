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

std::vector<std::vector<int>> MarkTreesVisible(const std::vector<std::vector<int>> treeGrid)
{
    auto visibleGrid = treeGrid;
    // Set to zero
    for(auto& treeLine : visibleGrid)
        std::ranges::fill(treeLine, 0);

    // Go over grid from all sides
    auto rowsCount{treeGrid.size()};
    auto columnCount{treeGrid[0].size()};

    // Left to Right
    for(auto row = 0u; row < rowsCount; ++row)
    {
        auto maxHeight{-1};
        for(auto column = 0u; column < columnCount; ++column)
        {
            auto height{treeGrid[row][column]};
            if(height > maxHeight)
            {
                visibleGrid[row][column] = 1;
                maxHeight = height;
            }
        }
    }

    // Right to Left
    for(auto row = 0u; row < rowsCount; ++row)
    {
        auto maxHeight{-1};
        for(int column = columnCount - 1; column >= 0; --column)
        {
            auto height{treeGrid[row][column]};
            if(height > maxHeight)
            {
                visibleGrid[row][column] = 1;
                maxHeight = height;
            }
        }
    }

    // Top to Bottom
    for(auto column = 0u; column < columnCount; ++column)
    {
        auto maxHeight{-1};
        for(auto row = 0u; row < rowsCount; ++row)
        {
            auto height{treeGrid[row][column]};
            if(height > maxHeight)
            {
                visibleGrid[row][column] = 1;
                maxHeight = height;
            }
        }
    }

    // Bottom to Top
    for(auto column = 0u; column < columnCount; ++column)
    {
        auto maxHeight{-1};
        for(int row = rowsCount - 1; row >= 0; --row)
        {
            auto height{treeGrid[row][column]};
            if(height > maxHeight)
            {
                visibleGrid[row][column] = 1;
                maxHeight = height;
            }
        }
    }

    // auto count{0};
    // for(auto& line : visibleGrid)
    // {
    //     for(auto c : line)
    //     {
    //         count += c;
    //         std::cout << c;
    //     }
    //     std::cout << std::endl;
    // }

    return visibleGrid;
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



int main(int argc, char* argv[])
{
    std::cout << "*#*#*# AOC 08.12.2022 *#*#*#\n";

    std::ifstream file("../input/aoc08.txt");
    std::vector<std::vector<int>> treeGrid;
    std::string input;
    while(std::getline(file, input))
    {
        std::vector<int> treeLine;
        for(auto c : input)
            treeLine.emplace_back(c - '0');

        treeGrid.emplace_back(std::move(treeLine));
    }

    auto visibleGrid = MarkTreesVisible(treeGrid);

    auto treesVisible = ranges::accumulate(visibleGrid | ranges::views::join, 0);
    std::cout << "Task 1: Number of Trees visible is: " << treesVisible << std::endl;

    auto treeScoreGrid = ComputeTreeScore(treeGrid);
    auto treeScore = ranges::max(treeScoreGrid | ranges::views::join);

    std::cout << "Task 2: Best Score is: " << treeScore << std::endl;
    return 0;
}