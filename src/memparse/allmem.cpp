#include <regex>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

namespace
{

size_t parseMemoryUsage(const std::string& pid)
{
    std::ifstream mapsFile("/proc/" + pid + "/maps");
    if (!mapsFile.is_open())
    {
        throw std::runtime_error("Failed to open /proc/" + pid + "/maps");
    }

    size_t totalMemory = 0;
    std::string line;
    std::regex regex(R"(([\da-f]+)-([\da-f]+) .*)");

    while (std::getline(mapsFile, line))
    {
        std::smatch match;
        if (std::regex_match(line, match, regex))
        {
            auto startAddr = std::stoul(match[1], nullptr, 16);
            auto endAddr = std::stoul(match[2], nullptr, 16);
            totalMemory += (endAddr - startAddr);
        }
    }

    mapsFile.close();
    return totalMemory;
}

}

// Read all memory allocated to a process
// This is useful for detecting changes in memory usage when running under valgrind
int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <pid>" << std::endl;
        return 1;
    }

    try {
        size_t memoryUsage = parseMemoryUsage(argv[1]);
        std::cout << "Memory usage: " << (size_t)(memoryUsage / 1024) << " KB" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
