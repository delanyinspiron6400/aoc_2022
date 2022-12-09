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

struct Vec
{
  int x{};
  int y{};

  auto operator<=>(const Vec&) const = default;

  Vec& operator+= (const Vec& other)
  {
    x += other.x;
    y += other.y;
    return *this;
  }

  bool isTouching(const Vec& pos) const
  {
    return (pos.x >= (x - 1)) && (pos.x <= (x + 1)) && (pos.y >= (y - 1)) && (pos.y <= (y + 1));
  }

  bool isCorner(const Vec& pos) const
  {
    return (pos.x != x) && (pos.y != y);
  }

  Vec CreateMovement(int length) const
  {
    return {x * length, y * length};
  }

  unsigned int CreateUniqueHashableValue() const
  {
    return static_cast<unsigned int>(x + 1000) * 2000 + static_cast<unsigned int>(y + 1000);
  }
};

using Vecs = std::vector<std::pair<Vec, int>>;

// Forward declaration
Vecs ParseMovements(std::istream&& input);
int CountVisitedPositions(const Vecs& directions);
int CountVisitedPositionsLong(const Vecs& directions);

int main(int argc, char* argv[])
{
    std::cout << "*#*#*# AOC 09.12.2022 *#*#*#\n";

    auto motionSeries = ParseMovements(std::fstream("../input/aoc09.txt"));

    // for(auto& motion : motionSeries)
    // {
    //     fmt::print("({},{})\n", motion.x, motion.y);
    // }

    auto visitedPositionsCount{CountVisitedPositions(motionSeries)};
    auto visitedPositionsCountLong{CountVisitedPositionsLong(motionSeries)};


    fmt::print("Task 1: Number of visited positions {}\n", visitedPositionsCount);
    fmt::print("Task 2: Number of visited positions {}\n", visitedPositionsCountLong);

    return 0;
}

Vecs ParseMovements(std::istream&& input)
{
  // using namespace ranges;
  using namespace ranges::views;
  using ranges::getlines;
  using ranges::to;
  return getlines(input) |
         transform([](auto& view) 
         { 
            // std::pair<Vec, int> dir;
            Vec dir;
            if(view.at(0) == 'L')
                dir = {-1, 0};
            else if(view.at(0) == 'R')
                dir = {1, 0};
            else if(view.at(0) == 'U')
                dir = {0, 1};
            else if(view.at(0) == 'D')
                dir = {0, -1};
          return std::pair<Vec, int>(dir, std::stoi(std::string(view.begin() + 2, view.end()))); 
         }) |
         to<std::vector<std::pair<Vec, int>>>;
}

int CountVisitedPositions(const Vecs& directions)
{
    std::unordered_set<unsigned int> visitedPositions;
    auto headPos{Vec{0, 0}};
    auto tailPos{headPos};
    visitedPositions.insert(tailPos.CreateUniqueHashableValue());

    for(const auto& [dir, length] : directions)
    {
        // Need to find out how much movement was needed
        for(auto i {0}; i < length; ++i)
        {
            auto tmpPos{headPos};
            headPos += dir;
            if(tailPos.isTouching(headPos))
                continue;
            
            if(tailPos.isCorner(headPos))
                tailPos = tmpPos; // Move diagonally
            else
                tailPos += dir; // Move straight

            // fmt::print("New Head Pos ({}, {}) and Tail Pos ({}, {})\n", oldHeadPos.x, oldHeadPos.y, tailPos.x, tailPos.y);
            visitedPositions.insert(tailPos.CreateUniqueHashableValue());
        }

        // fmt::print("Headpos ({}, {}) and Tailpos ({}, {})\n", headPos.x, headPos.y, tailPos.x, tailPos.y);
        // fmt::print("------------------------------\n");
    }

    return std::ssize(visitedPositions);
}

int CountVisitedPositionsLong(const Vecs& directions)
{
    std::unordered_set<unsigned int> visitedPositions;
    std::array<Vec, 10> positions;
    std::fill(positions.begin(), positions.end(), Vec{0,0});
    visitedPositions.insert(positions[9].CreateUniqueHashableValue());

    // for(const auto& [dir, length] : directions)
    // {
    //     auto oldHeadPos{headPos};
    //     headPos += dir.CreateMovement(length);
    //     if(tailPos.isTouching(headPos))
    //     {
    //         fmt::print("No movement needed Head ({},{}) and Tail ({},{})\n", headPos.x, headPos.y, tailPos.x, tailPos.y);
    //     }
    //     else
    //     {
    //         // Need to find out how much movement was needed
    //         for(auto i {0}; i < length; ++i)
    //         {
    //             auto tmpPos{oldHeadPos};
    //             oldHeadPos += dir;
    //             if(tailPos.isTouching(oldHeadPos))
    //                 continue;
                
    //             if(tailPos.isCorner(oldHeadPos))
    //                 tailPos = tmpPos; // Move diagonally
    //             else
    //                 tailPos += dir; // Move straight

    //             fmt::print("New Head Pos ({}, {}) and Tail Pos ({}, {})\n", oldHeadPos.x, oldHeadPos.y, tailPos.x, tailPos.y);
    //             visitedPositions.insert(tailPos.CreateUniqueHashableValue());
    //         }
    //     }


    //     fmt::print("Headpos ({}, {}) and Tailpos ({}, {})\n", headPos.x, headPos.y, tailPos.x, tailPos.y);
    //     fmt::print("------------------------------\n");
    // }

    return std::ssize(visitedPositions);
}