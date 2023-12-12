#include "DataCatcher.h"
#include "FileReader.h"
#include <exception>
#include "VectorStringToIntConverter.h"

DataCatcher::DataCatcher() {

}

DataCatcher::Data DataCatcher::GetDataFromFile(const wchar_t* fileName) {
	DataCatcher::Data catchedData;
	try {
		FileReader* reader = new FileReader(fileName);
		std::vector<std::string> buffer = reader->readAllToVector();
		catchedData.type = DataCatcher::ResolveDataType(buffer);
		if (catchedData.type == Types::Type::STRING) {
			catchedData.charData = buffer;
		}
		else {
			catchedData.intData = VectorStringToIntConverter::convert(buffer);
		}
		delete reader;
	}
	catch (std::exception e) {
		MessageBox(NULL, L"Error while reading file", L"Error", MB_ICONERROR);
	}
	return catchedData;
}

DataCatcher::Data2 DataCatcher::GetData(const wchar_t* fileName) {
	DataCatcher::Data2 catchedData;
	try {
		FileReader* reader = new FileReader(fileName);
		std::vector<char*> buffer = reader->readAllToChar();
		catchedData.type = DataCatcher::ResolveDataType(buffer[0]);
		catchedData.charData = buffer;
		delete reader;
	}
	catch (std::exception e) {
		MessageBox(NULL, L"Error while reading file", L"Error", MB_ICONERROR);
	}
	return catchedData;
}

Types::Type DataCatcher::ResolveDataType(std::vector<std::string> checkedData) {
	bool isNum = true;
	try {
		std::stod(checkedData[0]);
	}
	catch (const std::exception&) {
		isNum = false;
	}
	return isNum ? Types::Type::INT : Types::Type::STRING;
}

Types::Type DataCatcher::ResolveDataType(char* checkedData) {
	bool isNum = true;
	try {
		std::stod(checkedData);
	}
	catch (const std::exception&) {
		isNum = false;
	}
	return isNum ? Types::Type::INT : Types::Type::STRING;
}