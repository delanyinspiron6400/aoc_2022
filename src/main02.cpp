#include <iostream>
#include <vector>
#include <fstream>
#include <string>


template<class T>
inline T mod(T a, T b) {
    T ret {a % b};
    return (ret >= 0) ? ret : ret+b;
}

int main(int argc, char* argv[])
{
    std::cout << "*#*#*# AOC 02.12.2022 *#*#*#\n";

    std::ifstream file("../input/aoc02.txt");

    std::vector<std::pair<int, int>> parsedInputDataInt;
    char prediction, move;
    while(file >> prediction >> move)
    {
        parsedInputDataInt.emplace_back(prediction - 'A', move - 'X');
    }

    auto accumulatedScore{0};
    auto accumulatedPredictionScore{0};
    for(auto [prediction, response] : parsedInputDataInt)
    {
        accumulatedScore += (mod(response + (1 - prediction), 3) * 3) + (response + 1);
        accumulatedPredictionScore += (response * 3) + (mod(prediction + (response - 1), 3) + 1);
    }

    std::cout << "Task 1: Predicted Score: " << accumulatedScore << std::endl;
    std::cout << "Task 2: Predicted Score: " << accumulatedPredictionScore << std::endl;

    return 0;
}