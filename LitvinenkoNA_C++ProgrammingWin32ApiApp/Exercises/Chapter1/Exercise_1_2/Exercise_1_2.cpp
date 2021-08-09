#include "Exercise_1_2.h"

const int MAX_LOADSTRING = 100;

HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

RECT rect = { -20, 10, 20, -10 };

const TCHAR* sMsgBoxTitle = L"Глава 1 Задание 1";
const TCHAR* sMsgBoxDesc = L"Программа строит прямоугольник в центре окна.\n\
Размеры прямоугольника изменяются автоматически каждую секунду. \n\
При нажатии на левую кнпку мыши его размеры станут уменьшаться, а при нажатии на правую кнопку — увеличиваться на 10 пикселов.";

int sx, sy;
bool bIncreaseMode = true;
const int dRectSize = 10;

const int timerId = 1;
HBRUSH hRectBrush;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
    WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        MessageBoxW(hWnd, sMsgBoxDesc, sMsgBoxTitle, MB_OK | MB_ICONEXCLAMATION);

        SetTimer(hWnd, timerId, 1000, nullptr);
        hRectBrush = CreateSolidBrush(RGB(128, 0, 128));
        break;
    }
    case WM_SIZE:
    {
        sx = LOWORD(lParam);
        sy = HIWORD(lParam);
        break;
    }
    case WM_RBUTTONUP:
    {
        bIncreaseMode = true;
        break;
    }
    case WM_LBUTTONUP:
    {
        bIncreaseMode = false;
        break;
    }
    case WM_TIMER:
    {
        if (bIncreaseMode && ((rect.top - rect.bottom) + dRectSize < sy))
        {
            rect.left -= dRectSize;
            rect.top += dRectSize;

            rect.right += dRectSize;
            rect.bottom -= dRectSize;
            InvalidateRect(hWnd, nullptr, TRUE);
        }
        else if (!bIncreaseMode && ((rect.top - rect.bottom) - dRectSize > 10L))
        {
            rect.left += dRectSize;
            rect.top -= dRectSize;

            rect.right -= dRectSize;
            rect.bottom += dRectSize;
            InvalidateRect(hWnd, nullptr, TRUE);
        }
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        SetMapMode(hdc, MM_ANISOTROPIC);
        SetWindowExtEx(hdc, sx, -sy, nullptr);
        SetViewportExtEx(hdc, sx, sy, nullptr);
        SetViewportOrgEx(hdc, sx / 2, sy / 2, nullptr);

        HRGN hRect = CreateRectRgnIndirect(&rect);
        FillRgn(hdc, hRect, hRectBrush);
        DeleteObject(hRect);

        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
    {
        DeleteObject(hRectBrush);
        KillTimer(hWnd, timerId);
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
    wcex.hIcon = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_EXERCISE12));
    wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_EXERCISE12);
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
    LoadStringW(hInstance, IDC_EXERCISE12, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAcceleratorsW(hInstance, MAKEINTRESOURCE(IDC_EXERCISE12));

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
