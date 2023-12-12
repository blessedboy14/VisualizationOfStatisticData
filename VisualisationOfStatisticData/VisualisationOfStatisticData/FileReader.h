#pragma once
#include "windows.h"
#include <vector>
#include <string>

class FileReader
{
public:
	FileReader(const wchar_t* fileName);
	std::vector<char> readAll();
	~FileReader();
	std::vector<std::string> readAllToVector();
	std::vector<char*> readAllToChar();
private:
	HANDLE file;
	bool isOpened = false;
	FileReader() {};
};