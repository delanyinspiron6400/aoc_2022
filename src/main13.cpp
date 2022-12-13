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
    Done,
    Continue,
    Break
};

Comp compareInts(int first, int second)
{
    if(first <= second)
    {
        if(first == second)
            return Comp::Continue;
        return Comp::Done;
    }
    else
    {
        return Comp::Break;
    }
}

Comp compareLists(const List & list1, const List & list2)
{
    auto itemCount{std::min(list1.items.size(), list2.items.size())};
    for(auto index : ranges::views::iota(0, static_cast<int>(itemCount)))
    {
        if(std::holds_alternative<int>(list1.items[index]) &&
            std::holds_alternative<int>(list2.items[index]))
        {
            auto firstValue{std::get<int>(list1.items[index])};
            auto secondValue{std::get<int>(list2.items[index])};
            auto comparisonResult{compareInts(firstValue, secondValue)};
            if(comparisonResult == Comp::Done || comparisonResult == Comp::Break)
                return comparisonResult;
            else
                continue;
        }
        else if(std::holds_alternative<List>(list1.items[index]) &&
                std::holds_alternative<List>(list2.items[index]))
        {
            auto& firstList{std::get<List>(list1.items[index])};
            auto& secondList{std::get<List>(list2.items[index])};
            auto compValue{compareLists(firstList, secondList)};
            if(compValue == Comp::Continue)
                continue;
            return compValue;
        }
        else
        {
            if(std::holds_alternative<int>(list1.items[index]))
            {
                // std::cout << "First one is int, second list\n";
                List tmp_list;
                tmp_list.items.push_back(list1.items[index]);
                auto compValue{compareLists(tmp_list, std::get<List>(list2.items[index]))};
                if(compValue == Comp::Continue)
                    continue;
                return compValue;
            }
            else
            {
                // std::cout << "First one is list, second int\n";
                List tmp_list;
                tmp_list.items.push_back(list2.items[index]);
                auto compValue{compareLists(std::get<List>(list1.items[index]), tmp_list)};
                if(compValue == Comp::Continue)
                    continue;
                return compValue;
            }
        }
    }

    if(list1.items.size() < list2.items.size())
        return Comp::Done;
    else if (list1.items.size() > list2.items.size())
        return Comp::Break;

    return Comp::Continue;
}

int ComputeValidPairs(const Input & pairs)
{
    Input successfulPairs;
    int indexSum{0};
    for(const auto & [listIndex, list] : ranges::views::enumerate(pairs))
    {
        const auto & [list1, list2]{list};
        if(compareLists(list1, list2) != Comp::Break)
        {
            // fmt::print("Pair {} is valid\n", listIndex);
            successfulPairs.emplace_back(list);
            indexSum += (listIndex + 1);
        }
    }

    auto sumSuccessfullPairs{0};
    for(const auto & [list1, list2] : successfulPairs)
    {
        ++sumSuccessfullPairs;
    }

    return indexSum;
}
