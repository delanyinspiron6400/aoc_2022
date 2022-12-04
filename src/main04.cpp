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

int main(int argc, char* argv[])
{
    std::cout << "*#*#*# AOC 04.12.2022 *#*#*#\n";

    std::ifstream file("../input/aoc04.txt");

	std::string line;
	std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> elven_pair_int;
	while(std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string first_elf;
		std::string second_elf;
		std::getline(ss, first_elf, ',');
		std::getline(ss, second_elf, ',');
		std::stringstream first_ss(first_elf);
		std::stringstream second_ss(second_elf);
		std::string first_first;
		std::string first_second;
		std::string second_first;
		std::string second_second;
		std::getline(first_ss, first_first, '-');
		std::getline(first_ss, first_second, '-');
		std::getline(second_ss, second_first, '-');
		std::getline(second_ss, second_second, '-');

		elven_pair_int.emplace_back(std::pair<std::pair<int, int> , std::pair<int, int>>{{std::stoi(first_first), std::stoi(first_second)}, {std::stoi(second_first), std::stoi(second_second)}});
	}

	for(auto [first_elf, second_elf] : elven_pair_int)
	{

	}
	return 0;
}