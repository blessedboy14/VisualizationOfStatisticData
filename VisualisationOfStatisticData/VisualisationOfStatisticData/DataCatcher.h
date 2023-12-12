#pragma once
#include "Types.h"
#include <vector>
#include <string>

class DataCatcher {
public:
	DataCatcher();
	struct Data
	{
		Types::Type type;
		std::vector<int> intData;
		std::vector<std::string> charData;
	};
	struct Data2
	{
		Types::Type type;
		std::vector<int> intData;
		std::vector<char*> charData;
	};
	DataCatcher::Data GetDataFromFile(const wchar_t* fileName);
	DataCatcher::Data2 GetData(const wchar_t* fileName);
	Types::Type ResolveDataType(std::vector<std::string> checkedData);
	Types::Type ResolveDataType(char* checkedData);
};