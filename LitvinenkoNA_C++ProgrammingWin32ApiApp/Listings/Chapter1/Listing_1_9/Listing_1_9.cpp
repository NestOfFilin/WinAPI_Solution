#include "Listing_1_9.h"

const int MAX_LOADSTRING = 100;

HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

const int aPensSize = 3;
HPEN aPens[aPensSize];
const int WIDTH = 314;
const int HEIGHT = 200;
constexpr double dHEIGHT = static_cast<double>(HEIGHT);
double sx, sy;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
    WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        aPens[0] = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
        aPens[1] = CreatePen(PS_SOLID, 2, RGB(0, 127, 0));
        aPens[2] = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
        break;
    }
    case WM_SIZE:
    {
        sx = static_cast<double>(LOWORD(lParam));
        sy = static_cast<double>(HIWORD(lParam));
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // Установка режима
        SetMapMode(hdc, MM_ANISOTROPIC);
        // Установка логических размеров вывода, ось y напрвлена вверх
        SetWindowExtEx(hdc, 2 * WIDTH, -2 * HEIGHT, nullptr);

        const int iSx = static_cast<int>(sx);
        const int iSy = static_cast<int>(sy);
        // Установка физических размеров на четверть окна
        SetViewportExtEx(hdc, iSx / 2, iSy / 2, nullptr);
        // Установка начала координат
        SetViewportOrgEx(hdc, iSx / 4, iSy / 4, nullptr);

        SelectObject(hdc, aPens[2]); // Синее перо
        MoveToEx(hdc, -WIDTH, 0, nullptr);
        LineTo(hdc, WIDTH, 0);
        MoveToEx(hdc, 0, HEIGHT, nullptr);
        LineTo(hdc, 0, -HEIGHT);

        SelectObject(hdc, aPens[0]); // Красное перо
        MoveToEx(hdc, -WIDTH, 0, nullptr);
        int xScr = -WIDTH;
        for (double x = -M_PI; x < M_PI; x += 0.03, xScr += 3)
        {
            const int yScr = static_cast<int>(dHEIGHT * sin(x));
            LineTo(hdc, xScr, yScr);
        }

        // Перенос начала координат
        SetViewportOrgEx(hdc, iSx * 3 / 4, iSy / 4, nullptr);
        SelectObject(hdc, GetStockObject(BLACK_PEN)); // Чёрное перо
        // Рисуем координатные оси
        MoveToEx(hdc, -WIDTH, 0, nullptr);
        LineTo(hdc, WIDTH, 0);
        MoveToEx(hdc, 0, HEIGHT, nullptr);
        LineTo(hdc, 0, -HEIGHT);

        SelectObject(hdc, aPens[1]); // Зелёное перо
        MoveToEx(hdc, 0, 0, nullptr);
        constexpr double PI_m_2 = 2.0 * M_PI;
        const double K = 4.0;
        for (double angle = 0.0; angle < PI_m_2; angle += 0.02)
        {
            const double sinKAngle = dHEIGHT * sin(K * angle);
            const int xScr = static_cast<int>(sinKAngle * cos(angle));
            const int yScr = static_cast<int>(sinKAngle * sin(angle));
            LineTo(hdc, xScr, yScr);
        }

        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
    {
        for (int i = 0; i < aPensSize; i++)
            DeleteObject(aPens[i]);
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
    wcex.hIcon = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_LISTING19));
    wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LISTING19);
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
    LoadStringW(hInstance, IDC_LISTING19, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAcceleratorsW(hInstance, MAKEINTRESOURCE(IDC_LISTING19));

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
