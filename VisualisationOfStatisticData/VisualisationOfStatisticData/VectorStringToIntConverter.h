#pragma once
#include <vector>
#include <string>

class VectorStringToIntConverter {
public:
	std::vector<int> static convert(std::vector<std::string> dataToConvert);
	std::vector<int> static convert(std::vector<char*> dataToConvert);
};