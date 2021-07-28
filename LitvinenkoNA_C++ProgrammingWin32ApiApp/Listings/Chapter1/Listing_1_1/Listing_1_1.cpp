// ������� 1.1. ����������� ��� ������� Windows-����������

#include <Windows.h>
#include <tchar.h>
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
TCHAR WinName[] = _T("MainFrame");
// �.�. ��� ������, �� ������� ������� ���������� wWinMain, � ��� _tWinMain
int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance,					// ���������� �������� ����������
	_In_opt_ HINSTANCE hPrevInstance,			// � ����������� �������� ������ 0
	_In_ LPWSTR lpCmdLine,						// ��������� ������
	_In_ int nShowCmd)							// ����� ����������� ����
{
	HWND hWnd;              // ���������� �������� ���� ���������
	MSG msg;                // ��������� ��� �������� ���������
	WNDCLASS wc;  // ����� ����
// ����������� ������ ����
	wc.hInstance = hInstance;
	wc.lpszClassName = WinName;                // ��� ������ ����
	wc.lpfnWndProc = WndProc;                  // ������� ����
	wc.style = CS_HREDRAW | CS_VREDRAW;       // ����� ����
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);    // ����������� ������
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // ����������� ������
	wc.lpszMenuName = NULL;       // ��� ����
	wc.cbClsExtra = 0;            // ��� �������������� ������ ������
	wc.cbWndExtra = 0;            // ��� �������������� ������ ����
	// ���������� ���� ����� ������
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	if (!RegisterClass(&wc)) return 0;   // ����������� ������ ����
// �������� ����
	hWnd = CreateWindow(WinName, // ��� ������ ����
		_T("������ Windows-����������"),  // ��������� ����
		WS_OVERLAPPEDWINDOW,         // ����� ����
		CW_USEDEFAULT, // x
		CW_USEDEFAULT, // y   ������� ����
		CW_USEDEFAULT, // width
		CW_USEDEFAULT, // Height
		HWND_DESKTOP, // ���������� ������������� ����
		NULL,         // ��� ����
		hInstance,         // ���������� ����������
		NULL);        // �������������� ���������� ���
	ShowWindow(hWnd, nShowCmd); // �������� ����
// ���� ��������� ���������
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg); // ������� ���������� ����� ������� �������
		DispatchMessage(&msg);  // �������� ��������� ������� WndProc()
	}
	return 0;
}
// ������� ������� ���������� ������������ ��������
// � �������� ��������� �� ������� ��� ������� ����������
LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{       // ���������� ���������
	switch (message)
	{
	case WM_DESTROY: PostQuitMessage(0);
		break; // ���������� ���������
// ��������� ��������� �� ���������
	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}