#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <ranges>
#include <algorithm>
#include <string_view>
#include <bitset>
#include <memory>

struct File
{
    size_t size{};
    std::string name;
};

struct Directory
{
    std::string name;
    std::vector<File*> files;
    std::vector<Directory*> directories;
    Directory* parent{};

    size_t GetSize()
    {
        size_t size{};
        for(auto dir : directories)
            size += dir->GetSize();
        for(auto file : files)
            size += file->size;
        return size;
    }
};


int main(int argc, char* argv[])
{
    std::cout << "*#*#*# AOC 07.12.2022 *#*#*#\n";

    std::vector<std::unique_ptr<Directory>> directories;
    std::vector<std::unique_ptr<File>> files;
    Directory* root;

    std::ifstream file("../input/aoc07.txt");
    std::string input;
    while(std::getline(file, input))
    {
        if(input.at(0) == '$')
        {
            // We got a command
        }
        else
        {
            // We got a file or directory
        }
            std::cout << input << std::endl;
    }

    return 0;
}