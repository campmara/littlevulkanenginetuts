#include "Pipeline.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace XIV {
    Pipeline::Pipeline(const std::string &vertPath, const std::string &fragPath) {
        Create(vertPath, fragPath);
    }

    std::vector<char> Pipeline::ReadFile(const std::string &fileName) {
        std::ifstream file{fileName, std::ios::ate | std::ios::binary};

        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + fileName);
        }

        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        return buffer;
    }

    void Pipeline::Create(const std::string &vertPath, const std::string &fragPath) {
        std::vector<char> vertCode = ReadFile(vertPath);
        std::vector<char> fragCode = ReadFile(fragPath);

        std::cout << "Vertex Shader Code Size: " << vertCode.size() << '\n';
        std::cout << "Fragment Shader Code Size: " << fragCode.size() << '\n';
    }
} // namespace XIV