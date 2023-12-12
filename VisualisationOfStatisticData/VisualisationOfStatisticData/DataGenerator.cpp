#include "DataGenerator.h"
#include <random>
#include "FileWriter.h"

DataGenerator::DataGenerator(size_t size) {
	DataGenerator::sizeOfData = size;
	DataGenerator::generateDataType = Types::Type::INT;
}

DataGenerator::DataGenerator(size_t size, Types::Type type) {
	DataGenerator::sizeOfData = size;
	DataGenerator::generateDataType = type;
}

DataGenerator::DataGenerator(size_t size, Types::Type type, int minInt, int maxInt) {
	DataGenerator::sizeOfData = size;
	DataGenerator::generateDataType = type;
	DataGenerator::minInt = minInt;
	DataGenerator::maxInt = maxInt;
	DataGenerator::isBounds = true;
}

void DataGenerator::setDataType(Types::Type type) {
	DataGenerator::generateDataType = type;
}

DataGenerator::Data DataGenerator::generateData() {
	Data output;
	output.type = DataGenerator::generateDataType;
	std::random_device rd;
	std::mt19937 gen(rd());
	if (DataGenerator::generateDataType == Types::Type::INT) {
		output.charData = new char* [DataGenerator::sizeOfData];
		if (DataGenerator::isBounds) {
			std::uniform_int_distribution<int> dis(DataGenerator::minInt, DataGenerator::maxInt);
			for (int i = 0; i < DataGenerator::sizeOfData; i++) {
				int randNum = dis(gen);
				std::string strNum = std::to_string(randNum);
				output.charData[i] = new char[strNum.length() + 1];
				strcpy_s(output.charData[i], strNum.length() + 1, strNum.c_str());
			}
		}
		else {
			std::uniform_int_distribution<int> dis(DataGenerator::fixedMin, DataGenerator::fixedMax);
			for (int i = 0; i < DataGenerator::sizeOfData; i++) {
				int randNum = dis(gen);
				char* temp = new char[2];
				temp[0] = '0' + randNum;
				temp[1] = '\0';
				output.charData[i] = temp;
			}
		}
	}
	else {
		std::uniform_int_distribution<int> lengthDistribution(3, maxStringLength);
		std::uniform_int_distribution<int> charDistribution(0, charsetSize - 1);
		output.charData = new char*[DataGenerator::sizeOfData];
		for (int i = 0; i < DataGenerator::sizeOfData; i++) {
			int length = lengthDistribution(gen);
			output.charData[i] = new char[length + 1];
			char* temp = new char[length + 1];
			for (int j = 0; j < length; j++) {
				temp[j] = charset[charDistribution(gen)];
				if (temp[j] == ' ') {
					int x = 0;
				}
			}
			temp[length] = '\0';
			output.charData[i] = temp;
		}
	}
	return output;
}

void DataGenerator::generateAndWriteToFile(const wchar_t* pathToFile) {
	Data generatedData = DataGenerator::generateData();
	try {
		FileWriter* writer = new FileWriter(pathToFile);
		writer->write(generatedData.charData, DataGenerator::sizeOfData);
		delete writer;
	}
	catch (std::exception e) {
		MessageBox(NULL, L"Error when writing data to file", L"I/O Error", MB_OKCANCEL);
	}
}