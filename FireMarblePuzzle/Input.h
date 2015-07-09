// =====================================
// "Input.h"
// Mouse & Keyboard input centralization
// (See implementation for more info)
// =====================================

#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>


// Forward declaration to Game class
class CGame;

// The CInput class itself
class CInput
{
public:
	CInput();
	~CInput();
	BOOL Init(CGame *game);
	BOOL Poll();
	BOOL IsKeyDown(SHORT key);
	BOOL IsMouseDown(SHORT button);
	POINT GetMousePosition();
	POINT GetMouseDelta();
	VOID Quit();

private:
	BOOL PollKeyboard();
	BOOL PollMouse();

	CGame				*m_game;
	IDirectInput8		*m_di8;
	IDirectInputDevice8	*m_keyboard;
	IDirectInputDevice8	*m_mouse;
	unsigned char		m_keyState[256];
	DIMOUSESTATE		m_mouseState;
	POINT				m_mouseAbs;
	RECT				m_rcClient;
};
