#pragma once
#include <vector>
#include "windows.h"
#include "Config.h"
#include <map>
#include <vector>
#include <optional>
#include <string>
#include "Types.h"

class Drawer {
public:
	Drawer();
	Drawer(Types::Type freqType);
	Drawer(Types::VisualizationType freqType);
	Drawer(Types::VisualizationType visualType, Types::Type freqType);
	void drawPlot(std::vector<char*>, HDC hdc, HWND hwnd);
private:
	std::optional<std::map<std::string, int>> countFreq(std::vector<char*>);
	void setConfigValues();
	void drawAxes(HDC hdc, HWND hwnd, std::map<std::string, int> frequency);
	void drawBars(HDC hdc, HWND hwnd, std::map<std::string, int> frequency);
	void drawVals(HDC hdc, HWND hwnd, std::map<std::string, double> vals);
	int chartWidth;
	int chartHeight;
	int chartX;
	int chartY;
	Types::Type typeOfData;
	Types::VisualizationType type;
	void initConfig();
	Config drawerConfig;
};