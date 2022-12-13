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
#include <unordered_set>
#include <variant>
#include <vector>

#include <range/v3/all.hpp>

struct List
{
    std::vector<std::variant<int, List>> items;

    void print() const
    {
        fmt::print("[");
        for(auto & item : items)
        {
            if(std::holds_alternative<int>(item))
            {
                fmt::print("{}, ", std::get<int>(item));
            }
            else
            {
                std::get<List>(item).print();
            }
        }
        fmt::print("]");
    }
};

using Input = std::vector<std::pair<List, List>>;

Input ParseInput(std::istream&& input);
int ComputeValidPairs(const Input & pairs);

int main(int argc, char* argv[])
{
    std::cout << "*#*#*# AOC 13.12.2022 *#*#*#\n";

    auto pairs = ParseInput(std::fstream("../input/aoc13.txt"));

    auto sumValidPairs{ComputeValidPairs(pairs)};

    fmt::print("Task 1: Valid pairs sum is: {}\n", sumValidPairs);

    return 0;
}

List GenerateListItem(const std::string & line)
{
    List item;
    List * current{&item};
    std::stack<List *> itemStack;
    // itemStack.push(current);

    int skipIndex{0};
    // std::cout << line << std::endl;
    for(const auto & [index, c] : ranges::views::enumerate(line) | ranges::views::drop(1))
    {
        if(skipIndex > 0)
        {
            --skipIndex;
            // std::cout << "Longer Number" << std::endl;
            continue;
        }

        if(c == '[')
        {
            // std::cout << "[\n";
            List new_item;
            current->items.emplace_back(new_item);
            itemStack.push(current);
            current = &std::get<List>(current->items.back());
        }
        else if(c == ']')
        {
            // std::cout << "]\n";
            if(itemStack.size())
            {
                current = itemStack.top();
                itemStack.pop();
            }
            else
            {
                // std::cout << "We are done here!\n";
            }
        }
        else if(c == ',')
        {
            // std::cout << ",\n";
            continue;
        }
        else if(std::isdigit(c))
        {
            // std::cout << "Is digit\n";
            // Expect a number
            for(auto i{index + 1}; i < line.size() - 1; ++i)
            {
                if(std::isdigit(line.at(i)))
                    ++skipIndex;
                else
                    break;
            }

            // std::cout << std::string(line.begin() + index, line.begin() + index + 1 +
            // skipIndex) << std::endl;
            auto number{std::stoi(
                std::string(line.begin() + index, line.begin() + index + 1 + skipIndex))};
            current->items.emplace_back(number);
            // std::cout << number << "\n";
        }
        else
        {
            std::cout << "Not found\n";
        }
    }
    // std::cout << "Stop" << std::endl;

    return item;
}

Input ParseInput(std::istream && input)
{
    Input listPairs;
    std::string line1;
    while(std::getline(input, line1))
    {
        std::string line2;
        std::getline(input, line2);

        listPairs.emplace_back(
            std::pair<List, List>{GenerateListItem(line1), GenerateListItem(line2)});

        std::getline(input, line2); // Empty line
    }
    return listPairs;
}

enum class Comp
{
    SMALLER,
    EQUAL,
    LARGER
};

Comp compareInts(int first, int second)
{
    if(first <= second)
    {
        if(first == second)
            return Comp::EQUAL;
        return Comp::SMALLER;
    }
    else
    {
        return Comp::LARGER;
    }
}

bool compareLists(const List & list1, const List & list2)
{
    return true;
}

int ComputeValidPairs(const Input & pairs)
{
    Input successfulPairs;
    for(const auto & [listIndex, list] : ranges::views::enumerate(pairs))
    {
        const auto & [list1, list2]{list};
        // list1.print();
        // std::cout << std::endl;
        // list2.print();
        // std::cout << std::endl;
        // std::cout << "................\n";
        auto itemCount{std::min(list1.items.size(), list2.items.size())};
        for(auto index : ranges::views::iota(0, static_cast<int>(itemCount)))
        {
            if(std::holds_alternative<int>(list1.items[index]) &&
               std::holds_alternative<int>(list2.items[index]))
            {
                auto firstValue{std::get<int>(list1.items[index])};
                auto secondValue{std::get<int>(list2.items[index])};
                auto comparisonResult{compareInts(firstValue, secondValue)};
                if(comparisonResult == Comp::SMALLER)
                {
                    successfulPairs.emplace_back(pairs[listIndex]);
                    break;
                }
                else if(comparisonResult == Comp::LARGER)
                    break;
                else
                    continue;
            }
            else if(std::holds_alternative<List>(list1.items[index]) &&
                    std::holds_alternative<List>(list2.items[index]))
            {
                std::cout << "Both Lists" << std::endl;
            }
            else
            {
                if(std::holds_alternative<int>(list1.items[index]))
                {
                    std::cout << "First one is int, second list\n";
                }
                else
                {
                    std::cout << "First one is list, second int\n";
                }
            }
        }
    }

    auto sumSuccessfullPairs{0};
    for(const auto & [list1, list2] : successfulPairs)
    {
        ++sumSuccessfullPairs;
    }

    return sumSuccessfullPairs;
}
