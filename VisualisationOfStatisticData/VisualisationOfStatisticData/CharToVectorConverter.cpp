#include "CharToVectorConverter.h"
#include <sstream>

std::vector<std::string> CharToVectorConverter::convert(std::vector<char> chars) {
    std::stringstream ss;
    std::vector<std::string> strings;

    for (char c : chars)
    {
        if (c == ' ')
        {
            strings.push_back(ss.str());
            ss.str("");
        }
        else
        {
            ss << c;
        }
    }
    strings.push_back(ss.str());
    return strings;
}