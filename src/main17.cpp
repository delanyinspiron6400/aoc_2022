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

enum class Direction
{
    LEFT,
    RIGHT,
    DOWN
};

struct Point
{
    int x{};
    int y{};

    // Point() = default;
    // Point(int x, int y): x{x}, y{y} {}
    // Point(const Point& other) = default;

    Point & operator+=(const Point & other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Point operator+(const Point& other)
    {
        Point ret{*this};
        ret += other;
        return ret;
    }

    Point & operator-=(const Point & other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Point & operator/=(int divisor)
    {
        x /= divisor;
        y /= divisor;
        return *this;
    }

    bool operator==(const Point &other) const {
        return x == other.x && y == other.y; // no problem accessing other.id
    }
};

struct PointHash {
  auto operator()(const Point &p) const -> size_t {
    return (static_cast<size_t>(p.x) << 32) | static_cast<size_t>(p.y);
  }
};



using Vec = Point;
using Rock = std::vector<Point>;
using Input = std::vector<Direction>;

Rock& operator+=(Rock& rock, const Point& point)
{
    for(auto& p : rock)
        p += point;
    return rock;
}

Rock operator+(Rock& rock, const Point& point)
{
    Rock r{rock};
    for(auto& p : r)
        p += point;
    return r;
}

int Height(Rock& rock)
{
    auto height{0};
    for(auto& p : rock)
        height = std::max(std::abs(p.y), height);
    return height;
}

enum class State
{
    MOVED,
    BORDER,
    PLACE
};

State MoveRock(auto& map, Rock& rock, Direction dir)
{
    Vec movement;
    if(dir == Direction::DOWN)
    {
        // fmt::print("Move down\n");
        movement = {0, 1};
        State movementState{State::MOVED};
        for(auto& point : rock)
        {
            auto newPoint{point + movement};
            if(map.find(newPoint) != map.end())
                movementState = State::PLACE;
        }
        if(movementState == State::PLACE)
        {
            for(auto& point : rock)
                map.insert(point);
            return movementState;
        }
    }
    else
    {
        if (dir == Direction::RIGHT)
        {
            // fmt::print("Move right\n");
            movement = {1,0};
            for(auto& point : rock)
            {
                auto newPoint{point + movement};
                if(newPoint.x >= 7 || map.find(newPoint) != map.end())
                    return State::BORDER;
            }
        }
        else
        {
            // fmt::print("Move left\n");
            movement = {-1,0};
            for(auto& point : rock)
            {
                auto newPoint{point + movement};
                if(newPoint.x < 0 || map.find(newPoint) != map.end())
                    return State::BORDER;
            }
        }
    }

    rock += movement;

    return State::MOVED;
}

Rock Line{{0, 0}, {1, 0}, {2, 0}, {3, 0}};
Rock Plus{{1, 0}, {0, 1}, {1, 1}, {2, 1}, {1, 2}};
Rock L{{2, 0}, {2,1}, {0,2}, {1,2}, {2,2}};
Rock VertLine{{0,0}, {0,1}, {0,2}, {0,3}};
Rock Square{{0,0}, {0,1}, {1,0}, {1,1}};

std::vector<Rock> rocks{Line, Plus, L, VertLine, Square};
std::vector<int> rockHeight{1, 3, 3, 4, 2};

Input ParseInput(auto && input);
int64_t LetTheRocksFall(const Input& directions, int64_t rounds);

auto main(int argc, char * argv[]) -> int
{
    std::cout << "*#*#*# AOC 17.12.2022 *#*#*#\n";

    auto directions = ParseInput(std::fstream("../input/aoc17_test.txt"));

    fmt::print("Task 1: Height is: {}\n", LetTheRocksFall(directions, 2022));

    fmt::print("Task 1: Height is: {}\n", LetTheRocksFall(directions, 1000000000000));

    return 0;
}

void printMap(auto& map)
{
    auto maxHeight{ranges::max(map | ranges::views::transform([](auto& point){return std::abs(point.y);}))};
    std::cout << "maxHeight " << maxHeight<< std::endl;
    std::vector<char> line {'|', '.', '.', '.', '.', '.', '.', '.', '|'};
    std::vector<std::vector<char>> display(maxHeight + 1);
    for(auto& l : display)
        l = line;

    for(auto& point : map)
    {
        display[std::abs(point.y)][point.x + 1] = '#';
    }

    for(auto& l : ranges::views::reverse(display))
    {
        for(auto c : l)
            std::cout << c;
        std::cout << std::endl;
    }
}


auto ParseInput(auto && input) -> Input
{
    // using namespace ranges;
    using namespace ranges::views;
    using ranges::getlines;
    using ranges::to;

    std::string inputStr;
    std::getline(input, inputStr);
    return c_str(inputStr.c_str()) | transform([](auto c){
        if(c == '<')
            return Direction::LEFT;
        else
            return Direction::RIGHT;}) | to<Input>;
}

int64_t LetTheRocksFall(const Input& directions, int64_t rounds)
{
    std::unordered_set<Point, PointHash> placedRocks{{0,0}, {1,0}, {2,0}, {3,0}, {4,0}, {5,0}, {6,0}};
    auto currentHeight{0};
    auto directionIndex{0};
    int64_t heightPruned{0};
    for (int64_t rockCount{0LL}; rockCount < rounds; ++rockCount)
    {
        auto currentRock {rocks[rockCount % 5] + Point{2, -(currentHeight + 3 + rockHeight[rockCount % 5])}};

        while(true)
        {
            MoveRock(placedRocks, currentRock, directions[directionIndex]);
            directionIndex = (directionIndex + 1) % directions.size();

            // auto rockCopy = placedRocks;
            // for(auto& p: currentRock)
            //     rockCopy.insert(p);
            // printMap(rockCopy);
            // std::getchar();


            if(MoveRock(placedRocks, currentRock, Direction::DOWN) == State::PLACE)
            {
                currentHeight = std::max(currentHeight, Height(currentRock));
                
                // std::cout << "RockIndex: " << rockCount << " with height " << currentHeight << std::endl;
                // printMap(placedRocks);
                // std::getchar();
                break;
            }

            // auto rockCopy2 = placedRocks;
            // for(auto& p: currentRock)
            //     rockCopy2.insert(p);
            // printMap(rockCopy2);
            // std::getchar();
        }

        if(rockCount % 10000 == 9999)
        {
            auto pruningHeight{currentHeight - 100};
            std::erase_if(placedRocks, [pruningHeight](auto point){return std::abs(point.y) < pruningHeight;});
            // fmt::print("Current Height {}\n", currentHeight);
            std::unordered_set<Point, PointHash> newRocks;
            for(auto& p : placedRocks)
            {
                if(std::abs(p.y) < pruningHeight)
                    continue;
                newRocks.insert(Point{0, pruningHeight} + p);
            }
            placedRocks = newRocks;
            
            currentHeight -= pruningHeight;
            heightPruned += pruningHeight;
            fmt::print("Round {} - {:.3f}%\n", rockCount, (static_cast<double>(rockCount) / static_cast<double>(rounds)) * 100.0);
        }
    }
    return currentHeight + heightPruned;
}