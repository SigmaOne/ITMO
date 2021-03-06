#include <stdafx.h>
#include <windows.h>
#include <objidl.h>
#include "Painter.h"
#include "WorldEngine.h"
#include <time.h>

WorldEngine WORLD_ENGINE;
int frames_draw_counter = 0;

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
		if (!GetAsyncKeyState(VK_DOWN) && !GetAsyncKeyState(VK_UP) && !GetAsyncKeyState(VK_LEFT) && !GetAsyncKeyState(VK_RIGHT)) {
			if (wParam != VK_DOWN && wParam != VK_UP && wParam != VK_LEFT && wParam != VK_RIGHT) {
				return 0;
			}
		}

		WORLD_ENGINE.changeDirection(PACMAN_CHARACTER_INDEX, wParam);
		return 0;
	
	case WM_ERASEBKGND:
		return 1;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		if (WORLD_ENGINE.isGameOver()) {
			painter.drawGameOverScreen(hdc);
		} else if (WORLD_ENGINE.isPlayerWins()) {
			painter.drawWinScreen(hdc);
        } else  {
			painter.drawWorld(hdc, map, character_coordinates);
		}

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
	    791,                      // initial x size
		388,                      // initial y size
		NULL,                     // parent window handle
		NULL,                     // window menu handle
		hInstance,                // program instance handle
		NULL);                    // creation parameters

	SetTimer(hWnd, 1, 50, (TIMERPROC)  NULL); 
	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);
	srand(time(0));

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	GdiplusShutdown(gdiplusToken);
	return msg.wParam;
}
