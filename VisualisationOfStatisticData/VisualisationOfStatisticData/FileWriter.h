#pragma once
#include <vector>
#include "windows.h"
#include <string>

class FileWriter {
public:
	FileWriter(const wchar_t* fileName);
	void writeAll(std::vector<std::string> data);
	void write(char** data, int length);
	~FileWriter();
private:
	HANDLE file;
	bool isOpened = false;
	FileWriter() {};
};