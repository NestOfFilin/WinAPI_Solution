#include "Exercise_1_4.h"

const int MAX_LOADSTRING = 100;

HINSTANCE hInst = nullptr;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name


const TCHAR* sMsgBoxTitle = L"Глава 1 Задание 4";
const TCHAR* sMsgBoxDesc =
L"Программа с окном, разрисованным как шахматная доска.\n\
При нажатии левой кнопки мыши выведет окно сообщений с именем клетки,\n\
где находится курсор в шахматной нотации.";

int sx, sy;
const int CHESS_ROW_SIZE = 8;
const int CHESS_ROW_BORDER_SIZE = 7; // первую и последнюю границу не храним

int aRowBordersCrd[CHESS_ROW_BORDER_SIZE] { 0, 0, 0, 0, 0, 0, 0 };
int aClmnBordersCrd[CHESS_ROW_BORDER_SIZE] { 0, 0, 0, 0, 0, 0, 0 };

HBRUSH hBlackBrsh = nullptr;
HBRUSH hWhiteBrsh = nullptr;

inline int FillChessRect(HDC hDC, const RECT* lprc, const bool& bIsBlack)
{
    return FillRect(hDC, lprc, (bIsBlack ? hBlackBrsh : hWhiteBrsh));
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
    WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        MessageBoxW(hWnd, sMsgBoxDesc, sMsgBoxTitle, MB_OK | MB_ICONQUESTION);
        hBlackBrsh = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
        hWhiteBrsh = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
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
        const int cursorPCoordX = LOWORD(lParam);
        const int cursorPCoordY = HIWORD(lParam);

        int xBorder = CHESS_ROW_BORDER_SIZE >> 1;
        int dx = (xBorder >> 1) + 1;
        int yBorder = xBorder;
        int dy = dx;
        do
        {
            if (dx != 0)
            {
                if (cursorPCoordX > aRowBordersCrd[xBorder])
                {
                    xBorder += dx;
                }
                else if (cursorPCoordX < aRowBordersCrd[xBorder])
                {
                    xBorder -= dx;
                }

                dx = (dx == 1) ? 0 : (dx >> 1);
            }
            if (dy != 0)
            {
                if (cursorPCoordY > aClmnBordersCrd[yBorder])
                {
                    yBorder += dy;
                }
                else if (cursorPCoordY < aClmnBordersCrd[yBorder])
                {
                    yBorder -= dy;
                }

                dy = (dy == 1) ? 0 : (dy >> 1);
            }
            
        } while (dx != 0 || dy != 0);

        int xCell = (cursorPCoordX > aRowBordersCrd[xBorder]) ? xBorder + 1 : xBorder;
        int yCell = (cursorPCoordY > aClmnBordersCrd[yBorder]) ? yBorder + 1 : yBorder;

        String sChessCellMsg;
        sChessCellMsg += static_cast<TCHAR>(L'a' + xCell);
        sChessCellMsg += static_cast<TCHAR>(L'8' - yCell);

        MessageBoxW(hWnd, sChessCellMsg.data(), sChessCellMsg.data(), MB_OK | MB_ICONQUESTION);

        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hDC = BeginPaint(hWnd, &ps);

        SetWindowExtEx(hDC, sx, -sy, nullptr);

        const int dsx = sx / CHESS_ROW_SIZE;
        const int dsy = sy / CHESS_ROW_SIZE;

        const int psx = sx % CHESS_ROW_SIZE;
        const int psy = sy % CHESS_ROW_SIZE;

        int incX = 0;
        int incY = 0;

        int left = 0, right = 0;

        bool bIsBlack = true;
        for (int i = 0; i < CHESS_ROW_SIZE; i++)
        {
            if (incX != psx)
            {
                incX++;
                right++;
            }
            right += dsx;

            if (i != CHESS_ROW_SIZE - 1)
                aRowBordersCrd[i] = right;

            int top = 0, bottom = 0;
            for (int j = 0; j < CHESS_ROW_SIZE; j++)
            {
                if (i == 0)
                {
                    if (incY != psy)
                    {
                        incY++;
                        top++;
                    }
                    top += dsy;

                    if (j != CHESS_ROW_SIZE - 1)
                        aClmnBordersCrd[j] = top;
                }
                else
                {
                    top = (j != CHESS_ROW_SIZE - 1) 
                        ? aClmnBordersCrd[j] 
                        : sy;
                }

                const RECT cellRct = { left , top, right, bottom };

                FillChessRect(hDC, &cellRct, bIsBlack);
                
                bIsBlack = !bIsBlack;
                bottom = top;
            }
            bIsBlack = !bIsBlack;
            left = right;
        }

        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
    {
        hBlackBrsh = nullptr;
        hWhiteBrsh = nullptr;
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
    wcex.hIcon = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_EXERCISE14));
    wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_EXERCISE14);
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
    LoadStringW(hInstance, IDC_EXERCISE14, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAcceleratorsW(hInstance, MAKEINTRESOURCE(IDC_EXERCISE14));

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
