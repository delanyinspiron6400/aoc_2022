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

struct Point
{
    int x{};
    int y{};

    Point & operator+=(const Point & other)
    {
        x += other.x;
        y += other.y;
        return *this;
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
};

using Line = std::vector<Point>;
using Input = std::vector<Line>;

enum class CellContent
{
    AIR,
    ROCK,
    SAND
};

struct Grid
{
    Grid(int rows, int cols)
    {
        grid.resize(rows);
        for(auto & line : grid)
        {
            line.resize(cols);
            std::fill(line.begin(), line.end(), CellContent::AIR);
        }
        this->rows = rows;
        this->cols = cols;
    }
    std::vector<std::vector<CellContent>> grid;
    int rows{};
    int cols{};
    std::unordered_map<CellContent, unsigned char> map{{CellContent::AIR, '.'},
                                                       {CellContent::ROCK, '#'},
                                                       {CellContent::SAND, 'o'}};

    bool isInBounds(const Point & point)
    {
        // fmt::print("Is in Bounds {},{}\n", point.x, point.y);
        return (point.x >= 0) && (point.x < cols) && (point.y >= 0) && (point.y < rows);
    }

    bool isFree(const Point & point)
    {
        // fmt::print("Is free {},{}\n", point.x, point.y);
        return grid[point.y][point.x] == CellContent::AIR;
    }

    void MarkPosition(const Point & point, CellContent content)
    {
        grid[point.y][point.x] = content;
    }

    void Print()
    {
        for(auto & line : grid)
        {
            for(auto cell : line)
                std::cout << map[cell];
            std::cout << std::endl;
        }
    }
};

Input ParseInput(std::istream&& input);
int SimulateSand(const Input & traces);
bool ThrowSandOntoRocks(Grid & grid, Point startingPosition);

int main(int argc, char* argv[])
{
    std::cout << "*#*#*# AOC 14.12.2022 *#*#*#\n";

    auto traces = ParseInput(std::fstream("../input/aoc14_test.txt"));

    // for(auto & trace : traces)
    // {
    //     for(auto & point : trace)
    //     {
    //         fmt::print("{},{} ->", point.x, point.y);
    //     }
    //     fmt::print("\n");
    // }

    fmt::print("Units of Sand : {}\n", SimulateSand(traces));

    return 0;
}

Input ParseInput(std::istream && input)
{
    // using namespace ranges;
    using namespace ranges::views;
    using ranges::getlines;
    using ranges::to;
    return getlines(input) | transform([](auto & view) {
               return view | split(' ') | transform([](auto && pointStr) {
                          auto res{pointStr | split(',') | transform([](auto && rng) {
                                       return std::stoi(std::string(
                                           &*rng.begin(), ranges::distance(rng)));
                                   }) |
                                   to<std::vector<int>>};
                          return Point(res[0], res[1]);
                      }) |
                      to<Line>;
           }) |
           to<std::vector<Line>>;
}

int SimulateSand(const Input & traces)
{
    // Find maximum extent
    auto xMax{ranges::max(traces | ranges::views::join |
                          ranges::views::transform([](auto point) { return point.x; }))};
    auto xMin{ranges::min(traces | ranges::views::join |
                          ranges::views::transform([](auto point) { return point.x; }))};
    auto yMax{ranges::max(traces | ranges::views::join |
                          ranges::views::transform([](auto point) { return point.y; }))};
    auto yMin{ranges::min(traces | ranges::views::join |
                          ranges::views::transform([](auto point) { return point.y; }))};

    auto xExtent{xMax - xMin};
    auto yExtent{yMax};

    Grid cells(yExtent + 1, xExtent);

    auto alignedTraces = traces;
    for(auto & line : alignedTraces)
    {
        for(auto & point : line)
        {
            point.x -= xMin;
        }
    }

    for(auto & trace : alignedTraces)
    {
        for(auto i{0}; i < trace.size() - 1; ++i)
        {
            auto startPoint{trace[i]};
            auto endPoint{trace[i + 1]};

            Point vec{endPoint.x - startPoint.x, endPoint.y - startPoint.y};
            auto length{(vec.x != 0) ? vec.x : vec.y};
            vec /= length;

            cells.MarkPosition(startPoint, CellContent::ROCK);
            for(auto start{0}; start < length; ++start)
            {
                startPoint += vec;
                cells.MarkPosition(startPoint, CellContent::ROCK);
            }
        }
    }
    std::cout << "\n\n\n";

    cells.Print();

    std::cout << "\n\n\n";

    // Fill in sand
    Point sandEntryStartingPos{.x = 500 - xMin, .y = -1};

    auto sandCount{0};
    while(true)
    {
        if(!ThrowSandOntoRocks(cells, sandEntryStartingPos))
            break;

        // cells.Print();
        // std::getchar();
        ++sandCount;
    }

    cells.Print();

    // fmt::print("{} {} {} {}\n", xMax, xMin, yMax, yMin);
    // fmt::print("{} {}\n", xExtent, yExtent);
    return sandCount;
}

bool ThrowSandOntoRocks(Grid & grid, Point position)
{
    static constexpr Point downVec{0, 1};
    static constexpr Point leftVec{-1, 0};
    static constexpr Point rightVec{2, 0};
    bool continueThrowingSand{false};
    // Find correct position
    while(true)
    {
        auto current_position{position};
        position += downVec;
        if(!grid.isInBounds(position))
            break;
        if(grid.isFree(position))
            continue;

        // Test left
        position += leftVec;
        if(!grid.isInBounds(position))
            break;
        if(grid.isFree(position))
            continue;

        // Test left
        position += rightVec;
        if(!grid.isInBounds(position))
            break;
        if(grid.isFree(position))
            continue;

        grid.MarkPosition(current_position, CellContent::SAND);
        continueThrowingSand = true;
        break;
    }

    return continueThrowingSand;
}