#include "Drawer.h"
#include "ConfigReader.h"
#include "FrequencyCounter.h"
#include <algorithm>
#include "StatisticDataResolver.hpp"
#include <sstream>
#include <iomanip>

Drawer::Drawer() {
    Drawer::typeOfData = Types::Type::INT;
	Drawer::initConfig();
}

Drawer::Drawer(Types::Type freqType) {
    Drawer::typeOfData = freqType;
    Drawer::initConfig();
}

Drawer::Drawer(Types::VisualizationType freqType) {
    Drawer::type = freqType;
    Drawer::initConfig();
}

Drawer::Drawer(Types::VisualizationType freqType, Types::Type type) {
    Drawer::typeOfData = type;
    Drawer::type = freqType;
    Drawer::initConfig();
}

void Drawer::initConfig() {
	ConfigReader* reader = new ConfigReader();
	Drawer::drawerConfig = reader->readConfig(L"DrawerConfig.ini");
    Drawer::setConfigValues();
}

void Drawer::setConfigValues() {
    Drawer::chartWidth = Drawer::drawerConfig.chartWidth;
    Drawer::chartHeight = Drawer::drawerConfig.chartHeight;
    Drawer::chartX = Drawer::drawerConfig.chartX;
    Drawer::chartY = Drawer::drawerConfig.chartY;
}

//std::optional<std::map<std::string, int>> Drawer::countFreq(std::vector<char*> data) {
//    FrequencyCounter* counter = new FrequencyCounter();
//    std::map<std::string, int> frequency;
//    try {
//        if (Drawer::typeOfData == Types::Type::INT) {
//            frequency = counter->countFrequencyEach(data);
//        }
//        else if (Drawer::typeOfData == Types::Type::STRING) {
//            frequency = counter->countAlphabetFrequency(data);
//        }
//        else if (Drawer::typeOfData == Types::Type::WORDS) {
//            frequency = counter->countWordsFrequency(data);
//        }
//        return frequency;
//    }
//    catch (std::exception e) {
//        MessageBox(NULL, L"Error when counting frequency", L"Error", MB_ICONERROR);
//        return std::nullopt;
//    }
//}

std::optional<std::map<std::string, int>> Drawer::countFreq(std::vector<char*> data) {
    FrequencyCounter* counter = new FrequencyCounter();
    std::map<std::string, int> frequency;
    try {
        if (Drawer::type == Types::VisualizationType::CHAR) {
            frequency = counter->countAlphabetFrequency(data);
        }
        else if (Drawer::type == Types::VisualizationType::WORD) {
            frequency = counter->countWordsFrequency(data);
        }
        return frequency;
    }
    catch (std::exception e) {
        MessageBox(NULL, L"Error when counting frequency", L"Error", MB_ICONERROR);
        return std::nullopt;
    }
}

bool compareByValue(const std::pair<int, int>& a, const std::pair<int, int>& b) {
    return a.second > b.second;
}

void Drawer::drawPlot(std::vector<char*> data, HDC hdc, HWND hWnd) {
    SetBkMode(hdc, TRANSPARENT);
    std::optional<std::map<std::string, int>> result = Drawer::countFreq(data);
    if (!result) {
        return;
    }
    std::map<std::string, int> frequency = result.value(); 
    Drawer::drawBars(hdc, hWnd, frequency);
    Drawer::drawAxes(hdc, hWnd, frequency);
    std::map<std::string, double> mathParams;
    if (Drawer::typeOfData == Types::Type::INT) {
        mathParams = StatisticResolver::countMathParams(frequency);
    }
    if (mathParams.size() > 0) {
        drawVals(hdc, hWnd, mathParams);
    }
}

void Drawer::drawVals(HDC hdc, HWND hwnd, std::map<std::string, double> vals) {
    int x = Drawer::chartX - 220;
    int y = Drawer::chartY + Drawer::chartHeight - 40;
    int i = 0;
    HFONT myFont = CreateFontA(38, 0, 0, 0, 800, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Death Note");
    SelectObject(hdc, myFont);
    for (const auto& pair : vals) {
        RECT textRect;
        textRect.left = x;
        textRect.right = x + 200;
        textRect.top = y - i * 30;
        textRect.bottom = textRect.top - 27;
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(3) << pair.second;
        std::string text = pair.first + " " + oss.str();
        DrawTextA(hdc, text.c_str(), -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        i++;
    }
    DeleteObject(myFont);
}

void Drawer::drawBars(HDC hdc, HWND hwnd, std::map<std::string, int> frequency) {
    auto maxElement = std::max_element(frequency.begin(), frequency.end(),
        [](const auto& pair1, const auto& pair2) {
            return pair1.second < pair2.second;
        });
    int barWidth = Drawer::chartWidth / frequency.size();
    int i = 0;
    for (const auto& pair : frequency)
    {
        int barHeight = (pair.second * Drawer::chartHeight) / maxElement->second;
        RECT barRect, textRect;
        barRect.left = Drawer::chartX + i * barWidth;
        barRect.top = Drawer::chartY + Drawer::chartHeight - barHeight;
        barRect.right = barRect.left + barWidth;
        barRect.bottom = Drawer::chartY + chartHeight;
        textRect.left = barRect.left;
        textRect.right = barRect.right;
        textRect.bottom = barRect.top;
        textRect.top = textRect.bottom - 30;
        std::wstring label = std::to_wstring(pair.second);
        HBRUSH brush = CreateSolidBrush(RGB(255 - i * 20, i * 30, i * 40));
        FillRect(hdc, &barRect, brush);
        DrawText(hdc, label.c_str(), -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        DeleteObject(brush);
        i++;
    }
}

void Drawer::drawAxes(HDC hdc, HWND hwnd, std::map<std::string, int> frequency) {
    int barWidth = Drawer::chartWidth / frequency.size();
    MoveToEx(hdc, Drawer::chartX, Drawer::chartY + Drawer::chartHeight, NULL);
    LineTo(hdc, Drawer::chartX + Drawer::chartWidth, Drawer::chartY + Drawer::chartHeight);
    MoveToEx(hdc, Drawer::chartX, Drawer::chartY, NULL);
    LineTo(hdc, Drawer::chartX, Drawer::chartY + Drawer::chartHeight);
    int i = 0;
    for (const auto& pair : frequency)
    {
        std::wstring label(pair.first.begin(), pair.first.end());
        RECT labelRect;
        labelRect.left = Drawer::chartX + i * barWidth;
        labelRect.top = Drawer::chartY + Drawer::chartHeight + 2;
        labelRect.right = labelRect.left + barWidth;
        labelRect.bottom = labelRect.top + 20;
        DrawText(hdc, label.c_str(), -1, &labelRect, DT_CENTER | DT_VCENTER);
        i++;
    }
}