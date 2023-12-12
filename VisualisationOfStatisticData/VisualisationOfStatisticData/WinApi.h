#pragma once
#include <vector>
#include "windows.h"
#include <string>
#include "Constants.h"
#include "Types.h"
#include "DataCatcher.h"

class WinApi {
public:
	static WinApi& getInstance()
	{
		static WinApi    instance;
		return instance;
	}
	WinApi(WinApi const&) = delete;
	struct PositionData {
		int x;
		int y;
		int width;
		int height;
	};
	void DrawVisualization(std::vector<char*> data, HDC hdc, HWND hwnd);
	void DrawVisualization(std::vector<char*> data, HDC hdc, HWND hwnd, Types::VisualizationType type);
	void DrawVisualization(std::vector<char*> data, HDC hdc, HWND hwnd, Types::VisualizationType type, Types::Type typeOfData);
	std::vector<char*> HookData(HWND hwnd);
	DataCatcher::Data2 HookData(HWND hwnd, void* useless);
	void UpdateLabelValue(HWND label, HWND trackBar);
	BOOL InitComponent(DWORD style, HWND window, HWND& hComponent, const wchar_t* className, PositionData data, const wchar_t* initText);
	BOOL InitComponent(HINSTANCE hInstance, HWND window, HWND& hComponent, DWORD type, const wchar_t* label, DWORD command, PositionData data);
	std::wstring OpenFileForm(HWND hwnd);
private:
	HWND hLowerBoundEdit;
	HWND hUpperBoundEdit;
	bool isBounds;
	WinApi() {};
	std::pair<int, int> bounds;
};
