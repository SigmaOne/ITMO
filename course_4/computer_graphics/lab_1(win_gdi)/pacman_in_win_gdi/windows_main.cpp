#include <stdafx.h>
#include <windows.h>
#include <objidl.h>
#include "Painter.h"
#include "WorldEngine.h"

WorldEngine WORLD_ENGINE;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;

	Painter painter;
	char** map = WORLD_ENGINE.getMap();
	int** character_coordinates = WORLD_ENGINE.getCharacterCoordinates();

	switch (message) {
	case WM_TIMER:
		switch (wParam) {
		case 1:
			WORLD_ENGINE.tick();
			InvalidateRect(hWnd, NULL, true);
		break;

		default:
		break;
		}

	case WM_KEYDOWN:
		if (wParam != VK_DOWN && wParam != VK_UP && wParam != VK_LEFT && wParam != VK_RIGHT) {
			return 0;
		}
		
		WORLD_ENGINE.changeDirection(wParam);
		return 0;
	
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		painter.drawWorld(hdc, map, character_coordinates);
		EndPaint(hWnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow) {
	HWND                hWnd;
	MSG                 msg;
	WNDCLASS            wndClass;
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;

	// Initialize GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = TEXT("GettingStarted");

	RegisterClass(&wndClass);

	hWnd = CreateWindow(
		TEXT("GettingStarted"),   // window class name
		TEXT("Getting Started"),  // window caption
		WS_OVERLAPPEDWINDOW,      // window style
		CW_USEDEFAULT,            // initial x position
		CW_USEDEFAULT,            // initial y position
		CW_USEDEFAULT,            // initial x size
		CW_USEDEFAULT,            // initial y size
		NULL,                     // parent window handle
		NULL,                     // window menu handle
		hInstance,                // program instance handle
		NULL);                    // creation parameters

	SetTimer(hWnd, 1, 100, (TIMERPROC)  NULL); 
	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	GdiplusShutdown(gdiplusToken);
	return msg.wParam;
}
