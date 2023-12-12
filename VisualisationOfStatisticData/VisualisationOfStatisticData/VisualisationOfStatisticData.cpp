// VisualisationOfStatisticData.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "VisualisationOfStatisticData.h"
#include "DataGenerator.h"
#include "commdlg.h"
#include "windows.h"
#include <commctrl.h>
#include "Constants.h"
#include "Drawer.h"
#include "Types.h"
#include "WinApi.h"

//Buttons
HWND generateButton;
HWND hookButton;
HWND visualizeButton;

//checkBoxes
HWND strTypeCheckBox;
HWND intTypeCheckBox;

HWND charStyleCheckBox, wordStyleCheckBox;

//trackBar
HWND amountTrackBar;

//labels
HWND trackBarLabel;

//edits
HWND hLowerBoundEdit, hUpperBoundEdit;

HINSTANCE hInst; 
WCHAR szTitle[MAX_LOADSTRING];   
WCHAR szWindowClass[MAX_LOADSTRING];         
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void                GenerateData(HWND hWnd);
INT_PTR CALLBACK    DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

std::vector<char*> data;
Types::Type inputType;
bool isPaint = false;
std::pair<int, int> bounds;
bool isBounds = false;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_VISUALISATIONOFSTATISTICDATA, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_VISUALISATIONOFSTATISTICDATA));
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_VISUALISATIONOFSTATISTICDATA));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_VISUALISATIONOFSTATISTICDATA);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitButtons(HINSTANCE hInstance, HWND window) {
    WinApi::PositionData data = { 10, 10, 110, 50 };
    if (WinApi::getInstance().InitComponent(hInstance, window, generateButton, BS_DEFPUSHBUTTON, L"Generate Data", GENERATE_DATA, data) != TRUE) return FALSE;

    data = { data.x + data.width + 10, data.y, 70, 20};
    if (WinApi::getInstance().InitComponent(hInstance, window, intTypeCheckBox, BS_AUTORADIOBUTTON | WS_GROUP, L"Int", INT_TYPE, data) != TRUE) return FALSE;
    SendMessage(intTypeCheckBox, BM_SETCHECK, BST_CHECKED, 0);

    data = { data.x, data.y + data.height + 10, data.width, data.height};
    if (WinApi::getInstance().InitComponent(NULL, window, strTypeCheckBox, BS_AUTORADIOBUTTON, L"String", STRING_TYPE, data) != TRUE) return FALSE;

    data = { data.x + data.width + 5, 10 + 5, 210, 40 };
    if (WinApi::getInstance().InitComponent(0, window, amountTrackBar, TRACKBAR_CLASS, data, L"") != TRUE) return FALSE;
    SendMessage(amountTrackBar, TBM_SETRANGEMIN, TRUE, MIN_TRACK_BAR);
    SendMessage(amountTrackBar, TBM_SETRANGEMAX, TRUE, MAX_TRACK_BAR);
    SendMessage(amountTrackBar, TBM_SETPOS, TRUE, MAX_TRACK_BAR / 2);

    data = { data.x + data.width + 5, 10 + 5, 50, 40 };
    if (WinApi::getInstance().InitComponent(0, window, trackBarLabel, L"STATIC", data, L"Value: 0") != TRUE) return FALSE;
    WinApi::getInstance().UpdateLabelValue(trackBarLabel, amountTrackBar);

    data = { 10 + 110 + 10, 103 + data.height, 70, 20 };
    if (WinApi::getInstance().InitComponent(hInstance, window, wordStyleCheckBox, BS_AUTORADIOBUTTON | WS_GROUP, L"Word", WORD_TYPE, data) != TRUE) return FALSE;
    SendMessage(wordStyleCheckBox, BM_SETCHECK, BST_CHECKED, 0);

    data = { data.x, data.y + data.height + 10, data.width, data.height };
    if (WinApi::getInstance().InitComponent(NULL, window, charStyleCheckBox, BS_AUTORADIOBUTTON, L"Char", CHAR_TYPE, data) != TRUE) return FALSE;

    data = { 10, 10 + 65, 110, 50 };
    if (WinApi::getInstance().InitComponent(hInstance, window, hookButton, BS_DEFPUSHBUTTON, L"GetData", HOOK_DATA, data) != TRUE) return FALSE;

    data = { 10, data.y + data.height + 15, data.width, data.height };
    if (WinApi::getInstance().InitComponent(hInstance, window, visualizeButton, BS_DEFPUSHBUTTON, L"Visualize", VISUALIZE, data) != TRUE) return FALSE;
    return TRUE;

}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
   if (!hWnd)
   {
      return FALSE;
   }
   if (InitButtons(hInst, hWnd) == FALSE) {
       return FALSE;
   }
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_HSCROLL:
    {
        int x = 0;
        if (lParam == reinterpret_cast<LPARAM>(amountTrackBar))
        {
            WinApi::getInstance().UpdateLabelValue(trackBarLabel, amountTrackBar);
        }
        break;
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case GENERATE_DATA:
            {
                Types::Type type = IsDlgButtonChecked(hWnd, INT_TYPE) ? Types::Type::INT : Types::Type::STRING;
                if (type == Types::Type::INT) {
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_BOUNDS), hWnd, DialogProc);
                }
                GenerateData(hWnd);
                isBounds = false;
                break;
            }
            case HOOK_DATA:
            {
                DataCatcher::Data2 info = WinApi::getInstance().HookData(hWnd, nullptr);
                data = info.charData;
                inputType = info.type;
                if (inputType == Types::Type::INT) {
                    if (IsDlgButtonChecked(hWnd, CHAR_TYPE)) {
                        SendMessage(charStyleCheckBox, BM_SETCHECK, BST_UNCHECKED, 0);
                        SendMessage(wordStyleCheckBox, BM_SETCHECK, BST_CHECKED, 0);
                    }
                    EnableWindow(charStyleCheckBox, FALSE);
                } else {
                    SendMessage(charStyleCheckBox, BM_SETCHECK, BST_UNCHECKED, 0);
                    SendMessage(wordStyleCheckBox, BM_SETCHECK, BST_CHECKED, 0);
                    EnableWindow(charStyleCheckBox, TRUE);
                }
                break;
            }
            case VISUALIZE:
            {
                if (!data.empty()) {
                    isPaint = true;
                    InvalidateRect(hWnd, NULL, TRUE);
                }
                break;
            }
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            if (isPaint) {
                Types::VisualizationType type = IsDlgButtonChecked(hWnd, CHAR_TYPE) ? 
                    Types::VisualizationType::CHAR : Types::VisualizationType::WORD;
                WinApi::getInstance().DrawVisualization(data, hdc, hWnd, type, inputType);
                isPaint = false;
            }
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void GenerateData(HWND hWnd) {
    int trackPos = SendMessage(amountTrackBar, TBM_GETPOS, 0, 0);
    Types::Type type = IsDlgButtonChecked(hWnd, INT_TYPE) ? Types::Type::INT : Types::Type::STRING;
    std::wstring fileName = WinApi::getInstance().OpenFileForm(hWnd);
    if (!fileName.empty()) {
        if (isBounds) {
            DataGenerator* generator = new DataGenerator(trackPos, type, bounds.first, bounds.second);
            generator->generateAndWriteToFile(fileName.c_str());
            delete generator;
        }
        else {
            DataGenerator* generator = new DataGenerator(trackPos, type);
            generator->generateAndWriteToFile(fileName.c_str());
            delete generator;
        }
        MessageBox(NULL, L"Data successfully writed in file", L"Success", MB_OK);
    }
}

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
    {
        // Initialize the dialog box controls
        hLowerBoundEdit = GetDlgItem(hwndDlg, IDC_EDIT_LOWERBOUND);
        hUpperBoundEdit = GetDlgItem(hwndDlg, IDC_EDIT_UPPERBOUND);
        const char* labelText = "Choose bounds from -5000 to 1000001. Close if don't want to";
        SetDlgItemTextA(hwndDlg, IDC_MBOX_LABEL, labelText);
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
        {
            std::string lowerVal;
            std::string upperVal;
            GetWindowTextA(hLowerBoundEdit, const_cast<LPSTR>(lowerVal.c_str()), 256);
            GetWindowTextA(hUpperBoundEdit, const_cast<LPSTR>(upperVal.c_str()), 256);
            int lowerBound;
            int upperBound;
            try {
                int lowerBound = std::stoi(lowerVal);
                int upperBound = std::stoi(upperVal);
                if (lowerBound <= upperBound && lowerBound > -5000 && upperBound < 1000001)
                {
                    bounds = { lowerBound, upperBound };
                    isBounds = true;
                    EndDialog(hwndDlg, IDOK);
                }
                else
                {
                    MessageBox(hwndDlg, L"Invalid bounds!", L"Error", MB_OK | MB_ICONERROR);
                }
            }
            catch (std::exception e) {
                MessageBox(hwndDlg, L"Invalid input", L"Error", MB_OK | MB_ICONERROR);
            }
        }
        break;

        case IDCANCEL:
            EndDialog(hwndDlg, IDCANCEL);
            break;
        }
        break;
    }

    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
