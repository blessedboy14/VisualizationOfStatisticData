#include "ConfigReader.h"
#include <Windows.h>
#include <string>
#include <iostream>
#include <filesystem>

Config ConfigReader::readConfig(const wchar_t* path) {
    LPCWSTR filePath = path;
    LPCWSTR section = L"AppConfig";
    LPCWSTR keys[] = {
        L"chartHeight",
        L"chartWidth",
        L"chartX",
        L"chartY",
    };
    Config config;
    wchar_t buffer[256];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::wstring fullPath(buffer);
    fullPath = fullPath.substr(0, fullPath.find_last_of(L"\\"));
    fullPath = fullPath.substr(0, fullPath.find_last_of(L"\\"));
    fullPath = fullPath.substr(0, fullPath.find_last_of(L"\\"));
    std::wstring projectName = fullPath.substr(fullPath.find_last_of(L"\\") + 1);
    std::wstring actualPath = fullPath + L"\\" + projectName + L"\\" + path;
    const wchar_t* cStylePath = actualPath.c_str();
    config.chartHeight = GetPrivateProfileInt(section, keys[0], 0, cStylePath);
    config.chartWidth = GetPrivateProfileInt(section, keys[1], 0, cStylePath);
    config.chartX = GetPrivateProfileInt(section, keys[2], 0, cStylePath);
    config.chartY = GetPrivateProfileInt(section, keys[3], 0, cStylePath);
    return config;
}