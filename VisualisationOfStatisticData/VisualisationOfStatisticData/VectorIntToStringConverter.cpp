#include "VectorIntToStringConverter.h"

std::vector<std::string> VectorIntToStringConverter::convert(std::vector<int> toConvert) {
	std::vector<std::string> out;
	for (int num : toConvert) {
		out.push_back(std::to_string(num));
	}
	return out;
}