#include "Exercise_1_5.h"


const int MAX_LOADSTRING = 100;

HINSTANCE hInst = nullptr;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name


const TCHAR* sMsgBoxTitle = L"Глава 1 Задание 5";
const TCHAR* sMsgBoxDesc =
L"Программа с периодичностью в 0,1 секунды заполняет окно прямоугольниками случайного\n\
размера (не превосходящими 1/4 окна) и случайного цвета.";


int sx, sy, sx_2, sy_2;
unsigned __int64 timerID = 1U;


static RECT s_RectStruct;
static HBRUSH s_RectBrush = nullptr;


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        MessageBoxW(hWnd, sMsgBoxDesc, sMsgBoxTitle, MB_OK | MB_ICONQUESTION);
        SetTimer(hWnd, timerID, 100U, nullptr);
        srand(static_cast<unsigned>(time(0U)));
        break;
    }
    case WM_SIZE:
    {
        sx = LOWORD(lParam);
        sy = HIWORD(lParam);
        sx_2 = sx / 2;
        sy_2 = sy / 2;
        break;
    }
    case WM_TIMER:
    {
        const int width{rand() % (sx_2 - 1) + 1};
        const int left{ rand() % (sx - width) };

        const int height{ rand() % (sy_2 - 1) + 1 };
        const int top{ rand() % (sy - height) };

        s_RectStruct = RECT{ left , top, left + width, top + height };

        s_RectBrush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));

        HDC hDC = GetDC(hWnd);
        FillRect(hDC, &s_RectStruct, s_RectBrush);
        ReleaseDC(hWnd, hDC);

        DeleteObject(s_RectBrush);
        s_RectBrush = nullptr;
        break;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    }
    default:
        return DefWindowProcW(hWnd, message, wParam, lParam);
    }
    return 0i64;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_EXERCISE15));
    wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_EXERCISE15);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIconW(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    HWND hWnd = CreateWindowExW(0L, szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_EXERCISE15, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAcceleratorsW(hInstance, MAKEINTRESOURCE(IDC_EXERCISE15));

    MSG msg;

    // Main message loop:
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        if (!TranslateAcceleratorW(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    return static_cast<int>(msg.wParam);
}
