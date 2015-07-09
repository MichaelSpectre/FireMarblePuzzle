// ===============================
// "main.cpp"
// Entry point, window logic, etc
// ===============================
#pragma once

// Self-explanatory macro definitions
#define CLASS_NAME		L"FMPWindowClass"
#define WINDOW_TITLE	L"Fire Marble Puzzle"
#define WINDOW_WIDTH	316
#define WINDOW_HEIGHT	78
#define WINDOW_TOP		30
#define WINDOW_LEFT		10
#define WINDOW_STYLE	WS_OVERLAPPEDWINDOW

// Forward declaration for message handler
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);