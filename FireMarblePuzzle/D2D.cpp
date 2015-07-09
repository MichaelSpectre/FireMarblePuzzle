// ==================================
// "D2D.cpp"
// Direct 2D wrapper for rendering
// ==================================
#include "stdafx.h"
#include "D2D.h"

#pragma comment(lib, "d2d1.lib")

// Contructor
CD2D::CD2D() : m_factory(nullptr), m_renderTarget(nullptr)
{
	// Nothing to do here (and RAII isn't optimal for this class)
}

// Destructor
CD2D::~CD2D()
{
	// Ensures no memory/resource leaks will occur
	Quit();
}

// Direct2D initialization routines
BOOL CD2D::Init(HWND hWindow)
{
	// Create Direct2D factory COM object, or return an error
	auto hr = D2D1CreateFactory(D2D1_FACTORY_TYPE::D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_factory);
	if (!SUCCEEDED(hr)) {
		MessageBox(NULL, L"Failed to create Direct2D Factory!", L"Error", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	// Set up our D2D render target parameters
	RECT rc = { 0, 0, 0, 0 };
	GetClientRect(hWindow, &rc); // Yes, I realize I'm not error-checking here, but it's just a quick demo
	D2D1_HWND_RENDER_TARGET_PROPERTIES hwrtProps = D2D1::HwndRenderTargetProperties(
		hWindow, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)
	);

	// Create Direct2D render target, or return an error
	hr = m_factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), hwrtProps, &m_renderTarget);
	if (!SUCCEEDED(hr)) {
		MessageBox(NULL, L"Failed to create Direct2D Factory!", L"Error", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}

// Cleans up Direct2D resources
VOID CD2D::Quit()
{
	// Only release valid objects, using the
	// FILO (first in, last out) strategy
	SAFE_RELEASE(m_renderTarget);
	SAFE_RELEASE(m_factory);
}

VOID CD2D::Begin() {
	m_renderTarget->BeginDraw();
}

VOID CD2D::Clear(float r, float g, float b) {
	m_renderTarget->Clear(D2D1::ColorF(r, g, b));
}

VOID CD2D::DrawSprite(CSprite *sprite) {
	sprite->Render(this);
}

VOID CD2D::End() {
	m_renderTarget->EndDraw();
}