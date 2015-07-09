// =====================================
// "Input.h"
// Mouse & Keyboard input centralization
// =====================================

#include "stdafx.h"
#include "Input.h"
#include "Game.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

// Constructor
CInput::CInput() : m_game(nullptr), m_di8(nullptr), m_keyboard(nullptr), m_mouse(nullptr)
{
	ZeroMemory(&m_mouseAbs, sizeof(POINT));
	ZeroMemory(&m_rcClient, sizeof(RECT));
}

// Destructor
CInput::~CInput()
{
	Quit();
}

BOOL CInput::Init(CGame *game)
{
	ZeroMemory(&m_mouseAbs, sizeof(POINT));
	auto hr = DirectInput8Create(game->GetInstanceHandle(), DIRECTINPUT_VERSION, IID_IDirectInput8, (PVOID*)&m_di8, NULL);
	if (!SUCCEEDED(hr)) return FALSE;

	// Keyboard setup
	hr = m_di8->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (!SUCCEEDED(hr)) return FALSE;
	hr = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (!SUCCEEDED(hr)) return FALSE;
	hr = m_keyboard->SetCooperativeLevel(game->GetWindowHandle(), DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (!SUCCEEDED(hr)) return FALSE;

	// Mouse setup
	hr = m_di8->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (!SUCCEEDED(hr)) return FALSE;
	hr = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (!SUCCEEDED(hr)) return FALSE;
	hr = m_mouse->SetCooperativeLevel(game->GetWindowHandle(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (!SUCCEEDED(hr)) return FALSE;

	// Set initial mouse position
	m_game = game;
	GetClientRect(m_game->GetWindowHandle(), &m_rcClient);

	return TRUE;
}

VOID CInput::Quit()
{
	if (m_mouse != nullptr) m_mouse->Unacquire();
	SAFE_RELEASE(m_mouse);
	if (m_keyboard != nullptr) m_keyboard->Unacquire();
	SAFE_RELEASE(m_keyboard);
	SAFE_RELEASE(m_di8);
}

BOOL CInput::PollKeyboard()
{
	auto hr = m_keyboard->GetDeviceState(sizeof(m_keyState), (LPVOID)&m_keyState);
	if (!SUCCEEDED(hr))
	{
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED)) {
			m_keyboard->Acquire();
		} else {
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CInput::PollMouse()
{
	auto hr = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (!SUCCEEDED(hr))
	{
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED)) {
			m_mouse->Acquire();
		} else {
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CInput::Poll()
{
	if (!PollKeyboard()) return FALSE;
	if (!PollMouse()) return FALSE;
	
	GetClientRect(m_game->GetWindowHandle(), &m_rcClient);
	auto scrW = GetSystemMetrics(SM_CXSCREEN);
	auto scrH = GetSystemMetrics(SM_CYSCREEN);

	GetCursorPos(&m_mouseAbs);
	if (m_mouseAbs.x < 0) m_mouseAbs.x = 0;
	if (m_mouseAbs.y < 0) m_mouseAbs.y = 0;
	if (m_mouseAbs.x > scrW) m_mouseAbs.x = scrW;
	if (m_mouseAbs.y > scrH) m_mouseAbs.y = scrH;

	return TRUE;
}

BOOL CInput::IsKeyDown(SHORT key)
{
	return m_keyState[key] & 0x80;
}

BOOL CInput::IsMouseDown(SHORT button)
{
	return m_mouseState.rgbButtons[button] & 0x80;
}

POINT CInput::GetMousePosition()
{
	POINT pt = m_mouseAbs;
	HWND hWnd = m_game->GetWindowHandle();
	ScreenToClient(hWnd, &pt);

	auto w = m_rcClient.right - m_rcClient.left;
	auto h = m_rcClient.bottom - m_rcClient.top;
	if (pt.x < 0) pt.x = 0;
	if (pt.y < 0) pt.y = 0;
	if (pt.x > w) pt.x = w;
	if (pt.y > h) pt.y = h;

	return pt;
}

POINT CInput::GetMouseDelta()
{
	return { m_mouseState.lX, m_mouseState.lY };
}