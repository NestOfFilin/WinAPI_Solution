#include "Listing_1_15.h"

const int MAX_LOADSTRING = 100;

HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

const int SCALE = 1000;
const int MARK = 4;

int sx, sy;
POINT point;

POINT pt[20];
int count = 0;
int index;

bool capture = false;
HPEN hDashPen, hBezierPen;
HBRUSH hRectBrush, hSelBrush;

void DcInLp(POINT& point, const int sx, const int sy)
{
    point.x = point.x * SCALE / sx;
    point.y = SCALE - point.y * SCALE / sy;
}

void Transform(HDC & hdc, const int sx, const int sy)
{
    SetMapMode(hdc, MM_ANISOTROPIC);
    SetWindowExtEx(hdc, SCALE, -SCALE, nullptr);
    SetViewportExtEx(hdc, sx, sy, nullptr);
    SetViewportOrgEx(hdc, 0, sy, nullptr);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
    WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        std::ifstream in;
        in.open("dat.txt");
        if (in.fail())
        {
            MessageBoxW(hWnd, L"Файл dat.txt не найден",
                L"Открытие файла", MB_OK | MB_ICONEXCLAMATION);
            PostQuitMessage(0);
            return 1;
        }

        for (count = 0; in >> pt[count].x; count++)
            in >> pt[count].y;
        in.close();

        hDashPen = CreatePen(PS_DASH, 1, 0);
        hBezierPen = CreatePen(PS_SOLID, 4, RGB(255, 0, 255));
        hRectBrush = CreateSolidBrush(RGB(128, 0, 128));
        hSelBrush = CreateSolidBrush(RGB(255, 0, 0));
        break;
    }
    case WM_SIZE:
    {
        sx = LOWORD(lParam);
        sy = HIWORD(lParam);
        break;
    }
    case WM_LBUTTONDOWN:
    {
        point.x = LOWORD(lParam);
        point.y = HIWORD(lParam);
        // Преобразование экранных координат курсора в логические
        DcInLp(point, sx, sy);
        RECT rt;
        for (int i = 0; i < count; i++)
        {
            SetRect(&rt, pt[i].x - MARK, pt[i].y - MARK, pt[i].x + MARK, pt[i].y + MARK);
            if (PtInRect(&rt, point))
            { // Курсор попал в точку
                index = i;
                capture = true;
                HDC hdc = GetDC(hWnd);
                Transform(hdc, sx, sy); // Переход в логические координаты
                FillRect(hdc, &rt, hSelBrush);
                ReleaseDC(hWnd, hdc);
                SetCapture(hWnd);
                break;
            }
        }
        break;
    }
    case WM_LBUTTONUP:
    {
        if (capture)
        {
            ReleaseCapture();
            capture = false;
        }
        break;
    }
    case WM_MOUSEMOVE:
    {
        if (capture)
        {
            point.x = LOWORD(lParam);
            point.y = HIWORD(lParam);
            // Преобразование экранных координат курсора в логические
            DcInLp(point, sx, sy);
            pt[index] = point;
            InvalidateRect(hWnd, nullptr, TRUE);
        }
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        Transform(hdc, sx, sy);

        SelectObject(hdc, hDashPen);
        Polyline(hdc, pt, count);
        SelectObject(hdc, hBezierPen);
        // Число точек для PolyBezier должно быть строго определенным, оно должно быть кратно 3 (i * 3) + 1
        // иначе кривая не будет нарисована и через GetLastError() получим код ошибки
        PolyBezier(hdc, pt, count);

        RECT rt;
        for (int i = 0; i < count; i++)
        {
            SetRect(&rt, pt[i].x - MARK, pt[i].y - MARK, pt[i].x + MARK, pt[i].y + MARK);
            FillRect(hdc, &rt, (capture && i == index) ? hSelBrush : hRectBrush);
        }

        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
    {
        DeleteObject(hDashPen);
        DeleteObject(hBezierPen);
        DeleteObject(hRectBrush);
        DeleteObject(hSelBrush);

        std::ofstream out;
        out.open("dat.txt");
        for (int i = 0; i < count; i++)
            out << pt[i].x << '\t' << pt[i].y << '\n';
        out.close();

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
    wcex.hIcon = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_LISTING115));
    wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LISTING115);
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
    LoadStringW(hInstance, IDC_LISTING115, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAcceleratorsW(hInstance, MAKEINTRESOURCE(IDC_LISTING115));

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
