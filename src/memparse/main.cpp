#include <regex>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace
{

size_t parseHeapMemory(const std::string& smapsFilePath)
{
    std::ifstream smapsFile(smapsFilePath);
    if (!smapsFile.is_open())
        throw std::runtime_error("Failed to open " + smapsFilePath);

    std::string line;
    bool inHeapRegion = false;
    size_t privateHeapMemory = 0;
    while (std::getline(smapsFile, line))
    {
        if (line.find("[heap]") != std::string::npos)
            inHeapRegion = true;
        else if (line.empty())
            inHeapRegion = false;

        if (inHeapRegion && line.find("Private_Dirty:") != std::string::npos)
        {
            std::istringstream iss(line);
            std::string key;
            size_t value = 0;
            iss >> key >> value;
            privateHeapMemory += value;
        }
    }

    smapsFile.close();
    return privateHeapMemory;
}

}

// Calculate accurate heap memory usage of a process
int main(int argc, const char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <pid>" << std::endl;
        return 1;
    }

    std::string pid = argv[1];

    try {
        size_t heapMemory = parseHeapMemory("/proc/" + pid + "/smaps");
        std::cout << "Heap memory usage: " << heapMemory << " KB" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
