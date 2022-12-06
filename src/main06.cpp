#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <ranges>
#include <algorithm>
#include <string_view>
#include <bitset>

bool checkUniqueness(std::string_view string, int numUniqueCharacters = 4)
{
    std::bitset<32> bitmask;
    for(auto c : string)
        bitmask.set(c - 'a');
    return bitmask.count() == numUniqueCharacters;
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
	
	return 0;
}