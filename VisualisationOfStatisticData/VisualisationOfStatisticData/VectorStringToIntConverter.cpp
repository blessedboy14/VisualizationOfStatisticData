#include "VectorStringToIntConverter.h"

std::vector<int> VectorStringToIntConverter::convert(std::vector<std::string> dataToConvert) {
    std::vector<int> result;
    for (const auto& str : dataToConvert)
    {
        result.push_back(std::stoi(str));
    }
    return result;
}

std::vector<int> VectorStringToIntConverter::convert(std::vector<char*> dataToConvert) {
    std::vector<int> result;
    for (const auto& str : dataToConvert)
    {
        result.push_back(std::stoi(str));
    }
    return result;
}