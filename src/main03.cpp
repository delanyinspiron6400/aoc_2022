#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <ranges>
#include <algorithm>
#include <cstring>
#include <string_view>
#include <unordered_set>

int ComputePriority(char c)
{
    if(std::isupper(c))
        return (c - 'A' + 27);
    else
        return (c - 'a' + 1);
}

int main(int argc, char* argv[])
{
    std::cout << "*#*#*# AOC 03.12.2022 *#*#*#\n";

    std::ifstream file("../input/aoc03.txt");

    std::vector<std::string> input;
    std::string line;
    while(std::getline(file, line))
        input.emplace_back(std::move(line));

    auto accumulated_priorities{0};
    for(const auto& str : input)
    {
        auto first_half = std::string_view(str.begin(), str.begin() + str.size() / 2);
        auto second_half = std::string_view(str.begin() + str.size() / 2, str.end());

        std::unordered_set<char> item_types;
        for(char c : first_half)
        {
            if(std::find(second_half.begin(), second_half.end(), c) != second_half.end())
                item_types.insert(c);
        }

        for(auto c : item_types)
        {
            auto priority{ComputePriority(c)};
            accumulated_priorities += priority;
        }
    }

    std::cout << "Task 1: " << accumulated_priorities << std::endl;

    accumulated_priorities = 0;
    for(auto index {0}; index < std::ssize(input); index += 3)
    {
        auto first = std::string_view(input[index]);
        auto second = std::string_view(input[index + 1]);
        auto third = std::string_view(input[index + 2]);

        std::unordered_set<char> shared_first_second;
        for(char c : first)
        {
            if(std::find(second.begin(), second.end(), c) != second.end())
                shared_first_second.insert(c);
        }

        for(auto c : shared_first_second)
        {
            if(std::find(third.begin(), third.end(), c) != third.end())
            {
                auto priority{ComputePriority(c)};
                accumulated_priorities += priority;
            }
        }
    }

    std::cout << "Task 2: " << accumulated_priorities << std::endl;

    return 0;
}