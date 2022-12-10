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

enum class Command
{
    NOOP = 0,
    ADDX
};

using Input = std::vector<std::pair<Command, int>>;
int ComputeSumSignalStrengths(const Input& commands);

// Forward declaration
Input ParseInput(std::istream&& input);


int main(int argc, char* argv[])
{
    std::cout << "*#*#*# AOC 10.12.2022 *#*#*#\n";

    auto commands = ParseInput(std::fstream("../input/aoc10.txt"));

    auto sumSignalStrengths{ComputeSumSignalStrengths(commands)};
    fmt::print("Task 1: The sum of the six signal strengths is: {}\n", sumSignalStrengths);
    fmt::print("Task 2: \n");

    return 0;
}

Input ParseInput(std::istream&& input)
{
  // using namespace ranges;
  using namespace ranges::views;
  using ranges::getlines;
  using ranges::to;
  return getlines(input) |
         transform([](auto& view) 
         { 
            // std::pair<Vec, int> dir;
            std::string_view command_str(view.begin(), view.begin() + 4);
            Command command;
            int number{0};
            if(command_str.compare("noop") == 0)
                command = Command::NOOP;
            else
            {
                command = Command::ADDX;
                number = std::stoi(std::string(view.begin() + 4, view.end()));
            }

          return std::pair<Command, int>(command, number); 
         }) |
         to<std::vector<std::pair<Command, int>>>;
}

int ComputeSumSignalStrengths(const Input& commands)
{
    auto registerValue{1};
    std::vector<int> signalStrengths{registerValue};

    for(const auto& [command, value] : commands)
    {
        if(command == Command::NOOP)
        {
            signalStrengths.emplace_back(registerValue);
        }
        else
        {
            signalStrengths.emplace_back(registerValue);
            registerValue += value;
            signalStrengths.emplace_back(registerValue);
        }
    }

    return (signalStrengths[19] * 20) + (signalStrengths[59] * 60) + (signalStrengths[99] * 100) + (signalStrengths[139] * 140) + (signalStrengths[179] * 180) + (signalStrengths[219] * 220);
}

