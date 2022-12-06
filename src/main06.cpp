#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <ranges>
#include <algorithm>
#include <string_view>
#include <bitset>

// Ranges
#include <range/v3/all.hpp>

bool checkUniqueness(std::string_view string, int numUniqueCharacters = 4)
{
    std::bitset<32> bitmask;
    for(auto c : string)
        bitmask.set(c - 'a');
    return bitmask.count() == numUniqueCharacters;
}

size_t findUniqueSequenceIndex(std::string_view input, int numUniqueCharacters = 4)
{
    auto result =
        input | ranges::views::sliding(numUniqueCharacters) |
        ranges::views::transform([numUniqueCharacters](auto view) {
            return ranges::accumulate(view, std::bitset<32>(), [](auto & set, auto c) {
                       set.set(c - 'a');
                       return set;
                   }).count() == numUniqueCharacters;
        }) |
        ranges::views::enumerate |
        ranges::views::filter([](auto v) { return std::get<1>(v) == true; }) |
        ranges::views::transform([](auto t) { return std::get<0>(t); }) |
        ranges::views::take(1);

    if(result.empty())
        return 0;
    return *result.begin() + numUniqueCharacters;
}

int main(int argc, char* argv[])
{
    std::cout << "*#*#*# AOC 06.12.2022 *#*#*#\n";

    std::ifstream file("../input/aoc06.txt");
    std::string input;
    std::getline(file, input);

    auto startPacketIndex{0};
    for(auto i = 4; i <= input.size(); ++i)
    {
        if(checkUniqueness(std::string_view(input.begin() + i - 4, input.begin() + i)))
        {
            startPacketIndex = i;
            break;
        }
    }
    std::cout << "Task 1 : Start Index at " << startPacketIndex << std::endl;
    std::cout << "Task 1 : Start Index at " << findUniqueSequenceIndex(input, 4)
              << std::endl;

    auto startMessageIndex{0};
    for(auto i = 14; i <= input.size(); ++i)
    {
        if(checkUniqueness(std::string_view(input.begin() + i - 14, input.begin() + i), 14))
        {
            startMessageIndex = i;
            break;
        }
    }
    std::cout << "Task 2 : Start Index at " << startMessageIndex << std::endl;
    std::cout << "Task 1 : Start Index at " << findUniqueSequenceIndex(input, 14)
              << std::endl;

    return 0;
}