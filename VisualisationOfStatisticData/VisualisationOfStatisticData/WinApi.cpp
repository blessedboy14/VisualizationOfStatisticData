#include "WinApi.h"
#include "Constants.h"
#include "Types.h"
#include "Drawer.h"
#include "DataGenerator.h"
#include <CommCtrl.h>

void WinApi::DrawVisualization(std::vector<char*> data, HDC hdc, HWND hwnd) {
    Drawer* drawer = new Drawer(Types::Type::STRING);
    drawer->drawPlot(data, hdc, hwnd);
    delete drawer;
}

void WinApi::DrawVisualization(std::vector<char*> data, HDC hdc, HWND hwnd, Types::VisualizationType type) {
    Drawer* drawer = new Drawer(type);
    drawer->drawPlot(data, hdc, hwnd);
    delete drawer;
}

void WinApi::DrawVisualization(std::vector<char*> data, HDC hdc, HWND hwnd, Types::VisualizationType type, Types::Type typeOfData) {
    Drawer* drawer = new Drawer(type, typeOfData);
    drawer->drawPlot(data, hdc, hwnd);
    delete drawer;
}

std::wstring WinApi::OpenFileForm(HWND hwnd)
{
    OPENFILENAME ofn;
    wchar_t fileName[MAX_PATH] = L"";
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_DONTADDTORECENT | OFN_PATHMUSTEXIST;
    if (GetOpenFileName(&ofn) == TRUE)
    {
        std::wstring filePath = fileName;
        std::wstring extension = L".txt";

        if (filePath.length() < extension.length() ||
            filePath.substr(filePath.length() - extension.length()) != extension)
        {
            filePath += extension;
        }
        return filePath;
    }
    return L"";
}

std::vector<char*> WinApi::HookData(HWND hwnd) {
    DataCatcher::Data2 dataFile;
    std::wstring fileName = OpenFileForm(hwnd);
    std::vector<char*> data;
    if (!fileName.empty()) {
        DataCatcher* catcher = new DataCatcher();
        dataFile = catcher->GetData(fileName.c_str());
        data = dataFile.charData;
        delete catcher;
        MessageBox(NULL, L"Data successfully read from file", L"Success", MB_OK);
    }
    return data;
}

DataCatcher::Data2 WinApi::HookData(HWND hwnd, void* useless) {
    DataCatcher::Data2 dataFile;
    std::wstring fileName = OpenFileForm(hwnd);
    if (!fileName.empty()) {
        DataCatcher* catcher = new DataCatcher();
        dataFile = catcher->GetData(fileName.c_str());
        delete catcher;
        MessageBox(NULL, L"Data successfully read from file", L"Success", MB_OK);
    }
    return dataFile;
}

void WinApi::UpdateLabelValue(HWND label, HWND trackBar) {
    int value = SendMessage(trackBar, TBM_GETPOS, 0, 0);
    wchar_t labelText[32];
    swprintf_s(labelText, L"Value: %d", value);
    SetWindowText(label, labelText);
}

BOOL WinApi::InitComponent(HINSTANCE hInstance, HWND window, HWND& hComponent, DWORD type, const wchar_t* label, DWORD command, PositionData data) {
    hComponent = CreateWindowW(
        L"BUTTON",
        label,
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | type,
        data.x, data.y, data.width, data.height,
        window,
        (HMENU)command,
        hInstance,
        NULL
    );
    if (hComponent == NULL)
    {
        return FALSE;
    }
    return TRUE;
}

BOOL WinApi::InitComponent(DWORD style, HWND window, HWND& hComponent, const wchar_t* className, PositionData data, const wchar_t* initText)
{
    hComponent = CreateWindowW(
        className,
        initText,
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | style,
        data.x, data.y, data.width, data.height,
        window,
        NULL,
        NULL,
        NULL
    );
    if (hComponent == NULL)
    {
        return FALSE;
    }
    return TRUE;
}