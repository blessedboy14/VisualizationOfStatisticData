#pragma once
#include "Config.h"

class ConfigReader {
public:
	Config readConfig(const wchar_t* path);
};