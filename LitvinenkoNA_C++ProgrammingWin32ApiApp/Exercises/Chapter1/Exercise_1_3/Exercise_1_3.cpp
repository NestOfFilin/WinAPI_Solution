#include "Exercise_1_3.h"

const int MAX_LOADSTRING = 100;

HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

RECT rectEllips = { -20, 20, 20, -20 };
const int ballSpeed = 2;
double ellipsSpeedX = 0, ellipsSpeedY = 0;
double currentX = 0, currentY = 0;

const TCHAR* sMsgBoxTitle = L"Глава 1 Задание 3";
const TCHAR* sMsgBoxDesc = 
L"Программа с движущимся шариком по законам геометрической оптики.\n\
Начало движения происходит из точки, в которой нажимается левая кнопка мыши.\n\
Начальный угол определяется случайным образом.";

int sx, sy, sx_2, sy_2;
bool bStarted = false;

const int timerId = 1;
HBRUSH hEllipsBrush;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
    WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        MessageBoxW(hWnd, sMsgBoxDesc, sMsgBoxTitle, MB_OK | MB_ICONQUESTION);
        break;
    }
    case WM_SIZE:
    {
        sx = LOWORD(lParam);
        sy = HIWORD(lParam);
        sx_2 = sx >> 1;
        sy_2 = sy >> 1;
        break;
    }
    case WM_LBUTTONUP:
    {
        if (bStarted)
            break;

        {
            const int cursorPCoordX = LOWORD(lParam);
            const int cursorPCoordY = HIWORD(lParam);

            WINDOWINFO windInfo = {};
            GetWindowInfo(hWnd, &windInfo);

            const int cursorLCoordX = cursorPCoordX - ((windInfo.rcClient.right - windInfo.rcClient.left) / 2);
            const int cursorLCoordY = -(cursorPCoordY - ((windInfo.rcClient.bottom - windInfo.rcClient.top) / 2));

            rectEllips.left += cursorLCoordX;
            rectEllips.right += cursorLCoordX;
            rectEllips.top += cursorLCoordY;
            rectEllips.bottom += cursorLCoordY;
        }

        hEllipsBrush = CreateSolidBrush(RGB(128, 0, 128));

        // генерация начального угла
        srand(static_cast<unsigned>(time(0u)));
        
        const unsigned quarter = static_cast<unsigned>(rand()) % 4u;

        const unsigned angle = static_cast<unsigned>(rand()) % 91u;
        if (angle == 0u)
        {
            ellipsSpeedX = ballSpeed;
            ellipsSpeedY = 0.0;
        }
        else if (angle == 90u)
        {
            ellipsSpeedX = 0.0;
            ellipsSpeedY = ballSpeed;
        }
        else
        {
            const double k = tan(3.1415926535 / 180.0 * angle);
            const double hypotenuse = sqrt(1.0 + k * k);
            const double similarityCoeff = 5.0 / hypotenuse;
            ellipsSpeedX = ballSpeed * similarityCoeff;
            ellipsSpeedY = ballSpeed * similarityCoeff * k;
        }
        
        switch (quarter)
        {
            case 2u:
            {
                ellipsSpeedX = -ellipsSpeedX;
                break;
            }
            case 3u:
            {
                ellipsSpeedX = -ellipsSpeedX;
                ellipsSpeedY = -ellipsSpeedY;
                break;
            }
            case 4u:
            {
                ellipsSpeedY = -ellipsSpeedY;
                break;
            }
            default: 
                break;
        }

        SetTimer(hWnd, timerId, 20, nullptr);
        bStarted = true;
        break;
    }
    case WM_TIMER:
    {
        currentX += ellipsSpeedX;
        currentY += ellipsSpeedY;
        const int iX = static_cast<int>(currentX);
        const int iY = static_cast<int>(currentY);

        rectEllips.left += iX;
        rectEllips.top += iY;
        rectEllips.right += iX;
        rectEllips.bottom += iY;

        currentX -= iX;
        currentY -= iY;

        const bool bLeftAboard = rectEllips.left < -sx_2;
        const bool bRightAboard = rectEllips.right > sx_2;
        if (bLeftAboard || bRightAboard)
        {
            const int dWidthCoord = bLeftAboard 
                ? rectEllips.left + sx_2
                : rectEllips.right - sx_2;

            rectEllips.left -= dWidthCoord * 2;
            rectEllips.right -= dWidthCoord * 2;

            ellipsSpeedX = -ellipsSpeedX;
        }

        const bool bTopAboard = rectEllips.top > sy_2;
        const bool bBottomAboard = rectEllips.bottom < -sy_2;
        if (bTopAboard || bBottomAboard)
        {
            const int dHeightCoord = bTopAboard
                ? rectEllips.top - sy_2
                : rectEllips.bottom + sy_2;

            rectEllips.top -= dHeightCoord * 2;
            rectEllips.bottom -= dHeightCoord * 2;

            ellipsSpeedY = -ellipsSpeedY;
        }

        InvalidateRect(hWnd, nullptr, TRUE);
        break;
    }
    case WM_PAINT:
    {
        if (!bStarted)
            break;
        
        PAINTSTRUCT ps;
        const HDC hdc = BeginPaint(hWnd, &ps);

        SetMapMode(hdc, MM_ANISOTROPIC);
        SetWindowExtEx(hdc, sx, -sy, nullptr);
        SetViewportExtEx(hdc, sx, sy, nullptr);
        SetViewportOrgEx(hdc, sx_2, sy_2, nullptr);

        const HRGN hRgnEllips = CreateEllipticRgnIndirect(&rectEllips);
        FillRgn(hdc, hRgnEllips, hEllipsBrush);
        DeleteObject(hRgnEllips);

        EndPaint(hWnd, &ps);
        
        break;
    }
    case WM_DESTROY:
    {
        if (!bStarted)
        {
            PostQuitMessage(0);
            break;
        }
        
        DeleteObject(hEllipsBrush);
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
    wcex.hIcon = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_EXERCISE13));
    wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_EXERCISE13);
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
    LoadStringW(hInstance, IDC_EXERCISE13, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAcceleratorsW(hInstance, MAKEINTRESOURCE(IDC_EXERCISE13));

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
