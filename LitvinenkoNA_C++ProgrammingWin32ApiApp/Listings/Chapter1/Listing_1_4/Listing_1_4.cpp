#include "Listing_1_4.h"

const int MAX_LOADSTRING = 100;

HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

const TCHAR* r_str = L"������ ������ ������ ����";
const TCHAR* l_str = L"������ ����� ������ ����";
const TCHAR* r_s_str = L"������ ������ ������ ���� + shift";
const TCHAR* l_s_str = L"������ ����� ������ ���� + shift";
const TCHAR* r_c_str = L"������ ������ ������ ���� + ctrl";
const TCHAR* l_c_str = L"������ ����� ������ ���� + ctrl";
const TCHAR* r_a_str = L"������ ������ ������ ���� + alt";
const TCHAR* l_a_str = L"������ ����� ������ ���� + alt";

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
    WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    switch (message)
    {
    case WM_RBUTTONDOWN:
    case WM_LBUTTONDOWN:
    {
        const __int64 i64LParam = static_cast<unsigned __int64>(lParam);
        const int x = static_cast<int>(i64LParam & 0xffff);
        const int y = static_cast<int>((i64LParam >> 16) & 0xffff);
        hdc = GetDC(hWnd);
        const bool isLButton = message == WM_LBUTTONDOWN;
        if (MK_SHIFT & wParam)
            TextOutW(hdc, x, y, isLButton ? l_s_str : r_s_str,
                static_cast<int>(_tcsclen(isLButton ? l_s_str : r_s_str)));
        else if (MK_CONTROL & wParam)
            TextOutW(hdc, x, y, isLButton ? l_c_str : r_c_str,
                static_cast<int>(_tcsclen(isLButton ? l_c_str : r_c_str)));
        else if (MK_ALT & wParam)
            TextOutW(hdc, x, y, isLButton ? l_a_str : r_a_str,
                static_cast<int>(_tcsclen(isLButton ? l_a_str : r_a_str)));
        else
            TextOutW(hdc, x, y, isLButton ? l_str : r_str, 
                static_cast<int>(_tcsclen(isLButton ? l_str : r_str)));
        ReleaseDC(hWnd, hdc);
        break;
    }
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
        InvalidateRect(hWnd, nullptr, TRUE);
        break;
    case WM_DESTROY: PostQuitMessage(0);
        break;
    default: 
        return DefWindowProcW(hWnd, message, wParam, lParam);
    }
    return 0;
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
	wcex.hIcon = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_LISTING14));
	wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LISTING14);
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
    LoadStringW(hInstance, IDC_LISTING14, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAcceleratorsW(hInstance, MAKEINTRESOURCE(IDC_LISTING14));

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
