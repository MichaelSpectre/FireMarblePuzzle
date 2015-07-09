// ===============================
// "main.cpp"
// Entry point, window logic, etc
// ===============================

#include "stdafx.h"
#include "main.h"
#include "Game.h"

// Global pointer to our game class (normally, I like to stay away from these)
CGame *g_pGame = nullptr;

// Entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HeapSetInformation(nullptr, HeapEnableTerminationOnCorruption, nullptr, 0);
	HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (!SUCCEEDED(hr)) {
		MessageBox(NULL, L"Failed to initialize COM library!", L"Error", MB_OK | MB_ICONERROR);
		return -2;
	}
	
	HWND hWnd;		// Window handle
	WNDCLASSEX wc;	// Window class

	// Basic window class data
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = CLASS_NAME;

	// Window class registration
	auto result = RegisterClassEx(&wc);
	// Basic error checking
	if (result == 0) {
		MessageBox(NULL, L"Failed to create window class!", L"Error", MB_OK | MB_ICONERROR);
		return -2;
	}

	// Adjust window dimensions to utilize desired client size
	RECT rc = { WINDOW_LEFT, WINDOW_TOP, WINDOW_LEFT + WINDOW_WIDTH, WINDOW_TOP + WINDOW_HEIGHT };
	AdjustWindowRectEx(&rc, WINDOW_STYLE, FALSE, NULL);

	// Window creation
	hWnd = CreateWindowEx(NULL,		// No extended styles
		CLASS_NAME, WINDOW_TITLE,	// Class name, window title
		WINDOW_STYLE,				// Window style
		rc.left, rc.top,			// x, y
		rc.right - rc.left,			// width
		rc.bottom - rc.top,			// height
		NULL, NULL,					// No parent window, no menus
		hInstance,					// Instance handle
		NULL);						// No additional params needed
	SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
	ShowWindow(hWnd, nCmdShow);

	// Initialize our game class
	g_pGame = new CGame(hInstance);
	if (!g_pGame->Init(hWnd)) {
		MessageBox(NULL, L"Failed to initialize game window!", L"Error", MB_OK | MB_ICONERROR);
		UnregisterClass(CLASS_NAME, hInstance);
		delete g_pGame;
		return -3;
	}

	// Enter an infinite message loop
	MSG msg;
	while (g_pGame->IsRunning())
	{
		// Check the message queue
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
			break;

		// Transfer control to our game logic...
		// If this returns false, we want to exit
		if (!g_pGame->UpdateFrame()) {
			PostQuitMessage(0);
		}
	}

	// Free up resources
	UnregisterClass(CLASS_NAME, hInstance);
	delete g_pGame;
	CoUninitialize();

	// Return control to the OS
	return msg.wParam;
}

// Just a basic message handler
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		break;
		case WM_PAINT:
		{
			if (!g_pGame->RenderFrame()) {
				PostQuitMessage(0);
				return 0;
			}
		}
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}