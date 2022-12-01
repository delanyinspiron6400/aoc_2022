#include <iostream>
#include <vector>
#include <fstream>
#include <string>

int main(int argc, char* argv[])
{
    std::cout << "*#*#*# AOC 01.12.2022 *#*#*#\n";

    std::ifstream file("../input/aoc01.txt");

    auto input_calories{0};
    using elf_calory_input = std::vector<int>;
    std::vector<elf_calory_input> calories_per_elf;

    std::string line;
    while (std::getline(file, line))
    {
        std::cout << line << "---" << line.size() << std::endl;
        // std::istringstream iss(line);
        // int a, b;
        // if (!(iss >> a >> b)) { break; } // error

        // // process pair (a,b)
    }

    return 0;
}