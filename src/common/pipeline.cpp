#include <fstream>
#include <iostream>
#include <stdexcept>

#include "pipeline.hpp"

namespace
{
std::vector<char> readFile(const std::string &filepath)
{
    // std::ios::ate move to the end of the move so by
    // calling tellg we can get the file size.
    std::ifstream file{filepath, std::ios::ate | std::ios::binary};

    if (!file.is_open())
    {
        throw std::runtime_error("failed to open file: " + filepath);
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);

    // Go the start of the file to read the data.
    file.seekg(0);
    file.read(buffer.data(), static_cast<long int>(fileSize));
    file.close();

    return buffer;
}
} // namespace

Pipeline::Pipeline(const std::string &vertFilepath, const std::string &fragFilepath)
{
    createGraphicsPipeline(vertFilepath, fragFilepath);
}

void Pipeline::createGraphicsPipeline(const std::string &vertFilepath, const std::string &fragFilepath)
{
    auto vertCode = readFile(vertFilepath);
    auto fragCode = readFile(fragFilepath);

    std::cout << "Vertex Shader Code Size: " << vertCode.size() << '\n';
    std::cout << "Fragment Shader Code Size: " << fragCode.size() << '\n';
}
