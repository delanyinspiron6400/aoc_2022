#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <ranges>
#include <algorithm>
#include <cstring>
#include <string_view>
#include <unordered_set>
#include <sstream>
#include <tuple>
#include <stack>

int main(int argc, char* argv[])
{
    std::cout << "*#*#*# AOC 05.12.2022 *#*#*#\n";

    std::ifstream input_stacks("../input/aoc05_1.txt");
	std::ifstream input_moves("../input/aoc05_2.txt");

    static constexpr int NUM_STACKS{9};
    std::vector<std::vector<char>> stacks(NUM_STACKS);
    std::vector<std::tuple<int, int, int>> moves;

    std::string line;
    std::vector<std::string> stack_strings;
    while (std::getline(input_stacks, line))
    {
        stack_strings.emplace_back(std::move(line));
    }

    for(auto it = stack_strings.rbegin(); it != stack_strings.rend(); ++it)
    {
        for(auto i = 0; i < NUM_STACKS; ++i)
        {
            if(it->at(i * 2) != ' ')
                stacks[i].emplace_back(it->at(i * 2));
        }
    }

    while (std::getline(input_moves, line))
    {
        std::istringstream iss(line);
        int move, from, to;
        iss >> move >> from >> to;
        moves.emplace_back(move, from - 1, to - 1);
    }

    auto stacks_copy = stacks;

    for(auto [move, from, to] : moves)
    {
        for(auto i {0}; i < move; ++i)
        {
            stacks[to].push_back(stacks[from].back());
            stacks[from].pop_back();
        }
    }

    std::cout << "Task 1: ";
    for(auto& stack : stacks)
        std::cout << stack.back();
    std::cout << std::endl;


    for(auto [move, from, to] : moves)
    {
        stacks_copy[to].insert(stacks_copy[to].end(), stacks_copy[from].end() - move, stacks_copy[from].end());
        stacks_copy[from].erase(stacks_copy[from].end() - move, stacks_copy[from].end());
    }

    std::cout << "Task 2: ";
    for(auto& stack : stacks_copy)
        std::cout << stack.back();
    std::cout << std::endl;


	return 0;
}