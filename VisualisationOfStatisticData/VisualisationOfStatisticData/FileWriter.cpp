#include "FileWriter.h"
#include "framework.h"
#include <stdexcept>

FileWriter::FileWriter(const wchar_t* fileName) {
	FileWriter::file = CreateFileW(fileName, (GENERIC_WRITE | GENERIC_READ), 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == NULL) {
		CloseHandle(FileWriter::file);
		throw std::exception("File not found");
	}
	FileWriter::isOpened = true;
}

void  FileWriter::writeAll(std::vector<std::string> data) {
	if (FileWriter::isOpened) {
		DWORD bytesWritten;
		wchar_t* fileData;
		std::string assembleString;
		for (const std::string& str : data) {
			assembleString += str + " ";
		}
		if (!WriteFile(FileWriter::file, assembleString.c_str(), assembleString.size(), &bytesWritten, NULL)) {
			throw std::exception("Error while writing file");
		}
	}
}

void FileWriter::write(char** data, int length) {
	if (FileWriter::isOpened) {
		for (int i = 0; i < length; ++i) {
			DWORD bytesWritten = 0;
			DWORD stringLength = static_cast<DWORD>(strlen(data[i]));
			if (!WriteFile(FileWriter::file, data[i], stringLength, &bytesWritten, NULL) || bytesWritten != stringLength)
			{
				throw std::exception("Error while writing file");
			}
			if (i != length - 1)
			{
				const char space = ' ';
				if (!WriteFile(FileWriter::file, &space, sizeof(space), &bytesWritten, NULL) || bytesWritten != sizeof(space))
				{
					throw std::exception("Error while writing file");
				}
			}
		}
	}
}

FileWriter::~FileWriter() {
	CloseHandle(FileWriter::file);
}