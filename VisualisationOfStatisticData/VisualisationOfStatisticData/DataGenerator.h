#pragma once
#include <vector>
#include <string>
#include "Types.h"

class DataGenerator {
public:
	const char* charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const int charsetSize = 52;
	struct Data
	{
		Types::Type type;
		char** intData;
		char** charData;
	};
	DataGenerator(size_t size, Types::Type type);
	DataGenerator(size_t size);
	DataGenerator(size_t size, Types::Type type, int minInt, int maxInt);
	void setDataType(Types::Type type);
	Data generateData();
	void generateAndWriteToFile(const wchar_t* fileName);
private:
	int minInt = 0;
	int maxInt = 100;
	bool isBounds;
	const int fixedMin = 0;
	const int fixedMax = 9;
	const int maxStringLength = 15;
	Types::Type generateDataType;
	size_t sizeOfData;
	Data generateWithBounds(int min, int max);
	DataGenerator() {};
};