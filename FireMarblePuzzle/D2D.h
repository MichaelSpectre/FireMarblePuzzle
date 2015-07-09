// ==================================
// "D2D.h"
// Direct 2D wrapper for rendering
// (See implementation for more info)
// ==================================

#pragma once
#include <d2d1.h>
#include "Sprite.h"

class CD2D
{
public:
	// Starup/cleanup methods
	CD2D();
	~CD2D();
	BOOL Init(HWND hWindow);
	VOID Quit();

	// Render methods
	VOID Begin();
	VOID Clear(float r = 0.75f, float g = 0.75f, float b = 0.75f);
	VOID DrawSprite(CSprite *sprite);
	VOID End();

	// Getter methods
	ID2D1Factory			*GetFactory()		{ return m_factory; }
	ID2D1HwndRenderTarget	*GetRenderTarget()	{ return m_renderTarget; }

private:

	ID2D1Factory			*m_factory;
	ID2D1HwndRenderTarget	*m_renderTarget;
};

