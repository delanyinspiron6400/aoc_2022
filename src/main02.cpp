#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <unordered_map>

std::unordered_map<char, std::string> printMap
{
    {'A', "Rock"},
    {'B', "Paper"},
    {'C', "Scissor"},
    {'X', "Rock"},
    {'Y', "Paper"},
    {'Z', "Scissor"}
};

std::unordered_map<char, int> shapePointResolution
{
    {'A', 1},
    {'B', 2},
    {'C', 3}
};

int main(int argc, char* argv[])
{
    std::cout << "*#*#*# AOC 02.12.2022 *#*#*#\n";

    std::ifstream file("../input/aoc02.txt");

    using InputData = std::vector<std::pair<char, char>>;
    InputData parsedInputData;
    char prediction, move;
    while(file >> prediction >> move)
    {
        parsedInputData.emplace_back(prediction, move);
    }


    auto accumulatedScore{0};
    for(auto [predictedMove, responseMove] : parsedInputData)
    {
        switch(predictedMove)
        {
            case 'A':
            {
                switch(responseMove)
                {
                    case 'X':
                    {
                        accumulatedScore += 3 + 1;
                        break;
                    }
                    case 'Y':
                    {
                        accumulatedScore += 6 + 2;
                        break;
                    }
                    case 'Z':
                    {
                        accumulatedScore += 0 + 3;
                        break;
                    }
                    default:
                        std::cout << "Unrecognized response\n";
                }
                break;
            }
            case 'B':
            {
                switch(responseMove)
                {
                    case 'X':
                    {
                        accumulatedScore += 0 + 1;
                        break;
                    }
                    case 'Y':
                    {
                        accumulatedScore += 3 + 2;
                        break;
                    }
                    case 'Z':
                    {
                        accumulatedScore += 6 + 3;
                        break;
                    }
                    default:
                        std::cout << "Unrecognized response\n";
                }
                break;
            }
            case 'C':
            {
                switch(responseMove)
                {
                    case 'X':
                    {
                        accumulatedScore += 6 + 1;
                        break;
                    }
                    case 'Y':
                    {
                        accumulatedScore += 0 + 2;
                        break;
                    }
                    case 'Z':
                    {
                        accumulatedScore += 3 + 3;
                        break;
                    }
                    default:
                        std::cout << "Unrecognized response\n";
                }
                break;
            }
            default:
                std::cout << "Unrecognized prediction\n";
        }
    }

    auto accumulatedPredictionScore{0};
    for(auto [predictedMove, prediction] : parsedInputData)
    {
        switch(predictedMove)
        {
            case 'A':
            {
                switch(prediction)
                {
                    case 'X':
                    {
                        accumulatedPredictionScore += 0 + 3;
                        break;
                    }
                    case 'Y':
                    {
                        accumulatedPredictionScore += 3 + 1;
                        break;
                    }
                    case 'Z':
                    {
                        accumulatedPredictionScore += 6 + 2;
                        break;
                    }
                    default:
                        std::cout << "Unrecognized response\n";
                }
                break;
            }
            case 'B':
            {
                switch(prediction)
                {
                    case 'X':
                    {
                        accumulatedPredictionScore += 0 + 1;
                        break;
                    }
                    case 'Y':
                    {
                        accumulatedPredictionScore += 3 + 2;
                        break;
                    }
                    case 'Z':
                    {
                        accumulatedPredictionScore += 6 + 3;
                        break;
                    }
                    default:
                        std::cout << "Unrecognized response\n";
                }
                break;
            }
            case 'C':
            {
                switch(prediction)
                {
                    case 'X':
                    {
                        accumulatedPredictionScore += 0 + 2;
                        break;
                    }
                    case 'Y':
                    {
                        accumulatedPredictionScore += 3 + 3;
                        break;
                    }
                    case 'Z':
                    {
                        accumulatedPredictionScore += 6 + 1;
                        break;
                    }
                    default:
                        std::cout << "Unrecognized response\n";
                }
                break;
            }
            default:
                std::cout << "Unrecognized prediction\n";
        }
    }

    std::cout << "Task 1: Predicted Score: " << accumulatedScore << std::endl;
    std::cout << "Task 2: Predicted Score: " << accumulatedPredictionScore << std::endl;

    return 0;
}