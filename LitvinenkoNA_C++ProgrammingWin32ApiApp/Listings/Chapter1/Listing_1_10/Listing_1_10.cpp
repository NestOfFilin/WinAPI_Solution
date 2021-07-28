#include "Listing_1_10.h"

const int MAX_LOADSTRING = 100;

HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

const int hHatchBrushsSize = 6;
HBRUSH hSolidBrush, hHatchBrushs[hHatchBrushsSize];
const int aHatchBrushIndexes[] = {
    HS_BDIAGONAL,
    HS_CROSS,
    HS_DIAGCROSS,
    HS_FDIAGONAL,
    HS_HORIZONTAL,
    HS_VERTICAL
};
const TCHAR* str = L"сплошное заполнение";
const TCHAR* aStr[] = { 
    L"HS_BDIAGONAL Ч слева направо и снизу вверх",
    L"HS_CROSS Ч горизонтальна€ и вертикальна€ штриховка",
    L"HS_DIAGCROSS Ч под углом в 45 градусов",
    L"HS_FDIAGONAL Ч слева направо и сверху вниз",
    L"HS_HORIZONTAL Ч горизонтальна€ штриховка",
    L"HS_VERTICAL Ч вертикальна€ штриховка"
};
const COLORREF lHatchBrushColor = RGB(0, 128, 0);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
    WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        hSolidBrush = CreateSolidBrush(RGB(255, 255, 0));
        for (int i = 0; i < hHatchBrushsSize; i++)
            hHatchBrushs[i] = CreateHatchBrush(aHatchBrushIndexes[i], lHatchBrushColor);
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        
        SelectObject(hdc, hSolidBrush);
        Ellipse(hdc, 1, 1, 40, 40);
        TextOutW(hdc, 50, 11, str, static_cast<int>(_tcsclen(str)));
        for (int i = 0; i < hHatchBrushsSize; i++)
        {
            SelectObject(hdc, hHatchBrushs[i]);
            Rectangle(hdc, 1, 41 + i * 40, 40, 80 + i * 40);
            TextOut(hdc, 50, 51 + i * 40, aStr[i], static_cast<int>(_tcsclen(aStr[i])));
        }

        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
    {
        DeleteObject(hSolidBrush);
        for (int i = 0; i < hHatchBrushsSize; i++)
            DeleteObject(hHatchBrushs[i]);
        PostQuitMessage(0);
        break;
    }
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
    wcex.hIcon = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_LISTING110));
    wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LISTING110);
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
    LoadStringW(hInstance, IDC_LISTING110, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAcceleratorsW(hInstance, MAKEINTRESOURCE(IDC_LISTING110));

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
