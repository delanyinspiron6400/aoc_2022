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

enum class Operation
{
    ADD = 0,
    MUL
};

struct Monkey
{
    std::vector<size_t> items;
    std::pair<Operation, size_t> operation;
    size_t divisibleBy{};
    int throwToIndex[2];
    size_t inspectionCount{};

    size_t performOperation(size_t value)
    {
        auto tmp{operation.second != std::numeric_limits<size_t>::max() ? operation.second : value};
        return operation.first == Operation::ADD ? value + tmp : value * tmp;
    }

    void print()
    {
        fmt::print("Starting items: ");
        for(auto item : items)
            fmt::print("{}, ", item);
        fmt::print("\nOperation: new = old {} ", operation.first == Operation::ADD ? "+" : "*");
        if(operation.second > 0)
            fmt::print("{}\n", operation.second);
        else
            fmt::print("{}\n", "old");
        fmt::print("Test: divisible by {}\n", divisibleBy);
        fmt::print("  If true: throw to monkey {}\n", throwToIndex[1]);
        fmt::print("  If false: throw to monkey {}\n", throwToIndex[0]);
    }
};

using Input = std::vector<Monkey>;

Input ParseInput(std::istream&& input);

size_t PlayKeepAway(Input& input, int rounds, size_t divisor);

int main(int argc, char* argv[])
{
    std::cout << "*#*#*# AOC 11.12.2022 *#*#*#\n";

    auto monkeyInput = ParseInput(std::fstream("../input/aoc11.txt"));
    auto monkeyInputCopy = monkeyInput;

    auto levelOfMonkeyBusiness{PlayKeepAway(monkeyInput, 20, 3ULL)};
    auto levelOfMonkeyBusiness10000{PlayKeepAway(monkeyInputCopy, 10000, 1ULL)};

    fmt::print("Task 1 - Level of Monkey business: {}\n", levelOfMonkeyBusiness);
    fmt::print("Task 2 - Level of Monkey business: {}\n", levelOfMonkeyBusiness10000);

    return 0;
}

Input ParseInput(std::istream&& input)
{
    Input monkeys;
    std::string tmp;
    while(std::getline(input, tmp))
    {
        Monkey monkey;
        std::getline(input, tmp); // Header

        std::getline(input, tmp); // Starting items
        std::istringstream iss(tmp);
        std::string param;
        while(std::getline(iss, param, ','))
            monkey.items.emplace_back(std::stoull(param));

        std::getline(input, tmp); // Operation
        Operation op {tmp.at(0) == '*' ? Operation::MUL : Operation::ADD};
        size_t value{};
        if(std::string_view(tmp.begin() + 2, tmp.end()).compare("old") == 0)
            value = std::numeric_limits<size_t>::max();
        else
            value = std::stoull(std::string{tmp.begin() + 2, tmp.end()});
        monkey.operation = {op, value};

        std::getline(input, tmp); // Divisible by
        monkey.divisibleBy = std::stoull(tmp);

        std::getline(input, tmp); // if True
        monkey.throwToIndex[1] = std::stoi(tmp);

        std::getline(input, tmp); // if False
        monkey.throwToIndex[0] = std::stoi(tmp);

        monkeys.emplace_back(std::move(monkey));
        // monkeys.back().print();
    }

    return monkeys;
}

size_t PlayKeepAway(Input& input, int rounds, size_t divisor)
{
    // Chinese Remainder Theorem
    size_t superMod{1};
    for(auto& monkey : input)
        superMod *= monkey.divisibleBy;

    for(auto round {0}; round < rounds; ++round)
    {
        for(auto& monkey : input)
        {
            for(auto item : monkey.items)
            {
                item = monkey.performOperation(item) % superMod / divisor;
                input[monkey.throwToIndex[((item % monkey.divisibleBy) == 0) ? 1 : 0]].items.push_back(item);
                ++monkey.inspectionCount;
            }
            monkey.items.clear();
        }
    }

    ranges::sort(input, [](auto& monkeyA, auto& monkeyB){return monkeyA.inspectionCount > monkeyB.inspectionCount;});

    // for(auto& monkey : input)
    //     fmt::print("Monkey has inspection count {}\n", monkey.inspectionCount);
    return input[0].inspectionCount * input[1].inspectionCount;
}