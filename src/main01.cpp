#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

int main(int argc, char* argv[])
{
    std::cout << "*#*#*# AOC 01.12.2022 *#*#*#\n";

    std::ifstream file("../input/aoc01.txt");

    auto input_calories{0};
    std::vector<int> caloriesPerElfSum;

    std::string line;
    auto caloriesSum{0};
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        if ((iss >> input_calories )) 
        {
            caloriesSum += input_calories;
        }
        else
        {
            caloriesPerElfSum.emplace_back(caloriesSum);
            caloriesSum = 0;
        }
    }

    std::sort(caloriesPerElfSum.begin(), caloriesPerElfSum.end(), std::greater<int>());

    std::cout << "Task 1: " << caloriesPerElfSum[0] << " kcal" << std::endl;

    std::cout << "Task 2: " << caloriesPerElfSum[0] + caloriesPerElfSum[1] + caloriesPerElfSum[2]  << " kcal" << std::endl;

    return 0;
}