// ===================================
// "Game.cpp"
// Centralization of logic & rendering
// ===================================

#include "stdafx.h"
#include "Game.h"
#include "main.h"

// Constructor
CGame::CGame(HINSTANCE hInst) : m_hInst(hInst), m_hWnd(NULL),
m_frame(0), m_fps(0), m_bInitialized(FALSE), m_bRunning(FALSE), 
m_d2d(nullptr), m_input(nullptr), m_logic(nullptr)
{
}

// Destructor
CGame::~CGame()
{
	m_bRunning = FALSE;
	m_bInitialized = FALSE;

	// Ensure that all objects are released
	if (m_logic != nullptr) {
		delete m_logic;
		m_logic = nullptr;
	}
	if (m_input != nullptr) {
		delete m_input;
		m_input = nullptr;
	}
	if (m_d2d != nullptr) {
		delete m_d2d;
		m_d2d = nullptr;
	}
}

// Basic initialization checking
BOOL CGame::Init(HWND hWindow)
{
	// Make sure everything is in working order
	if (!m_bInitialized)
	{
		// Validate window handle
		m_bInitialized = (m_hWnd = hWindow) != NULL;
		if (!m_bInitialized) {
			MessageBox(NULL, L"Invalid window handle detected!", L"Error", MB_OK | MB_ICONERROR);
			return FALSE; // Effectively tells the application to exit
		}

		// Validate renderer
		m_d2d = new CD2D();
		m_bInitialized = m_d2d->Init(m_hWnd);
		if (!m_bInitialized) {
			MessageBox(NULL, L"Failed to initialize graphics renderer!", L"Error", MB_OK | MB_ICONERROR);
			return FALSE; // Effectively tells the application to exit
		}

		// Validate input
		m_input = new CInput();
		m_bInitialized = m_input->Init(this);
		if (!m_bInitialized) {
			MessageBox(NULL, L"Failed to initialize input class!", L"Error", MB_OK | MB_ICONERROR);
			return FALSE; // Effectively tells the application to exit
		}

		// Validate game logic
		m_logic = new CGameLogic();
		m_bInitialized = m_logic->Init(this);
		if (!m_bInitialized) {
			MessageBox(NULL, L"Failed to initialize core game logic!", L"Error", MB_OK | MB_ICONERROR);
			return FALSE; // Effectively tells the application to exit
		}

		// Set the clock & running flag
		m_tNow = HRClock::now();
		m_bRunning = TRUE;
	}

	// Should be true by now, but just in case
	return m_bInitialized && m_bRunning;
}

// Checks if we have a valid session or not
BOOL CGame::IsRunning()
{
	return m_bRunning;
}

// Handles each frame of our game, passing
// game logic wherever it needs to go. Also
// keeps track of time, and frames per sec
BOOL CGame::UpdateFrame()
{
	// Disallow uninitialized usage
	if (!m_bInitialized) return FALSE;

	// Update our timers
	auto now = HRClock::now();
	m_elapsed = std::chrono::duration_cast<milliseconds>(now - m_tNow);
	++m_frame;

	// Display the FPS in the window title
	if (m_elapsed.count() >= FRAMECOUNT_SPEED_MS) {
		m_fps = m_frame;
		m_frame = 0;
		m_tNow = now;

		std::wostringstream title;
		title << WINDOW_TITLE << L" (" << m_fps << L" FPS)";
		SetWindowText(m_hWnd, title.str().c_str());
	}

	// Process the input queue
	if (!m_input->Poll()) {
		MessageBox(NULL, L"Failed to poll an input device!", L"Error", MB_OK | MB_ICONERROR);
		Quit();
		return FALSE;
	}

	// Process game logic
	m_logic->Update(this, m_input);

	// Force a new frame to be drawn
	RedrawWindow(m_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT);
	return m_bRunning;
}


// Renders each frame of our game, passing
// render logic to wherever it needs to go.
BOOL CGame::RenderFrame()
{
	// Disallow uninitialized or invalid usage
	if (!m_bInitialized || !m_bRunning) return FALSE;

	// Begin rendering
	m_d2d->Begin();
	m_d2d->Clear(0, 0, 0);

	// Render all sprites
	for (auto spr : m_logic->GetSprites()) {
		if (spr != nullptr) {
			spr->Render(m_d2d);
		}
	}

	// End rendering
	m_d2d->End();

	return TRUE;
}

HINSTANCE CGame::GetInstanceHandle() {
	return m_hInst;
}

HWND CGame::GetWindowHandle() {
	return m_hWnd;
}

CD2D *CGame::GetRenderer() {
	return m_d2d;
}

// Helper function to get the folder path
// where the current executable is running from
std::wstring CGame::GetExecutablePath()
{
	wchar_t result[MAX_PATH];
	DWORD dwRet = GetModuleFileNameW(NULL, result, MAX_PATH);
	std::wstring path(result, dwRet);
	std::size_t found = path.find_last_of(L"\\");
	path = path.substr(0, found);
	return path;
}

VOID CGame::Quit()
{
	m_bRunning = FALSE;
}