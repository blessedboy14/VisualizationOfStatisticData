#include "FileReader.h"
#include "framework.h"
#include <stdexcept>
#include "CharToVectorConverter.h"
#include <sstream>

FileReader::FileReader(const wchar_t* fileName) {
	FileReader::file = CreateFileW(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == NULL) {
		CloseHandle(FileReader::file);
		throw std::exception("File not found");
	}
	FileReader::isOpened = true;
}

std::vector<char> FileReader::readAll() {
	if (isOpened) {
		DWORD fileSize;
		fileSize = GetFileSize(FileReader::file, NULL);
		DWORD bytesRead;
		std::vector<char> buffer(fileSize + 1);
		if (!ReadFile(FileReader::file, buffer.data(), fileSize, &bytesRead, NULL)) 
		{
			CloseHandle(FileReader::file);
			throw std::exception("Error while reading file");
		}
		return buffer;
	} else {
		std::vector<char> vect;
		return vect;
	}
}

std::vector<std::string> FileReader::readAllToVector() {
	std::vector<char> data = FileReader::readAll();
	std::vector<std::string> vectorData = CharToVectorConverter::convert(data);
	return vectorData;
}

std::vector<char*> FileReader::readAllToChar() {
	std::vector<char> buffer = FileReader::readAll();
	std::vector<char*> elements;
	std::istringstream iss(buffer.data());
	std::string element;
	while (std::getline(iss, element, ' '))
	{
		char* elementPtr = new char[element.size() + 1];
		strcpy_s(elementPtr, element.size() + 1, element.c_str());
		elements.push_back(elementPtr);
	}
	return elements;
}

FileReader::~FileReader() {
	CloseHandle(FileReader::file);
}