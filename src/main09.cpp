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

  unsigned int CreateUniqueHashableValue() const
  {
    return static_cast<unsigned int>(x + 1000) * 4000 + static_cast<unsigned int>(y + 1000);
  }
};

using Vecs = std::vector<std::pair<Vec, int>>;

// Forward declaration
Vecs ParseMovements(std::istream&& input);
template<int LENGTH>
int CountVisitedPositions(const Vecs& directions);

int main(int argc, char* argv[])
{
    std::cout << "*#*#*# AOC 09.12.2022 *#*#*#\n";

    auto motionSeries = ParseMovements(std::fstream("../input/aoc09.txt"));

    auto visitedPositionsCount{CountVisitedPositions<2>(motionSeries)};
    auto visitedPositionsCountLong{CountVisitedPositions<10>(motionSeries)};


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

template <int LENGTH>
int CountVisitedPositions(const Vecs& directions)
{
    std::unordered_set<unsigned int> visitedPositions;
    std::array<Vec, LENGTH> positions;
    std::fill(positions.begin(), positions.end(), Vec{0,0});
    visitedPositions.insert(positions[LENGTH - 1].CreateUniqueHashableValue());

    for(const auto& [dir, length] : directions)
    {
        // Need to find out how much movement was needed
        for(auto i {0}; i < length; ++i)
        {
            positions[0] += dir;

            for(auto current{1}; current < LENGTH; ++current)
            {
                if(positions[current].isTouching(positions[current - 1]))
                    continue;

                // x
                auto diff = positions[current - 1].x - positions[current].x;
                if (diff != 0) 
                    positions[current].x += std::signbit(diff) ? -1 : 1;
                
                // y
                diff = positions[current - 1].y - positions[current].y;
                if (diff != 0) 
                    positions[current].y += std::signbit(diff) ? -1 : 1;
            }

            visitedPositions.insert(positions[LENGTH - 1].CreateUniqueHashableValue());
        }
    }

    return std::ssize(visitedPositions);
}