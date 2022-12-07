#include <algorithm>
#include <bitset>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <ranges>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

struct File
{
    size_t size{};
    std::string name;

    void print(int numSpaces = 0) const
    {
        std::string prefix(numSpaces, ' ');
        std::cout << prefix << "- " << name << " (file, size=" << size << ")"
                  << std::endl;
    }
};

struct Directory
{
    std::string name;
    std::vector<File> files;
    std::vector<Directory> directories;
    Directory* parent{};

    size_t GetSize() const
    {
        size_t size{};
        for(auto dir : directories)
            size += dir.GetSize();
        for(auto file : files)
            size += file.size;
        return size;
    }

    void printStructure(int numSpaces = 0) const
    {
        std::string prefix(numSpaces, ' ');
        std::cout << prefix << "- " << name << " (dir)" << std::endl;
        for(const auto & dir : directories)
            dir.printStructure(numSpaces + 2);
        for(const auto & file : files)
            file.print(numSpaces + 2);
    }

    // That is not overly efficient as it calls GetSize() multiple times instead of
    // propagating it up
    size_t getSumOfDirectoriesUnder100000() const
    {
        static constexpr size_t upper_threshold{100000ULL};
        auto returnSize{0ULL};
        auto size{GetSize()};
        if(size <= upper_threshold)
        {
            // std::cout << name << " | Size: " << size << std::endl;
            returnSize += size;
        }
        // else
        //     std::cout << name << " too large at: " << size << std::endl;

        for(const auto & dir : directories)
            returnSize += dir.getSumOfDirectoriesUnder100000();

        return returnSize;
    }

    size_t findDirectoryToDeleteForRequirements(
        size_t sizeToReduce, size_t currentMax = std::numeric_limits<size_t>::max()) const
    {
        auto size{GetSize()};
        // std::cout << name << " | Max: " << currentMax << std::endl;
        if(size >= sizeToReduce)
            currentMax = std::min(currentMax, size);

        for(const auto & dir : directories)
            currentMax =
                dir.findDirectoryToDeleteForRequirements(sizeToReduce, currentMax);

        return currentMax;
    }
};


int main(int argc, char* argv[])
{
    std::cout << "*#*#*# AOC 07.12.2022 *#*#*#\n";

    std::vector<std::unique_ptr<Directory>> directories;
    std::vector<std::unique_ptr<File>> files;
    Directory root{"/"};
    Directory * current_dir{&root};

    std::ifstream file("../input/aoc07.txt");
    std::string input;
    std::getline(file, input); // Flush the top line change
    while(std::getline(file, input))
    {

        if(input.at(0) == '$')
        {
            std::istringstream iss(std::string(input.begin() + 2, input.end()));
            std::string param;
            std::getline(iss, param, ' ');
            // We got a command
            if(param.compare("cd") == 0)
            {
                std::string directory;
                std::getline(iss, directory, ' ');
                if(directory.compare("..") == 0)
                {
                    // std::cout << "Change to parent dir: " << current_dir->parent->name
                    //           << std::endl;
                    current_dir = current_dir->parent;
                }
                else
                {
                    for(auto & dir : current_dir->directories)
                    {
                        if(dir.name.compare(directory) == 0)
                        {
                            current_dir = &dir;
                            // std::cout << "Change to: " << current_dir->name <<
                            // std::endl;
                        }
                    }
                };
            }
            else if(param.compare("ls") == 0)
                continue;
        }
        else
        {
            // We got a file or directory
            if(std::string_view(input.begin(), input.begin() + 3).compare("dir") == 0)
            {
                // We found a directory
                Directory new_dir{
                    std::string(input.begin() + 4, input.end()), {}, {}, current_dir};
                // std::cout << "Create dir: " << new_dir.name << std::endl;
                current_dir->directories.emplace_back(std::move(new_dir));
            }
            else
            {
                // We found a file
                std::istringstream iss(input);
                std::string param;
                std::getline(iss, param, ' ');
                size_t size{std::stoull(param)};
                std::getline(iss, param, ' ');
                // std::cout << "Create file: " << param << std::endl;
                current_dir->files.emplace_back(size, std::move(param));
            }
        }
        // std::cout << "Current dir: " << current_dir->name << std::endl;
    }

    // root.printStructure();

    auto totalSum{root.getSumOfDirectoriesUnder100000()};
    std::cout << "Task 1: Overall Size of directories under 100000 is: " << totalSum
              << std::endl;

    static constexpr auto fileSystemSize{70000000ULL};
    static constexpr auto unusedRequirement{30000000ULL};
    static constexpr auto maximumSize{fileSystemSize - unusedRequirement};
    auto diff{root.GetSize() - maximumSize};

    std::cout << "Task 2: Directory size to be deleted: "
              << root.findDirectoryToDeleteForRequirements(diff) << std::endl;

    return 0;
}