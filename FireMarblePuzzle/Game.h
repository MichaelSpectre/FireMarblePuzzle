// ===================================
// "Game.h"
// Centralization of logic & rendering
// (See implementation for more info)
// ===================================

#pragma once
#include "D2D.h"
#include "Input.h"
#include "GameLogic.h"
#include <chrono>

#define FRAMECOUNT_SPEED_MS 1000

// For brevity, I'm using C++0x/11 time structs,
// as opposed to Win32 HR timer methods (too much
// debate about winmm vs kernel methods in Win32)
typedef std::chrono::high_resolution_clock HRClock;
typedef std::chrono::milliseconds milliseconds;

// The CGame class itself. Designed to be a centralized
// point between all rendering and logic, this class is
// responsible for bridging the gap between everything
class CGame
{
public:
	CGame(HINSTANCE hInst);
	~CGame();
	BOOL Init(HWND hWindow);
	BOOL IsRunning();
	BOOL UpdateFrame();
	BOOL RenderFrame();
	HINSTANCE GetInstanceHandle();
	HWND GetWindowHandle();
	CD2D *GetRenderer();
	std::wstring GetExecutablePath();
	VOID Quit();

private:
	// Object/class variables
	HINSTANCE m_hInst;
	HWND m_hWnd;
	CD2D *m_d2d;
	CInput *m_input;
	CGameLogic *m_logic;

	// Timer/frame variables
	HRClock::time_point m_tNow;
	milliseconds m_elapsed;
	long m_frame;
	int m_fps;

	// State variables
	BOOL m_bInitialized;
	BOOL m_bRunning;
};
